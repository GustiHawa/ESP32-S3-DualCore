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

// Core 0 : Hitung waktu
void taskCore0(void *pvParameters) {
  while (true) {
    counter++;   // naik terus setiap 100 ms
    delay(100);
  }
}

// Core 1 : Tampilkan ke OLED
void taskCore1(void *pvParameters) {
  while (true) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 10);
    display.println("ESP32-S3 Dual Core");

    display.setCursor(0, 30);
    display.print("Counter: ");
    display.println(counter);

    display.display();
    delay(1000);
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inisialisasi I2C dan OLED
  Wire.begin(SDA_PIN, SCL_PIN);
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
    0  // core 0
  );

  // Jalankan task Core 1 (tampilkan OLED)
  xTaskCreatePinnedToCore(
    taskCore1,
    "TaskCore1",
    4096,
    NULL,
    1,
    &TaskCore1,
    1  // core 1
  );
}

void loop() {
  // semua kerja di task
}
