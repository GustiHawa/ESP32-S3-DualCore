#include <Arduino.h>

// Pin encoder
#define CLK 18
#define DT 19
#define SW 20

TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

volatile long encoderValue = 0;
volatile bool buttonPressed = false;
int lastStateCLK = HIGH;

// Core 0 : Baca encoder + tombol
void taskCore0(void *pvParameters) {
  while (true) {
    int currentStateCLK = digitalRead(CLK);
    int currentStateDT  = digitalRead(DT);

    // Deteksi perubahan pulsa CLK
    if (currentStateCLK != lastStateCLK) {
      if (currentStateDT != currentStateCLK) {
        encoderValue++;   // putar searah jarum jam
      } else {
        encoderValue--;   // putar berlawanan
      }
    }
    lastStateCLK = currentStateCLK;

    // Deteksi tombol
    if (digitalRead(SW) == LOW) {
      buttonPressed = true;
    } else {
      buttonPressed = false;
    }

    delay(2);  // debounce & jeda baca singkat
  }
}

// Core 1 : Tampilkan ke Serial
void taskCore1(void *pvParameters) {
  while (true) {
    Serial.print("Core 1 - Posisi Encoder = ");
    Serial.print(encoderValue);
    Serial.print("\tTombol = ");
    Serial.println(buttonPressed ? "Ditekan" : "Lepas");
    delay(200);
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Jalankan task Core 0 (baca encoder)
  xTaskCreatePinnedToCore(
    taskCore0, "TaskCore0", 2048, NULL, 1, &TaskCore0, 0
  );

  // Jalankan task Core 1 (tampilkan hasil)
  xTaskCreatePinnedToCore(
    taskCore1, "TaskCore1", 2048, NULL, 1, &TaskCore1, 1
  );
}

void loop() {
  // semua pekerjaan ada di task
}
