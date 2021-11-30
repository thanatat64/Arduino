#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "semphr.h"
#define SwRed 2
#define SwYl 3
#define SwGr 4
#define Red 8
#define Yl 9
#define Gr 10
SemaphoreHandle_t redMutex, grMutex;
QueueHandle_t redQ, grQ, ylQ;
void setup()
{
    Serial.begin(9600);
    redQ = xQueueCreate(1, sizeof(bool)); //ไม่ได้กดซ้ำๆแล้ว ใช้ 1 ก็พอ
    grQ = xQueueCreate(1, sizeof(bool));
    ylQ = xQueueCreate(1, sizeof(bool));
    redMutex = xSemaphoreCreateMutex();
    grMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(redMutex);
    xSemaphoreGive(grMutex);
    xTaskCreate(SendBtRed, "Sender Task Red ", 80, NULL, 1, NULL); // left
    xTaskCreate(SendBtYl, "Sender Task Yl ", 80, NULL, 1, NULL);   // mid
    xTaskCreate(SendBtGr, "Sender Task Gr ", 80, NULL, 1, NULL);   // right

    xTaskCreate(ReceiveRed, "Receiver Task Red", 80, NULL, 1, NULL); // left
    xTaskCreate(ReceiveYl, "Receiver Task Yl", 80, NULL, 1, NULL);   // mid
    xTaskCreate(ReceiveGr, "Receiver Task Gr", 80, NULL, 1, NULL);   // right
}
void SendBtRed(void *pvParameters)
{
    pinMode(SwRed, INPUT_PULLUP);
    bool lastState = false;
    while (1)
    {
        bool state = !digitalRead(SwRed);
        if (!lastState && state)
        {
            xQueueSend(redQ, NULL, 0);
        }
        lastState = state;
    }
}
void SendBtYl(void *pvParameters)
{
    pinMode(SwYl, INPUT_PULLUP);
    bool lastState = false;
    while (1)
    {
        bool state = !digitalRead(SwYl);
        if (xSemaphoreTake(redMutex, 0) == pdTRUE && xSemaphoreTake(grMutex, 0) == pdTRUE)
        {
            xSemaphoreGive(redMutex);
            xSemaphoreGive(grMutex);
            if (!lastState && state)
            {
                xQueueSend(ylQ, NULL, 0);
            }
            lastState = state;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void SendBtGr(void *pvParameters)
{
    pinMode(SwGr, INPUT_PULLUP);
    bool lastState = false;
    while (1)
    {
        bool state = !digitalRead(SwGr);
        if (xSemaphoreTake(redMutex, 0) == pdTRUE)
        {
            xSemaphoreGive(redMutex);
            if (!lastState && state)
            {
                xQueueSend(grQ, NULL, 0);
            }
            lastState = state;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void ReceiveRed(void *pvParameters) // left
{
    pinMode(Red, OUTPUT);
    bool pressSwitch = false;
    long lastState = 0;
    while (1)
    {
        if (xQueueReceive(redQ, NULL, 0) == pdPASS)
        {
            if (!pressSwitch)
            {
                digitalWrite(Red, HIGH);
                pressSwitch = true;
            }
            else
            {
                digitalWrite(Red, LOW);
                pressSwitch = false;
            }
            lastState = millis();
        }
        if (digitalRead(Red))
        {
            xSemaphoreTake(redMutex, 0);
        }
        else
        {
            xSemaphoreGive(redMutex);
        }
        if (pressSwitch)
        {
            if (millis() - lastState >= 3000)
            {
                digitalWrite(Red, LOW);
                pressSwitch = false;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void ReceiveYl(void *pvParameters) // mid
{
    pinMode(Yl, OUTPUT);
    while (1)
    {
        if (xQueueReceive(ylQ, NULL, 0) == pdPASS)
        {
            xQueueSend(ylQ, NULL, 0);
            digitalWrite(Yl, HIGH);
            vTaskDelay(pdMS_TO_TICKS(250));
            digitalWrite(Yl, LOW);
            vTaskDelay(pdMS_TO_TICKS(250));
            digitalWrite(Yl, HIGH);
            vTaskDelay(pdMS_TO_TICKS(250));
            digitalWrite(Yl, LOW);
            vTaskDelay(pdMS_TO_TICKS(250));
            digitalWrite(Yl, HIGH);
            vTaskDelay(pdMS_TO_TICKS(250));
            digitalWrite(Yl, LOW);
            vTaskDelay(pdMS_TO_TICKS(250));
            digitalWrite(Yl, HIGH);
            vTaskDelay(pdMS_TO_TICKS(250));
            digitalWrite(Yl, LOW);
            vTaskDelay(pdMS_TO_TICKS(250));
            xQueueReceive(ylQ, NULL, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void ReceiveGr(void *pvParameters) // right
{
    pinMode(Gr, OUTPUT);
    bool pressSwitch = false;
    long lastState = 0;
    while (1)
    {
        if (xQueueReceive(grQ, NULL, 0) == pdPASS)
        {
            if (!pressSwitch)
            {
                digitalWrite(Gr, HIGH);
                pressSwitch = true;
            }
            else
            {
                digitalWrite(Gr, LOW);
                pressSwitch = false;
            }
            lastState = millis();
        }
        if (pressSwitch)
        {
            if (millis() - lastState >= 3000)
            {
                digitalWrite(Gr, LOW);
                pressSwitch = false;
            }
        }
        if (digitalRead(Gr))
        {
            xSemaphoreTake(grMutex, 0);
        }
        else
        {
            xSemaphoreGive(grMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void loop() {}
