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

// ================= TASK CORE 0: Baca encoder + tombol =================
void taskCore0(void *pvParameters) {
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  for (;;) {
    int currentStateCLK = digitalRead(CLK);
    int currentStateDT  = digitalRead(DT);

    // Deteksi perubahan pulsa CLK
    if (currentStateCLK != lastStateCLK) {
      if (currentStateDT != currentStateCLK) {
        encoderValue++;   // putar searah jarum jam
      } else {
        encoderValue--;   // putar berlawanan
      }

      // Tampilkan log Core 0 saat terjadi perubahan
      Serial.print("Core 0 - Encoder bergerak = ");
      Serial.println(encoderValue);
    }
    lastStateCLK = currentStateCLK;

    // Deteksi tombol
    if (digitalRead(SW) == LOW) {
      if (!buttonPressed) {  // hanya tampil log saat tombol baru ditekan
        buttonPressed = true;
        Serial.println("Core 0 - Tombol ditekan");
      }
    } else {
      if (buttonPressed) {  // tombol dilepas
        buttonPressed = false;
        Serial.println("Core 0 - Tombol dilepas");
      }
    }

    vTaskDelay(2 / portTICK_PERIOD_MS);  // debounce & jeda baca singkat
  }
}

// ================= TASK CORE 1: Tampilkan hasil secara periodik =================
void taskCore1(void *pvParameters) {
  for (;;) {
    Serial.print("Core 1 - Posisi Encoder = ");
    Serial.print(encoderValue);
    Serial.print("\tTombol = ");
    Serial.println(buttonPressed ? "Ditekan" : "Lepas");

    vTaskDelay(200 / portTICK_PERIOD_MS);  // tampilkan tiap 200 ms
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Program Encoder Dual-Core ===");

  // Task Core 0 (baca encoder + tombol)
  xTaskCreatePinnedToCore(
    taskCore0, "TaskCore0", 2048, NULL, 1, &TaskCore0, 0
  );

  // Task Core 1 (tampilkan hasil)
  xTaskCreatePinnedToCore(
    taskCore1, "TaskCore1", 2048, NULL, 1, &TaskCore1, 1
  );
}

void loop() {
  // Kosong, semua kerja di task FreeRTOS
}
