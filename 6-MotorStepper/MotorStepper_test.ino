#include <Arduino.h>

#define STEP_PIN 4
#define DIR_PIN 5
#define ENABLE_PIN 2

const int stepsPerRevolution = 200;
const int stepDelay = 2500; // mikrodetik (2.5ms) → cukup lambat agar animasi Wokwi terlihat

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

// ====== TASK CORE 0 (PUTAR CW) ======
void Task1(void *pvParameters) {
  for (;;) {
    digitalWrite(ENABLE_PIN, LOW);   // aktifkan driver
    digitalWrite(DIR_PIN, HIGH);     // arah CW

    Serial.print("Core ");
    Serial.print(xPortGetCoreID());
    Serial.println(": Putar CW");

    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(stepDelay);
    }

    delay(1000); // jeda 1 detik sebelum arah berikutnya
  }
}

// ====== TASK CORE 1 (PUTAR CCW) ======
void Task2(void *pvParameters) {
  for (;;) {
    digitalWrite(ENABLE_PIN, LOW);   // aktifkan driver
    digitalWrite(DIR_PIN, LOW);      // arah CCW

    Serial.print("Core ");
    Serial.print(xPortGetCoreID());
    Serial.println(": Putar CCW");

    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(stepDelay);
    }

    delay(1000); // jeda sebelum putar berikutnya
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW); // selalu aktif supaya gerak terlihat

  Serial.println("=== Stepper ESP32-S3 Wokwi ===");
  Serial.println("Busur biru akan berputar CW lalu CCW bergantian");

  // Jalankan task di dua core
  xTaskCreatePinnedToCore(Task1, "TaskCore0", 10000, NULL, 1, &TaskCore0, 0);
  delay(500);
  xTaskCreatePinnedToCore(Task2, "TaskCore1", 10000, NULL, 1, &TaskCore1, 1);
}

void loop() {
  // kosong karena dijalankan oleh task
}
