#include <Arduino.h>

#define BUZZER_PIN 14  

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

// Task di Core 0
void taskCore0(void *pvParameters) {
  while (true) {
    tone(BUZZER_PIN, 1000);   // 1 kHz
    Serial.println("Core 0: Bunyi 1 kHz");
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);
  }
}

// Task di Core 1
void taskCore1(void *pvParameters) {
  while (true) {
    tone(BUZZER_PIN, 2000);   // 2 kHz
    Serial.println("Core 1: Bunyi 2 kHz");
    delay(300);
    noTone(BUZZER_PIN);
    delay(300);
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  // Jalankan task di core 0
  xTaskCreatePinnedToCore(
    taskCore0,      // fungsi task
    "TaskCore0",    // nama task
    2048,           // stack size
    NULL,           // parameter
    1,              // prioritas
    &TaskCore0,     // handle
    0               // core 0
  );

  // Jalankan task di core 1
  xTaskCreatePinnedToCore(
    taskCore1,
    "TaskCore1",
    2048,
    NULL,
    1,
    &TaskCore1,
    1               // core 1
  );
}

// Loop utama kosong
void loop() {
  // Tidak digunakan, semua tugas di-handle oleh dua core
}
