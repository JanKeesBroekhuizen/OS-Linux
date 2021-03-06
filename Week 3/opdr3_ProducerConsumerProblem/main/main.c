#include <stdio.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define A 100
#define N 10

int randomArray[A];
int bufferArray[N];

TaskHandle_t producerTaskHandleLifeCycle = NULL;
TaskHandle_t consumerHandleLifeCycle = NULL;
SemaphoreHandle_t emptyMutex;
SemaphoreHandle_t mutex;
SemaphoreHandle_t fullMutex;

void produceRandomIntToArray();
void consumePrint(int toBeConsumed);
int takeFromBuffer();
void putInBuffer(int valueToPutIn);

void producerProduce(){

        //Produce a new Random integer and put it in the random Array...
        produceRandomIntToArray();

        xSemaphoreTake(emptyMutex,0);
        xSemaphoreTake(mutex,0);
        
        putInBuffer(randomArray[0]);

        xSemaphoreGive(mutex);
        xSemaphoreGive(fullMutex);        

}

void consumerConsume(){

        xSemaphoreTake(fullMutex,0);
        xSemaphoreTake(mutex,0);

        int takenValue = takeFromBuffer();

        xSemaphoreGive(mutex);
        xSemaphoreGive(emptyMutex);

        consumePrint(takenValue);

}

void putInBuffer(int valueToPutIn){
        size_t n = sizeof(bufferArray) / sizeof(int);

        if (n < N)
        {
                bufferArray[n-1] = valueToPutIn;
        }
}

int takeFromBuffer(){
        size_t n = sizeof(bufferArray) / sizeof(int);
        int returnValue = bufferArray[0];

        for(int i=0; i<n-1; i++)
        {
            bufferArray[i] = bufferArray[i + 1];
            bufferArray[n-1] = 0;
        }

        return returnValue;
}

void consumePrint(int toBeConsumed){
        printf("The current consumed int is: %d", toBeConsumed);
}

void produceRandomIntToArray(){
        size_t n = sizeof(randomArray) / sizeof(int);
        if (n + (sizeof(int) * 1) < A)
        {       
                int randomNewInt = rand() % (A * 10) + 1; 
                randomArray[n+1] = randomNewInt;
                printf("New integer produced: %d", randomNewInt);
        }
}

void fillArrayWithRandoms(){
        for (int i = 0; i < A; i++)
        {
              randomArray[i] = rand() % (A * 10) + 1;
        }       
}

void app_main()
{
        fillArrayWithRandoms();

        xTaskCreate(producerProduce, "producerLifeCycle", 10000, (void*) 0, 5, &producerTaskHandleLifeCycle);
        xTaskCreate(consumerConsume, "consumerLifeCycle", 10000, (void*) 0, 5, &consumerHandleLifeCycle);

        vTaskStartScheduler();
        
}
