#include <Arduino.h>

#define POT_PIN 15  // sesuai wiring

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

volatile int potValue = 0;  // variabel global berbagi antar-core

// ================= TASK CORE 0: Baca Potensiometer =================
void taskCore0(void *pvParameters) {
  for (;;) {
    int val = analogRead(POT_PIN);  // baca nilai analog 0–4095
    potValue = val;                 // simpan ke variabel global

    // Tambahkan log untuk Core 0
    Serial.print("Core 0: Membaca potensiometer = ");
    Serial.println(val);

    vTaskDelay(100 / portTICK_PERIOD_MS);  // sampling tiap 100 ms
  }
}

// ================= TASK CORE 1: Tampilkan ke Serial =================
void taskCore1(void *pvParameters) {
  for (;;) {
    Serial.print("Core 1: Nilai pot terakhir = ");
    Serial.println(potValue);

    vTaskDelay(300 / portTICK_PERIOD_MS);  // tampilkan tiap 300 ms
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Program Dual-Core Potensiometer ===");

  pinMode(POT_PIN, INPUT);

  // Task pembaca (Core 0)
  xTaskCreatePinnedToCore(
    taskCore0,
    "TaskCore0",
    2048,
    NULL,
    1,
    &TaskCore0,
    0  // Core 0
  );

  // Task penampil (Core 1)
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

void loop() {
  // Kosong, semua kerja di task FreeRTOS
}
