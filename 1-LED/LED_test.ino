#define LED_MERAH 2
#define LED_HIJAU 4
#define LED_BIRU 5

// Task handles (opsional)
TaskHandle_t TaskLED_Merah;
TaskHandle_t TaskLED_Hijau;
TaskHandle_t TaskLED_Biru;

// ==========================
// Task untuk LED Merah (core 0)
// ==========================
void TaskMerah(void *pvParameters) {
  pinMode(LED_MERAH, OUTPUT);
  while (true) {
    digitalWrite(LED_MERAH, HIGH);
    delay(500);
    digitalWrite(LED_MERAH, LOW);
    delay(500);
  }
}

// ==========================
// Task untuk LED Hijau (core 1)
// ==========================
void TaskHijau(void *pvParameters) {
  pinMode(LED_HIJAU, OUTPUT);
  while (true) {
    digitalWrite(LED_HIJAU, HIGH);
    delay(300);
    digitalWrite(LED_HIJAU, LOW);
    delay(300);
  }
}

// ==========================
// Task untuk LED Biru (core 1)
// ==========================
void TaskBiru(void *pvParameters) {
  pinMode(LED_BIRU, OUTPUT);
  while (true) {
    digitalWrite(LED_BIRU, HIGH);
    delay(700);
    digitalWrite(LED_BIRU, LOW);
    delay(700);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Mulai program LED multi-core...");

  // Buat task untuk LED Merah di core 0
  xTaskCreatePinnedToCore(
    TaskMerah,        // Fungsi task
    "Task Merah",     // Nama task
    1000,             // Stack size
    NULL,             // Parameter
    1,                // Prioritas
    &TaskLED_Merah,   // Handle
    0                 // Jalankan di core 0
  );

  // Buat task untuk LED Hijau di core 1
  xTaskCreatePinnedToCore(
    TaskHijau,
    "Task Hijau",
    1000,
    NULL,
    1,
    &TaskLED_Hijau,
    1                 // Core 1
  );

  // Buat task untuk LED Biru di core 1 juga
  xTaskCreatePinnedToCore(
    TaskBiru,
    "Task Biru",
    1000,
    NULL,
    1,
    &TaskLED_Biru,
    1                 // Core 1 juga
  );
}

void loop() {
  // Kosong, karena semua dijalankan oleh FreeRTOS
}
