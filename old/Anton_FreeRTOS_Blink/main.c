#include "FreeRTOS.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "task.h"

#define RED_LED 10
#define GREEN_LED 11

void vBlinkWifiLED(void *pvParameters) {
  (void)pvParameters;

  while(1) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(250));
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(250));
  }
}
void vBlinkRedLED(void *pvParameters) {
  (void)pvParameters;
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, GPIO_OUT);

  while(1) {
    gpio_put(RED_LED, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_put(RED_LED, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
void vBlinkGreenLED(void *pvParameters) {
  (void)pvParameters;
  gpio_init(GREEN_LED);
  gpio_set_dir(GREEN_LED, GPIO_OUT);

   while(1){
    gpio_put(GREEN_LED, 1);
    vTaskDelay(pdMS_TO_TICKS(250));
    gpio_put(GREEN_LED, 0);
    vTaskDelay(pdMS_TO_TICKS(250));
  }
}

int main() {
  stdio_init_all();

  if (cyw43_arch_init()) {
    printf("WiFi init failed!\n");
    return -1;
  }

  xTaskCreate(vBlinkWifiLED, "Blink Wifi Task", 128, NULL, 1, NULL);
  xTaskCreate(vBlinkGreenLED, "Blink Green Task", 128, NULL, 1, NULL);
  xTaskCreate(vBlinkRedLED, "Blink Red Task", 128, NULL, 1, NULL);

  vTaskStartScheduler();
  while (1);

  return 0;
}