#define LED1 5   // LED kuning
#define LED2 4   // LED biru
#define LED3 2   // LED cyan (indikator)
#define BTN1 12  // Tombol hijau
#define BTN2 13  // Tombol merah

TaskHandle_t TaskButton1;
TaskHandle_t TaskButton2;

// Task 1: Tombol 1 di Core 0
void TaskTombol1(void *pvParameters) {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);

  Serial.printf("Task Tombol1 berjalan di core %d\n", xPortGetCoreID());

  while (true) {
    if (digitalRead(BTN1) == LOW) {
      digitalWrite(LED1, HIGH);
      Serial.println("Tombol 1 ditekan -> LED1 ON");
    } else {
      digitalWrite(LED1, LOW);
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// Task 2: Tombol 2 di Core 1
void TaskTombol2(void *pvParameters) {
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(LED2, OUTPUT);

  Serial.printf("Task Tombol2 berjalan di core %d\n", xPortGetCoreID());

  while (true) {
    if (digitalRead(BTN2) == LOW) {
      digitalWrite(LED2, HIGH);
      Serial.println("Tombol 2 ditekan -> LED2 ON");
    } else {
      digitalWrite(LED2, LOW);
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// Loop utama hanya untuk LED3
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Program Tombol Multi-Core dimulai ===");

  pinMode(LED3, OUTPUT);

  // Task tombol 1 (core 0)
  xTaskCreatePinnedToCore(
    TaskTombol1,
    "Tombol1",
    2000,
    NULL,
    1,
    &TaskButton1,
    0  // core 0
  );

  // Task tombol 2 (core 1)
  xTaskCreatePinnedToCore(
    TaskTombol2,
    "Tombol2",
    2000,
    NULL,
    1,
    &TaskButton2,
    1  // core 1
  );
}

void loop() {
  // LED3 berkedip untuk menunjukkan bahwa loop utama tetap hidup
  digitalWrite(LED3, HIGH);
  delay(500);
  digitalWrite(LED3, LOW);
  delay(500);
}
