#include <Arduino_FreeRTOS.h>
#include "queue.h"
#define Red 8
#define SwRed 2
#define Gr 9
#define SwGr 3
#define Yl 10
#define SwYl 4
QueueHandle_t redQ, grQ, ylQ;
void setup()
{
    Serial.begin(9600);
    redQ = xQueueCreate(10, sizeof(bool));
    grQ = xQueueCreate(1, sizeof(bool));
    ylQ = xQueueCreate(1, sizeof(bool));
    xTaskCreate(SendBtRed, "Sender Task Red ", 100, NULL, 1, NULL);
    xTaskCreate(ReceiveRed, "Receiver Task Red", 100, NULL, 1, NULL);
    xTaskCreate(SendBtGr, "Sender Task Gr ", 100, NULL, 1, NULL);
    xTaskCreate(ReceiveGr, "Receiver Task Gr", 100, NULL, 1, NULL);
    xTaskCreate(SendBtYl, "Sender Task Yl ", 100, NULL, 1, NULL);
    xTaskCreate(ReceiveYl, "Receiver Task Yl", 100, NULL, 1, NULL);
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
void SendBtGr(void *pvParameters)
{
    pinMode(SwGr, INPUT_PULLUP);
    bool lastState = false;
    while (1)
    {

        bool state = !digitalRead(SwGr);
        if (!lastState && state)
        {
            xQueueSend(grQ, NULL, 0);
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
        if (!lastState && state)
        {
            xQueueSend(ylQ, NULL, 0);
        }
        lastState = state;
    }
}
void ReceiveRed(void *pvParameters)
{
    pinMode(Red, OUTPUT);
    while (1)
    {
        if (xQueueReceive(redQ, NULL, 0))
        {
            digitalWrite(Red, HIGH);
            vTaskDelay(pdMS_TO_TICKS(3000));
        }
        else
        {
            digitalWrite(Red, LOW);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void ReceiveGr(void *pvParameters)
{
    pinMode(Gr, OUTPUT);
    bool blink = false;
    while (1)
    {
        if (xQueueReceive(grQ, NULL, 0))
        {
            blink = !blink;
        }
        if (blink)
        {
            digitalWrite(Gr, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(Gr, LOW);
            vTaskDelay(pdMS_TO_TICKS(400));
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void ReceiveYl(void *pvParameters)
{
    pinMode(Yl, OUTPUT);
    while (1)
    {
        if (xQueueReceive(ylQ, NULL, 0))
        {
            xQueueSend(ylQ, NULL, 0);
            digitalWrite(Yl, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(Yl, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(Yl, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(Yl, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(Yl, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(Yl, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
            xQueueReceive(ylQ, NULL, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void loop() {}
