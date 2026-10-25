
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"


#define LED_PIN 4

static const char *TAG = "main"; 

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT); 
    
    while (1) {
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG,"LED is on"); 
        vTaskDelay(pdMS_TO_TICKS(500));

        gpio_set_level(LED_PIN, 0);
        ESP_LOGI(TAG,"LED is off"); 
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    
}