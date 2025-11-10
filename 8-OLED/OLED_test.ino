#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin OLED
#define SDA_PIN 8
#define SCL_PIN 9

// Ukuran layar OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Objek display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variabel global dibagikan antar-core
volatile unsigned long counter = 0;

// Task handle
TaskHandle_t TaskCore0;
TaskHandle_t TaskCore1;

// Mutex untuk akses I2C
SemaphoreHandle_t i2cMutex;

// ================= TASK CORE 0: Hitung counter =================
void taskCore0(void *pvParameters) {
  String coreID = String(xPortGetCoreID());
  for (;;) {
    counter++;   // naik terus setiap 100 ms
    Serial.print("Core ");
    Serial.print(coreID);
    Serial.println(": Counter bertambah");

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// ================= TASK CORE 1: Tampilkan ke OLED =================
void taskCore1(void *pvParameters) {
  String coreID = String(xPortGetCoreID());
  for (;;) {
    // Ambil mutex sebelum akses I2C
    if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);

      display.setCursor(0, 0);
      display.println("ESP32-S3 Dual Core");

      display.setCursor(0, 20);
      display.print("Counter: ");
      display.println(counter);

      display.setCursor(0, 40);
      display.print("OLED Task berjalan di Core: ");
      display.println(coreID);

      display.display();

      xSemaphoreGive(i2cMutex);
    }

    Serial.print("Core ");
    Serial.print(coreID);
    Serial.println(": OLED diperbarui");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== Program OLED Dual-Core ===");

  // Inisialisasi I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Mutex untuk akses I2C
  i2cMutex = xSemaphoreCreateMutex();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Gagal inisialisasi OLED!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Inisialisasi...");
  display.display();
  delay(1000);

  // Jalankan task Core 0 (counter)
  xTaskCreatePinnedToCore(
    taskCore0,
    "TaskCore0",
    2048,
    NULL,
    1,
    &TaskCore0,
    0  // Core 0
  );

  // Jalankan task Core 1 (tampilkan OLED)
  xTaskCreatePinnedToCore(
    taskCore1,
    "TaskCore1",
    4096,
    NULL,
    1,
    &TaskCore1,
    1  // Core 1
  );
}

void loop() {
  // semua kerja di task
}
