#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "hardware/adc.h"
#include "hardware/watchdog.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "semphr.h"
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

// Sleep intervalls in milliseconds.
#define SENSOR 1030
#define SEND 2000
#define RETRY 5000

// Error codes
typedef enum {
  ERROR_NONE = 0,
  ERROR_WIFI_DISCONNECTED,
  ERROR_MQTT_DISCONNECTED,
  ERROR_SENSOR_READ,
  ERROR_MQTT_PUBLISH,
  ERROR_SYSTEM_REBOOT
} error_code_t;

// GLOBALS
volatile error_code_t current_error = ERROR_NONE;
__attribute__((section(".uninitialized_data"))) volatile bool system_rebooted;
__attribute__((section(".uninitialized_data"))) volatile char last_log[100];
SemaphoreHandle_t sensor_mutex;
volatile uint16_t latest_light = 0;
volatile uint16_t latest_moist = 0;
volatile bool wifi_connected = false;

// Mqtt-client object
static mqtt_client_t *mqtt_client;

// CRASH FUNCTIONS!!!! :@@@
// ################################################################
void crash_stack_overflow() {
  printf("STACK OVERFLOW MAN...\n");
  crash_stack_overflow();
}
void infinite_loop() {
  printf("INFINITE LOOP MOTHERFUCKER!");
  while (1) {
  }
}
void vApplicationMallocFailedHook(void) {
  printf("Memory allocation failed! Out of RAM.\n");
  while (1) {
  }
}
void mem_leak() {
  printf("Memory leak!!! DONT DO THIS AT HOME.\n");
  while (1) {
    void *ptr = pvPortMalloc(1024);
    if (ptr == NULL) {
      printf("Memory allocation failed! Memory is exhausted :((((.\n");
    }
  }
}
void null_pointer_crash() {
  printf("Trying to write to a null-pointer ehehe.\n");
  int *ptr = NULL;
  *ptr = 42;
}
void the_eternal_loop_task(void *pvParameters) {
  printf("This task is stupid and will loop forever.\n");
  volatile uint32_t i = 0;
  while (1) {
    i++;
  }
}
void crash_wifi() {
  printf("Crashing the wifi connection!\n");
  cyw43_arch_deinit();
}
void crash_mqtt() {
  printf("Killing the MQTT client!\n");
  mqtt_client_free(mqtt_client);
  mqtt_client = NULL;
}
void flood_mqtt() {
  printf("Spamming MQTT with large message!");
  char large_msg[1024];
  memset(large_msg, 'A', sizeof(large_msg) - 1);
  large_msg[sizeof(large_msg) - 1];
  while (1) {
    mqtt_publish(mqtt_client, MQTT_TOPIC, (const u8_t *)large_msg,
                 strlen(large_msg), 0, 0, mqtt_pub_request_cb, NULL);
    printf("Published a large message!\n");
  }
}
void block_network_a_while() {
  printf("Closing WifI temporarly..\n");
  cyw43_arch_deinit();
  vTaskDelay(pdMS_TO_TICKS(4000));
  cyw43_arch_init();
  printf("Wifi back online now.\n");
}
// ###############################################################

// Save logs after restart 
void log_message(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsnprintf((char *)last_log, sizeof(last_log), format, args);
  va_end(args);
  printf("%s\n", last_log);
}

uint16_t read_photoresistor() {
  adc_select_input(0);
  uint16_t value = adc_read();
  printf("Photoresistor value: %u\n", value);
  return value;
}

uint16_t read_moisture() {
  adc_select_input(1);
  uint16_t value = adc_read();
  printf("Moisture sensor value: %u\n", value);
  return value;
}

// Callback for MQTT connection
static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status) {
  if (status == MQTT_CONNECT_ACCEPTED) {
    printf("MQTT: Connected to broker!\n");
  } else {
    printf("MQTT: Connection failed, status: %d\n", status);
  }
}

// Callback for MQTT publish request
static void mqtt_pub_request_cb(void *arg, err_t result) {
  if (result != ERR_OK) {
    printf("MQTT: Publication failed: %d\n", result);
  } else {
    printf("MQTT: Publication succeeded.\n");
  }
}

// DEBUG
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  printf("Stack overflow detected in task: %s\n", pcTaskName);
  stdio_flush();
  vTaskDelay(pdMS_TO_TICKS(500));
  watchdog_reboot(0, 0, 0);
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
        // watchdog_reboot(0, 0, 0);
      }
      vTaskDelay(pdMS_TO_TICKS(RETRY));
    }
    vTaskDelay(pdMS_TO_TICKS(RETRY));
  }
}

