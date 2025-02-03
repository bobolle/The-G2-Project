#include <stdio.h>

#include "hardware/adc.h"
#include "lwip/tcp.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

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

uint16_t read_photoresistor() {
  adc_select_input(0);
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

  // Print data
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

  // Save PCV
  client_pcb = tpcb;

  printf("Connection established to %s:%d\n", SERVER_IP, SERVER_PORT);

  // Set up callbacks
  tcp_recv(tpcb, tcp_client_recv);
  tcp_poll(tpcb, tcp_client_poll, 2);

  return ERR_OK;
}

// Poll function

static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb) {
  if (!client_pcb) return ERR_OK;

  uint16_t adc_value = read_photoresistor();
  char msg[32];
  snprintf(msg, sizeof(msg), "Light level: %u\n", adc_value);

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

// Initialize connection
void tcp_client_connect() {
  client_pcb = tcp_new();
  ip_addr_t server_addr;
  ip4addr_aton(SERVER_IP, &server_addr);
  tcp_connect(client_pcb, &server_addr, SERVER_PORT, tcp_client_connected);
}

int main() {
  stdio_init_all();

  // Initialize wifi
  if (cyw43_arch_init()) {
    printf("WiFi initialization failed!\n");
    return 1;
  }
  cyw43_arch_enable_sta_mode();

  // Connect to wifi
  int max_retries = 5;
  int attempt = 0;

  printf("Connecting to WiFi...\n");
  printf("Trying to connect to WiFi: SSID=[%s], PASSWORD=[%s]\n", WIFI_SSID, WIFI_PASS);
  while (attempt < max_retries) {
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000) == 0) {
      printf("Connected to WiFi!\n");
      break;
    }

    attempt++;
    printf("Connection to WiFi Failed (%d/%d)...\n", attempt,max_retries);
    printf("Trying to connect to WiFi: SSID=[%s], PASSWORD=[%s]\n", WIFI_SSID, WIFI_PASS);
    sleep_ms(5000);
  }
  if (attempt == max_retries) {
    printf("Failed to connect to WiFi after %d attempts. Resart and try again.\n", max_retries);
    cyw43_arch_deinit();
    return 1;
  }

  // Initalize ADC
  adc_init();
  adc_gpio_init(26);

  // Connect to server
  printf("Connecting to server at: %s:%d\n", SERVER_IP, SERVER_PORT);
  tcp_client_connect();

  while (1) {
    // Keep program alive and use lwIP-poll to send data
    sleep_ms(5000);
  }

  cyw43_arch_deinit();
  return 0;
}