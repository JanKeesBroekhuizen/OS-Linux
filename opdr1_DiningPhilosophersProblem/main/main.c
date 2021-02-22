#include <stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "nvs_flash.h"

#define PhilosopherAmount 5
#define LEFT (phnum + 4) % PhilosopherAmount
#define RIGHT (phnum + 1) % PhilosopherAmount

enum state = {EATING = 0, HUNGRY = 1, THINKING = 2};

struct Philosopher {
    int number;
    enum state;
    int pin;
};

struct philosophers[] = {
    {0, THINKING, 12},
    {1, THINKING, 14},
    {2, THINKING, 27},
    {3, THINKING, 26},
    {4, THINKING, 25}
};

sem_t mutex;
sem_t S[PhilosopherAmount];

void checkFork(int philosopherNumber){
    if (philosophers[philosopherNumber].state == HUNGRY && philosophers[LEFT].state != EATING && philosophers[RIGHT].state != EATING)
    {
        philosophers[philosopherNumber].state = EATING;
        sleep(2);
        gpio_set_level(philosophers[philosopherNumber].pin, 1);
        sem_post(&S[philosophers]);
    } else {
        gpio_set_level(philosophers[philosopherNumber].pin, 0);
    }
}

void take_fork(int philosopherNumber){
    sem_wait(&mutex);
    philosophers[philosopherNumber].state = HUNGRY;
    checkFork(philosopherNumber);
    sem_post(&mutex);
    sem_wait(&S[philosopherNumber]);
    sleep(1);
}

void put_fork(int philosopherNumber){
    sem_wait(&mutex);
    philosophers[philosopherNumber].state = THINKING;
    checkFork(LEFT);
    checkFork(RIGHT);
    sem_post(&mutex);
}

void* philosopher(void* philosopherNumber){
    while (1)
    
        int* i = philosopherNumber;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}

void initLEDs(){
    for (int i = 0; i < PhilosopherAmount; i++)
    {
        gpio_pad_select_gpio(philosophers[i].pin);
        gpio_set_direction(philosophers[i].pin, GPIO_MODE_OUTPUT);
    }
    
    
}

void app_main(void)
{
    initLEDs();

    pthread_t thread_id[PhilosopherAmount];

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < PhilosopherAmount; i++)
    {
       sem_init(&S[i], 0, 0);
    }

    for (int i = 0; i < PhilosopherAmount; i++)
    {
        pthread_create(&thread_id[i], NULL, philosopher, &philosophers[i].number);
    }
    
    for (int i = 0; i < count; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
}


