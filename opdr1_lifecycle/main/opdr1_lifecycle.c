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

TaskHandle_t taskHandleLifeCycle = NULL;

void taskLifeCycle (void * p) {


    while (uxTaskPriorityGet(taskHandleLifeCycle) > 1) {
        printf("Begin! \r\n");
        printf("weer 3 seconden! : %d\r\n", uxTaskPriorityGet(taskHandleLifeCycle));
        vTaskDelay(3000/portTICK_PERIOD_MS);
        printf("weer 3 seconden! : %d\r\n", uxTaskPriorityGet(taskHandleLifeCycle));
        vTaskDelay(3000/portTICK_PERIOD_MS);
        int taskPrio =  uxTaskPriorityGet(taskHandleLifeCycle);
        taskPrio = taskPrio - 1;
        vTaskPrioritySet(taskHandleLifeCycle, taskPrio);
        printf("Eind! \r\n");
    }

    vTaskDelay(5000/portTICK_PERIOD_MS);

    printf("Klaar!!!!! \r\n");
     vTaskDelete(taskHandleLifeCycle);
     printf("Helemaal klaar!!!!!! \r\n");
}

void app_main(void)
{
    xTaskCreate(taskLifeCycle, "taskLifeCycle", 10000, (void*) 0, 5, &taskHandleLifeCycle);
    //vTaskStartScheduler();
}