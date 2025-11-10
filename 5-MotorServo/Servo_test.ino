#include <Arduino.h>
#include <ESP32Servo.h>  // Gunakan library ESP32Servo, bukan Servo biasa

#define SERVO_PIN 17

Servo myServo;

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

volatile int servoPos = 0;   // posisi servo bersama antar-core
volatile bool arahNaik = true;

// Core 0 : Gerakkan Servo
void taskCore0(void *pvParameters) {
  while (true) {
    if (arahNaik) {
      servoPos += 5;
      if (servoPos >= 180) arahNaik = false;
    } else {
      servoPos -= 5;
      if (servoPos <= 0) arahNaik = true;
    }

    myServo.write(servoPos);
    delay(100);  // kecepatan gerak servo
  }
}

// Core 1 : Tampilkan Posisi
void taskCore1(void *pvParameters) {
  while (true) {
    Serial.print("Core 1: Posisi Servo = ");
    Serial.println(servoPos);
    delay(300);
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inisialisasi servo
  myServo.attach(SERVO_PIN);
  myServo.write(0);

  // Jalankan task untuk Core 0 (gerak servo)
  xTaskCreatePinnedToCore(
    taskCore0,
    "TaskCore0",
    2048,
    NULL,
    1,
    &TaskCore0,
    0  // core 0
  );

  // Jalankan task untuk Core 1 (print posisi)
  xTaskCreatePinnedToCore(
    taskCore1,
    "TaskCore1",
    2048,
    NULL,
    1,
    &TaskCore1,
    1  // core 1
  );
}

void loop() {
  // semua kerja di task
}