// Sensor reading task
void sensor_task(void *pvParameters) {
  while (1) {
    xSemaphoreTake(sensor_mutex, portMAX_DELAY);
    latest_light = read_photoresistor();
    latest_moist = read_moisture();
    if (latest_light == 0 || latest_moist == 0 || latest_light == 0xFFFF ||
        latest_moist == 0xFFFF) {
      current_error = ERROR_SENSOR_READ;
    } else {
      current_error = ERROR_NONE;
    }
    xSemaphoreGive(sensor_mutex);
    vTaskDelay(pdMS_TO_TICKS(SENSOR));
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
      vTaskDelay(pdMS_TO_TICKS(RETRY));
    }

    // Connect to broker
    printf("Connecting to MQTT-broker at: %s:%d\n", MQTT_BROKER_IP,
           MQTT_BROKER_PORT);
    err_t err = mqtt_client_connect(mqtt_client, &broker_ip, MQTT_BROKER_PORT,
                                    mqtt_connection_cb, NULL, &client_info);
    printf("MQTT Connect returned: %d\n", err);
    vTaskDelay(pdMS_TO_TICKS(500));
    if (err != ERR_OK) {
      printf("Failed to start MQTT-connection: %d, retrying in 5s...\n", err);
      current_error = ERROR_MQTT_DISCONNECTED;
      vTaskDelay(pdMS_TO_TICKS(RETRY));

      if (mqtt_client != NULL) {
        mqtt_disconnect(mqtt_client);
        mqtt_client_free(mqtt_client);
        mqtt_client = NULL;
        vTaskDelay(pdMS_TO_TICKS(1000));
      }

      mqtt_client = mqtt_client_new();
      if (mqtt_client == NULL) {
        mqtt_attempts++;
        printf("Failed to allocate new MQTT client, retrying..\n");

        if (mqtt_attempts >= 10) {
          printf("Too many MQTT failures, rebooting device..\n");
          watchdog_reboot(0, 0, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(RETRY));
        continue;
      }
      mqtt_attempts = 0;
      vTaskDelay(pdMS_TO_TICKS(RETRY));
      continue;;
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
        vTaskDelay(pdMS_TO_TICKS(RETRY));
        continue;
      }
      vTaskDelay(pdMS_TO_TICKS(SEND));
    }
  }
}

void watchdog_task(void *pvParameters) {
  printf("Watchdog task started...\n");
  for (int i = 0; i < 8; i++) {
    printf("Updating watchdog...\n");
    watchdog_update();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  printf("Watchdog init complete, entering monitoring loop...\n");
  if (strlen((char *)last_log) > 0) {
    printf("Last message before reboot: %s\n", last_log);
  }
  while (1) {
    bool system_ok = true;

    if (!wifi_connected) {
      system_ok = false;
      log_message("WiFi is not working.\n");
      printf("REBOOTING");
    }
    if (mqtt_client == NULL) {
      system_ok = false;
      log_message("MQTT-client is not working.\n");
      printf("REBOOTING");
    }
    if (system_ok) {
      printf("System OK! Watchdog uppdated.\n");
      watchdog_update();
    } else {
      log_message("Watchdog did not update, system will restart!\n");
      printf("REBOOTING");
      stdio_flush();
      vTaskDelay(pdMS_TO_TICKS(500));
      watchdog_reboot(0, 0, 0);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
int main() {
  printf("Starting system...\n");
  watchdog_enable(15000, 1);
  stdio_init_all();
  if (cyw43_arch_init()) {
    printf("WiFi initialization failed!\n");
    return 1;
  }
  cyw43_arch_enable_sta_mode();
  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);
  sensor_mutex = xSemaphoreCreateMutex();

  if (sensor_mutex == NULL) {
    printf("Mutex creation failed!\n");
    return 1;
  }

  xTaskCreate(maintain_wifi, "WiFi Task", 1024, NULL, 2, NULL);
  xTaskCreate(watchdog_task, "Watchdog Task", 512, NULL,
              configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(sensor_task, "Sensor Task", 2048, NULL, 2, NULL);
  xTaskCreate(mqtt_task, "MQTT Task", 1024, NULL, 1, NULL);

  vTaskStartScheduler();

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(100));
    cyw43_arch_deinit();
    return 0;
  }
}