/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

TaskHandle_t taskHandleLifeCycle1 = NULL;
TaskHandle_t taskHandleLifeCycle2 = NULL;
TaskHandle_t taskHandleLifeCycle3 = NULL;
TaskHandle_t taskHandleLifeCycle4 = NULL;

void taskLifeCycle1 (void * p) {
    while (1) {
        printf("Task is running! Task name is : taskLifeCycle1 \r\n");
    }
    printf("Klaar!");
}

void taskLifeCycle2 (void * p) {
    while (1) {
        printf("Task is running! Task name is : taskLifeCycle2 \r\n");
    }
    printf("Klaar!");
}

void taskLifeCycle3 (void * p) {
    while (1) {
        printf("Task is running! Task name is : taskLifeCycle3 \r\n");
    }
    printf("Klaar!");
}

void taskLifeCycle4 (void * p) {
    while (1) {
        printf("Task is running! Task name is : taskLifeCycle4 \r\n");
    }
    printf("Klaar!");
}

void app_main(void)
{
    xTaskCreate(taskLifeCycle1, "taskLifeCycle1", 10000, (void*) 0, 5, &taskHandleLifeCycle1);
    xTaskCreate(taskLifeCycle2, "taskLifeCycle1", 10000, (void*) 0, 5, &taskHandleLifeCycle2);
    xTaskCreate(taskLifeCycle3, "taskLifeCycle1", 10000, (void*) 0, 1, &taskHandleLifeCycle3);
    xTaskCreate(taskLifeCycle4, "taskLifeCycle1", 10000, (void*) 0, 1, &taskHandleLifeCycle4);
    //vTaskStartScheduler();
}