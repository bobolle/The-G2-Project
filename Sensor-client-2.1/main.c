#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "hardware/adc.h"
#include "hardware/watchdog.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "pico/bootrom.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "task.h"

#ifndef WIFI_SSID
#define WIFI_SSID "default_ssid"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "default_password"
#endif

#ifndef MQTT_BROKER_IP
#define MQTT_BROKER_IP "localhost"
#endif

#ifndef MQTT_BROKER_PORT
#define MQTT_BROKER_PORT 1883
#endif

#ifndef MQTT_TOPIC
#define MQTT_TOPIC "sensors/edgedevice"
#endif

#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD "default_password"
#endif

#ifndef MQTT_USER
#define MQTT_USER "default_user"
#endif

// Error codes
typedef enum {
  ERROR_NONE = 0,
  ERROR_WIFI_DISCONNECTED,
  ERROR_MQTT_DISCONNECTED,
  ERROR_SENSOR_READ,
  ERROR_MQTT_PUBLISH,
  ERROR_SYSTEM_REBOOT
} error_code_t;

volatile error_code_t current_error = ERROR_NONE;
__attribute__((section(".uninitialized_data"))) volatile bool system_rebooted;

// Mqtt-client object
static mqtt_client_t *mqtt_client;
volatile bool wifi_connected = false;

// Global variables for sensors
volatile uint16_t latest_light = 0;
volatile uint16_t latest_moist = 0;

void restart() {
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
    printf("Rebooting in 3 seconds...\n");
    vTaskDelay(pdMS_TO_TICKS(3000));  // Only delay if RTOS is running
  }
  system_rebooted = true;
  printf("Reboot triggered.\n");
  watchdog_enable(1000, 1);
  watchdog_reboot(0,0,1000);
}

uint16_t read_photoresistor() {
  adc_select_input(0);
  uint16_t value = adc_read();
  bool error_detected = (value == 0 || value == 0xFFFF);

  if (error_detected) {
    current_error = ERROR_SENSOR_READ;
  } else if (!error_detected && latest_moist != 0xFFFF) {
    if (current_error == ERROR_SENSOR_READ) {
      current_error = ERROR_NONE;
    }
  }
  printf("Photoresistor value: %u\n", value); 
  return value;
}

uint16_t read_moisture() {
  adc_select_input(1);
  uint16_t value = adc_read();
  bool error_detected = (value == 0 || value == 0xFFFF);

  if (error_detected) {
    current_error = ERROR_SENSOR_READ;
  } else if (!error_detected && latest_light != 0xFFFF) {
    if (current_error == ERROR_SENSOR_READ) {
      current_error = ERROR_NONE;
    }
  }
  printf("Moisture sensor value: %u\n", value); 
  return value;
}

// Callback for MQTT connection
static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status) {
  if (status == MQTT_CONNECT_ACCEPTED) {
    printf("MQTT: Connected to broker!\n");
    current_error = ERROR_NONE;
  } else {
    printf("MQTT: Connection failed, status: %d\n", status);
    current_error = ERROR_MQTT_DISCONNECTED;
  }
}

// Callback for MQTT publish request
static void mqtt_pub_request_cb(void *arg, err_t result) {
  if (result != ERR_OK) {
    printf("MQTT: Publication failed: %d\n", result);
    current_error = ERROR_MQTT_PUBLISH;
  } else {
    printf("MQTT: Publication succeeded.\n");
    if (current_error == ERROR_MQTT_PUBLISH) {
      current_error = ERROR_NONE;
    }
  }
}

