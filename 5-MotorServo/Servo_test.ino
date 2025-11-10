#include <Arduino.h>
#include <ESP32Servo.h> 

#define SERVO_PIN 45

Servo myServo;

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

volatile int servoPos = 0;   // posisi servo bersama antar-core
volatile bool arahNaik = true;

// ================= TASK CORE 0: Gerakkan Servo =================
void taskCore0(void *pvParameters) {
  myServo.attach(SERVO_PIN); // attach di dalam task
  myServo.write(0);
  
  for (;;) {
    if (arahNaik) {
      servoPos += 5;
      if (servoPos >= 180) arahNaik = false;
    } else {
      servoPos -= 5;
      if (servoPos <= 0) arahNaik = true;
    }

    myServo.write(servoPos);  // update posisi servo

    // log Core 0 dibatasi agar tidak terlalu sering
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 200) {
      Serial.print("Core 0: Servo bergerak ke posisi = ");
      Serial.println(servoPos);
      lastPrint = millis();
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);  // kecepatan gerak servo
  }
}

// ================= TASK CORE 1: Tampilkan Posisi =================
void taskCore1(void *pvParameters) {
  for (;;) {
    Serial.print("Core 1: Posisi Servo terakhir = ");
    Serial.println(servoPos);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Program Servo Dual-Core ===");

  // Task Core 0: gerakkan servo
  xTaskCreatePinnedToCore(
    taskCore0,
    "TaskCore0",
    4096, // tingkatkan stack size untuk servo
    NULL,
    1,
    &TaskCore0,
    0
  );

  // Task Core 1: tampilkan posisi
  xTaskCreatePinnedToCore(
    taskCore1,
    "TaskCore1",
    2048,
    NULL,
    1,
    &TaskCore1,
    1
  );
}

void loop() {
  // Kosong, semua kerja di task FreeRTOS
}
