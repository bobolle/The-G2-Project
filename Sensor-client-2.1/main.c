#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "hardware/adc.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
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

// Mqtt-client object
static mqtt_client_t *mqtt_client;
volatile bool wifi_connected = false;

// Global variables for sensors
volatile uint16_t latest_light = 0;
volatile uint16_t latest_moist = 0;

uint16_t read_photoresistor() {
  adc_select_input(0);
  return adc_read();
}

uint16_t read_moisture() {
  adc_select_input(1);
  return adc_read();
}

// Callback for connection
static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status) {
  if (status == MQTT_CONNECT_ACCEPTED) {
    printf("MQTT: Connected to broker!\n");
  } else {
    printf("MQTT: Connection failed, status: %d\n", status);
  }
}

// Callback for pub
static void mqtt_pub_request_cb(void *arg, err_t result) {
  if (result != ERR_OK) {
    printf("MQTT: Publication failed: %d\n", result);
  } else {
    printf("MQTT: Publication succeeded.\n");
  }
}
void maintain_wifi(void *pvParameters) {
  while (1) {
    if (!wifi_connected) {
      printf("Trying to connect to WiFi: SSID=[%s], PASSWORD=[%s]\n", WIFI_SSID,
             WIFI_PASS);
      if (cyw43_arch_wifi_connect_timeout_ms(
              WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000) == 0) {
        printf("WiFi connected!\n");
        wifi_connected = true;
      } else {
        printf("WiFi reconnect failed.\n");
        wifi_connected = false;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void sensor_task(void *pvParameters){
  while(1) {
    latest_light = read_photoresistor();
    latest_moist = read_moisture();
    printf("Sensor readings - Light: %u, Moisture: %u\n", latest_light, latest_moist);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void mqtt_task(void *pvParameters) {
  mqtt_client = mqtt_client_new();
  struct mqtt_connect_client_info_t client_info = {.client_id = "pico",
                                                   .client_user = NULL,
                                                   .client_pass = NULL,
                                                   .keep_alive = 60,
                                                   .will_topic = NULL,
                                                   .will_msg = NULL,
                                                   .will_qos = 0,
                                                   .will_retain = 0};

  // Change broker-IP from string to id_addr_t
  ip_addr_t broker_ip;
  ip4addr_aton(MQTT_BROKER_IP, &broker_ip);

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
      vTaskDelay(pdMS_TO_TICKS(5000));
      continue;
    }

    while (wifi_connected) {
      char msg[50];
      snprintf(msg, sizeof(msg), "Light level: %u Moist level: %u\n", latest_light,
               latest_moist);
      printf("Publishing to %s:%d: %s\n", MQTT_BROKER_IP, MQTT_BROKER_PORT, msg);

      err = mqtt_publish(mqtt_client, MQTT_TOPIC, (const u8_t *)msg,
                         strlen(msg), 0, 0, mqtt_pub_request_cb, NULL);
      if (err != ERR_OK) {
        printf("Failed to publish MQTT-message: %d\n", err);
      }
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}


int main() {
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
    cyw43_arch_deinit();
    return 0;
  }
}