// Wi-Fi connection task
void maintain_wifi(void *pvParameters) {
  int wifi_attempts = 0;
  while (1) {
    if (!wifi_connected) {
      printf("Trying to connect to WiFi: SSID=[%s]\n", WIFI_SSID);
      if (cyw43_arch_wifi_connect_timeout_ms(
              WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000) == 0) {
        printf("WiFi connected!\n");
        wifi_connected = true;
        current_error = ERROR_NONE;
        wifi_attempts = 0;
      } else {
        printf("WiFi reconnect failed.\n");
        wifi_connected = false;
        current_error = ERROR_WIFI_DISCONNECTED;
        wifi_attempts++;
      }
      if (wifi_attempts >= 5) {
        printf("Too many WiFi failures, rebooting device..\n");
        restart();
      }
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

// Sensor reading task
void sensor_task(void *pvParameters) {
  while (1) {
    latest_light = read_photoresistor();
    latest_moist = read_moisture();
    vTaskDelay(pdMS_TO_TICKS(15000));  // Delay for sensor readings
  }
}

// MQTT task for connection and publishing
void mqtt_task(void *pvParameters) {
  mqtt_client = mqtt_client_new();
  struct mqtt_connect_client_info_t client_info = {.client_id = "pico",
                                                   .client_user = MQTT_USER,
                                                   .client_pass = MQTT_PASSWORD,
                                                   .keep_alive = 60,
                                                   .will_topic = NULL,
                                                   .will_msg = NULL,
                                                   .will_qos = 0,
                                                   .will_retain = 0};

  // Change broker-IP from string to id_addr_t
  ip_addr_t broker_ip;
  ip4addr_aton(MQTT_BROKER_IP, &broker_ip);
  int mqtt_attempts = 0;

  while (1) {
    while (!wifi_connected) {
      printf("Waiting for WiFi connection...\n");
      vTaskDelay(pdMS_TO_TICKS(5000));
    }

    // Connect to broker
    printf("Connecting to MQTT-broker at: %s:%d\n", MQTT_BROKER_IP,
           MQTT_BROKER_PORT);
    err_t err = mqtt_client_connect(mqtt_client, &broker_ip, MQTT_BROKER_PORT,
                                    mqtt_connection_cb, NULL, &client_info);
    if (err != ERR_OK) {
      printf("Failed to start MQTT-connection: %d, retrying in 5s...\n", err);
      current_error = ERROR_MQTT_DISCONNECTED;

      if (mqtt_client != NULL) {
        mqtt_client_free(mqtt_client);
        mqtt_client = NULL;
      }

      mqtt_client = mqtt_client_new();
      if (mqtt_client == NULL) {
        mqtt_attempts++;
        printf("Failed to allocate new MQTT client, retrying..\n");

        if (mqtt_attempts >= 10) {
          printf("Too many MQTT failures, rebooting device..\n");
          restart();
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
        continue;
      }
      mqtt_attempts = 0;
      vTaskDelay(pdMS_TO_TICKS(5000));
      continue;
    }

    while (wifi_connected) {
      char msg[150];

      if (system_rebooted) {
        snprintf(
            msg, sizeof(msg),
            "{\"device_id\": \"Pico01\", \"lightlevel\": %u, \"moisture\": %u, "
            "\"error_code\": %d, \"message\": \"Rebooted\"}",
            latest_light, latest_moist, ERROR_SYSTEM_REBOOT);
        system_rebooted = false;
      } else {
        snprintf(
            msg, sizeof(msg),
            "{\"device_id\": \"Pico01\", \"lightlevel\": %u, \"moisture\": "
            "%u, \"error_code\": %d}",
            latest_light, latest_moist, current_error);
      }

      printf("Publishing to %s:%d: %s\n", MQTT_BROKER_IP, MQTT_BROKER_PORT,
             msg);

      err = mqtt_publish(mqtt_client, MQTT_TOPIC, (const u8_t *)msg,
                         strlen(msg), 0, 0, mqtt_pub_request_cb, NULL);
      if (err != ERR_OK) {
        printf("Failed to publish MQTT-message: %d\n", err);
        current_error = ERROR_MQTT_PUBLISH;
        break;
      }
      vTaskDelay(pdMS_TO_TICKS(15000));  // Delay after publishing
    }
  }
}

int main() {
  printf("Starting system...\n");

  stdio_init_all();
  if (cyw43_arch_init()) {
    printf("WiFi initialization failed!\n");
    return 1;
  }
  cyw43_arch_enable_sta_mode();
  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);

  xTaskCreate(maintain_wifi, "WiFi Task", 1024, NULL, 1, NULL);
  xTaskCreate(sensor_task, "Sensor Task", 1024, NULL, 1, NULL);
  xTaskCreate(mqtt_task, "MQTT Task", 1024, NULL, 1, NULL);

  vTaskStartScheduler();

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(100));
    cyw43_arch_deinit();
    return 0;
  }
}