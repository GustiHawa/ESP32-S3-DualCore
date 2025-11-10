#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED1 2   // Core 0
#define LED2 4   // Core 1
#define LED3 5   // Core 1

// ================= TASK LED1 (CORE 0) =================
void Task_LED_Core0(void *parameter) {
  pinMode(LED1, OUTPUT);
  for(;;) {
    digitalWrite(LED1, HIGH);
    Serial.println("LED1 ON | Core 0");
    vTaskDelay(300 / portTICK_PERIOD_MS);

    digitalWrite(LED1, LOW);
    Serial.println("LED1 OFF | Core 0");
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// ================= TASK LED2 (CORE 1) =================
void Task_LED2_Core1(void *parameter) {
  pinMode(LED2, OUTPUT);
  for(;;) {
    digitalWrite(LED2, HIGH);
    Serial.println("LED2 ON | Core 1");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    digitalWrite(LED2, LOW);
    Serial.println("LED2 OFF | Core 1");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ================= TASK LED3 (CORE 1) =================
void Task_LED3_Core1(void *parameter) {
  pinMode(LED3, OUTPUT);
  for(;;) {
    digitalWrite(LED3, HIGH);
    Serial.println("LED3 ON | Core 1");
    vTaskDelay(700 / portTICK_PERIOD_MS);  // interval berbeda dari LED2

    digitalWrite(LED3, LOW);
    Serial.println("LED3 OFF | Core 1");
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Mulai program LED multi-core...");

  // Buat Task LED1 di Core 0
  xTaskCreatePinnedToCore(Task_LED_Core0, "LED_Core0", 2048, NULL, 1, NULL, 0);  

  // Buat Task LED2 di Core 1
  xTaskCreatePinnedToCore(Task_LED2_Core1, "LED2_Core1", 2048, NULL, 1, NULL, 1);  

  // Buat Task LED3 di Core 1
  xTaskCreatePinnedToCore(Task_LED3_Core1, "LED3_Core1", 2048, NULL, 1, NULL, 1);  
}

void loop() {
  // Kosong, semua dijalankan oleh FreeRTOS
}
