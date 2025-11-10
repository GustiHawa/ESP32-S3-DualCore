#include <Arduino.h>

#define POT_PIN 15  // sesuai wiring

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

volatile int potValue = 0;  // variabel global berbagi antar-core

// Core 0 : Baca Potensiometer
void taskCore0(void *pvParameters) {
  while (true) {
    int val = analogRead(POT_PIN);  // baca nilai analog 0–4095
    potValue = val;                 // kirim ke variabel global
    delay(100);                     // sampling setiap 100 ms
  }
}

// Core 1 : Tampilkan ke Serial
void taskCore1(void *pvParameters) {
  while (true) {
    Serial.print("Core 1: Nilai pot = ");
    Serial.println(potValue);
    delay(300);  // tampilkan tiap 300 ms
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(POT_PIN, INPUT);

  // Jalankan task pembaca di core 0
  xTaskCreatePinnedToCore(
    taskCore0,
    "TaskCore0",
    2048,
    NULL,
    1,
    &TaskCore0,
    0  // Core 0
  );

  // Jalankan task penampil di core 1
  xTaskCreatePinnedToCore(
    taskCore1,
    "TaskCore1",
    2048,
    NULL,
    1,
    &TaskCore1,
    1  // Core 1
  );
}

// Loop kosong — semua kerja di task
void loop() {
}
