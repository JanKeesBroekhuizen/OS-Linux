#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "esp_peripherals.h"

#define PhilosopherAmount 5
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (philosopherNumber + 4) % PhilosopherAmount
#define RIGHT (philosopherNumber + 1) % PhilosopherAmount

typedef struct {
    int number;
    int state;
    int pin;
} Philosopher;

Philosopher philosophers[] = {
    {0, THINKING, 12},
    {1, THINKING, 14},
    {2, THINKING, 27},
    {3, THINKING, 26},
    {4, THINKING, 25}
};

SemaphoreHandle_t xMutex;

void checkFork(int philosopherNumber){
    if (philosophers[philosopherNumber].state == HUNGRY && philosophers[LEFT].state != EATING && philosophers[RIGHT].state != EATING)
    {
        philosophers[philosopherNumber].state = EATING;
        vTaskDelay(2000 / portTICK_RATE_MS);
        gpio_set_level(philosophers[philosopherNumber].pin, 1);
        xSemaphoreGive(xMutex);
    } else {
        gpio_set_level(philosophers[philosopherNumber].pin, 0);
    }
}

void take_fork(int philosopherNumber){
    xSemaphoreTake(xMutex, portMAX_DELAY);
    philosophers[philosopherNumber].state = HUNGRY;
    checkFork(philosopherNumber);
    xSemaphoreGive(xMutex);
    vTaskDelay(1000 / portTICK_RATE_MS);
}

void put_fork(int philosopherNumber){
    xSemaphoreTake(xMutex, portMAX_DELAY);
    philosophers[philosopherNumber].state = THINKING;
    checkFork(LEFT);
    checkFork(RIGHT);
    xSemaphoreGive(xMutex);
}

void philosopher(void* philosopherNumber){
    while (1){
        int * number = philosopherNumber;
        vTaskDelay(1000 / portTICK_RATE_MS);
        take_fork(*number);
        vTaskDelay(100 / portTICK_RATE_MS);
        put_fork(*number);
    }
    vTaskDelete(NULL);
}

void initLEDs(){
    for (int i = 0; i < PhilosopherAmount; i++){
        gpio_pad_select_gpio(philosophers[i].pin);
        gpio_set_direction(philosophers[i].pin, GPIO_MODE_OUTPUT);
    }
}

void app_main(void){
    nvs_flash_init();

    initLEDs();
    for (int i = 0; i < PhilosopherAmount; i++){
        gpio_set_level(philosophers[i].pin, 1);
        vTaskDelay(2000 / portTICK_RATE_MS);
        gpio_set_level(philosophers[i].pin, 0);
    }
    
    int currentPhilosophersNumber;

    xMutex = xSemaphoreCreateMutex();
    if (xMutex != NULL){
        for (int i = 0; i < PhilosopherAmount; i++){
            currentPhilosophersNumber = i;
            char buffer[30];
            sprintf(buffer, "Philosopher%d", i);
            xTaskCreate(philosopher, buffer, 1024, (void *) currentPhilosophersNumber, 5, NULL);
        }
    }
    
    while (1){
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}