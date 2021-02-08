#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
 
#define RED_LED 4
#define GREEN_LED 5

void rood(void *pvParameter){
    
    gpio_pad_select_gpio(RED_LED);
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    while (1)
    {
        /* Blink off (output low) */
        gpio_set_level(RED_LED, 0);
        vTaskDelay(1000 / portTICK_RATE_MS);
        /* Blink on (output high) */
        gpio_set_level(RED_LED, 1);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    
}

void groen(void *pvParameter){
    gpio_pad_select_gpio(GREEN_LED);
    gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);
    while (1)
    {
        /* Blink off (output low) */
        gpio_set_level(GREEN_LED, 0);
        vTaskDelay(200 / portTICK_RATE_MS);
        /* Blink on (output high) */
        gpio_set_level(GREEN_LED, 1);
        vTaskDelay(200 / portTICK_RATE_MS);
    }   
}


void app_main(void)
{
    nvs_flash_init();
    xTaskCreate(&rood, "rood", 512, NULL,5,NULL );
    xTaskCreate(&groen, "groen", 512, NULL,5,NULL );
}
