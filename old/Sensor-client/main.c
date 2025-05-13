#include <stdio.h>

#include "FreeRTOS.h"
#include "hardware/adc.h"
#include "lwip/tcp.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "task.h"

#ifndef WIFI_SSID
#define WIFI_SSID "default_ssid"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "default_password"
#endif

#ifndef SERVER_IP
#define SERVER_IP "192.168.1.1"
#endif

#ifndef SERVER_PORT
#define SERVER_PORT 8080
#endif

static struct tcp_pcb *client_pcb;
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);

void read_sensors(void *pvParameters);
void maintain_wifi();
void send_data_task(void *pvParameters);

volatile bool wifi_connected = false;

uint16_t read_photoresistor() {
  adc_select_input(0);
  return adc_read();
}

uint16_t read_hygrometer() {
  adc_select_input(1);
  return adc_read();
}

// Function to close connection
static void close_connection() {
  if (client_pcb) {
    tcp_arg(client_pcb, NULL);
    tcp_sent(client_pcb, NULL);
    tcp_recv(client_pcb, NULL);
    tcp_poll(client_pcb, NULL, 0);
    tcp_err(client_pcb, NULL);
    tcp_close(client_pcb);
    client_pcb = NULL;
    printf("Connection closed.\n");
  }
}

// Handle error
static void tcp_client_err(void *arg, err_t err) {
  printf("TCP error occurred: %d\n", err);
  close_connection();
}

// Handle recv
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
                             err_t err) {
  if (!p) {  // If P = Null server has closed the connection
    printf("Server closed the connection.\n");
    close_connection();
    return ERR_OK;
  }
  printf("Received data: %.*s\n", p->len, (char *)p->payload);
  pbuf_free(p);  // Free memory from data
  return ERR_OK;
}

// Connect to server
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
  if (err != ERR_OK) {
    printf("Connection failed: %d\n", err);
    close_connection();
    return err;
  }
  printf("Connected to server!\n");
  client_pcb = tpcb;
  printf("Connection established to %s:%d\n", SERVER_IP, SERVER_PORT);
  tcp_recv(tpcb, tcp_client_recv);
  tcp_poll(tpcb, tcp_client_poll, 2);
  return ERR_OK;
}

// Poll function
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb) {
  if (!client_pcb) {
    printf("No active TCP connection, skipping poll.\n");
    return ERR_OK;
  }
  uint16_t light = read_photoresistor();
  uint16_t moisture = read_hygrometer();
  char msg[64];
  snprintf(msg, sizeof(msg), "Light level: %u, Moisture: %u\n", light,
           moisture);
  err_t err = tcp_write(tpcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
  if (err == ERR_OK) {
    tcp_output(tpcb);
    printf("Sent: %s", msg);
  } else {
    printf("Failed fo send data: %d\n", err);
    close_connection();
  }

  return ERR_OK;
}

void maintain_wifi() {
  while (1) {
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
    break;
  }
}

void read_sensors(void *pvParameters) {
  while (1) {
    uint16_t light = read_photoresistor();
    uint16_t moisture = read_hygrometer();
    printf("Light: %u, Moisture: %u\n", light, moisture);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void send_data_task(void *pvParameters) {
  maintain_wifi();
  while (1) {
    if (wifi_connected) {
      if (!client_pcb) {
        printf("Reconnecting to server...\n");
        client_pcb = tcp_new();
        if (!client_pcb) {
          printf("Failed to create new TCP PCB\n");
          vTaskDelay(pdMS_TO_TICKS(5000));
          continue;
        }
        ip_addr_t server_addr;
        ip4addr_aton(SERVER_IP, &server_addr);
        err_t err = tcp_connect(client_pcb, &server_addr, SERVER_PORT,
                                tcp_client_connected);
        if (err != ERR_OK) {
          printf("TCP connect error: %d\n", err);
          close_connection();
        }
      }
      vTaskDelay(pdMS_TO_TICKS(10000));
    } else {
      printf("WiFi not connected, waiting to retry TCP connection...\n");
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}

int main() {
  stdio_init_all();

  // Initialize wifi
  if (cyw43_arch_init()) {
    printf("WiFi initialization failed!\n");
    return 1;
  }
  cyw43_arch_enable_sta_mode();

  // Initalize ADC
  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);

  // Connect to server
  xTaskCreate(read_sensors, "Sensor Read Task", 512, NULL, 1, NULL);
  xTaskCreate(send_data_task, "TCP Client Task", 512, NULL, 1, NULL);

  vTaskStartScheduler();

  while (1);
  cyw43_arch_deinit();
  return 0;
}