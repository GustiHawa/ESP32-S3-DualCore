# 🔴🟢🔵 01 — Dual-Core LED Blinking (ESP32-S3)

## 📘 Deskripsi

Percobaan ini menampilkan implementasi **LED multi-core** pada ESP32-S3 menggunakan **FreeRTOS tasks**.  
Tujuan utama percobaan adalah menunjukkan bahwa dua core dapat menjalankan **task independen** secara bersamaan tanpa blocking.  

LED Merah dikendalikan oleh task yang dijalankan di Core 0, sedangkan LED Hijau dan LED Biru dijalankan di Core 1. Masing-masing LED memiliki interval kedip yang berbeda untuk membuktikan eksekusi **task paralel** secara visual.

---

## 🎯 Tujuan

- Memahami **dual-core multitasking** pada ESP32-S3.  
- Membuktikan bahwa task berbeda dapat berjalan **simultan** pada core berbeda.  
- Mengamati kestabilan LED saat task berjalan paralel.

---

## ⚙️ Hardware Mapping

| Komponen  | Pin     | Mode   | Core Eksekusi |
|-----------|---------|--------|---------------|
| LED Merah | GPIO 2  | Output | Core 0        |
| LED Hijau | GPIO 4  | Output | Core 1        |
| LED Biru  | GPIO 5  | Output | Core 1        |

---

## 🧠 Penjelasan Kode

Program ini menggunakan **tiga task FreeRTOS**, yaitu `TaskMerah`, `TaskHijau`, dan `TaskBiru`. Masing-masing task bertanggung jawab mengendalikan satu LED, dan dijalankan di core tertentu menggunakan `xTaskCreatePinnedToCore()`.

### Definisi Pin dan Task Handle

Di awal program, pin untuk setiap LED didefinisikan, dan task handle disiapkan untuk keperluan referensi task. Pin GPIO yang digunakan adalah 2 untuk LED Merah, 4 untuk LED Hijau, dan 5 untuk LED Biru. Task handle seperti `TaskLED_Merah` digunakan untuk menyimpan referensi task, yang dapat dimanfaatkan jika ingin memanipulasi task di runtime.
```cpp
#define LED_MERAH 2
#define LED_HIJAU 4
#define LED_BIRU 5

TaskHandle_t TaskLED_Merah;
TaskHandle_t TaskLED_Hijau;
TaskHandle_t TaskLED_Biru;
```

### Task LED Merah (Core 0)

Task Merah dijalankan di Core 0. Dalam task ini, pin LED Merah diatur sebagai output terlebih dahulu. Task kemudian masuk ke loop tak terbatas, di mana LED Merah menyala selama 500 ms, lalu mati selama 500 ms. Task ini berjalan secara independen di Core 0 sehingga tidak memblokir task lain.
```cpp
void TaskMerah(void *pvParameters) {
  pinMode(LED_MERAH, OUTPUT);
  while (true) {
    digitalWrite(LED_MERAH, HIGH);
    delay(500);
    digitalWrite(LED_MERAH, LOW);
    delay(500);
  }
}
```

### Task LED Hijau (Core 1)

Task Hijau dijalankan di Core 1. LED Hijau dikendalikan dengan interval kedip 300 ms. Task ini juga berjalan dalam loop tak terbatas, menunjukkan bahwa Core 1 dapat menangani task berbeda secara paralel tanpa mempengaruhi Core 0.
```cpp
void TaskHijau(void *pvParameters) {
  pinMode(LED_HIJAU, OUTPUT);
  while (true) {
    digitalWrite(LED_HIJAU, HIGH);
    delay(300);
    digitalWrite(LED_HIJAU, LOW);
    delay(300);
  }
}
```

### Task LED Biru (Core 1)

Task Biru juga dijalankan di Core 1. LED Biru menyala selama 700 ms dan mati selama 700 ms. Interval berbeda dari LED Hijau membuktikan bahwa task-task di Core 1 berjalan paralel tanpa saling mengganggu.
```cpp
void TaskBiru(void *pvParameters) {
  pinMode(LED_BIRU, OUTPUT);
  while (true) {
    digitalWrite(LED_BIRU, HIGH);
    delay(700);
    digitalWrite(LED_BIRU, LOW);
    delay(700);
  }
}
```

### Setup Task FreeRTOS

Pada fungsi `setup()`, komunikasi serial diinisialisasi untuk menampilkan log. Ketiga task LED dibuat menggunakan `xTaskCreatePinnedToCore()`, yang memetakan masing-masing task ke core tertentu. LED Merah ditempatkan di Core 0, sedangkan LED Hijau dan Biru ditempatkan di Core 1. Fungsi `loop()` sengaja dikosongkan karena semua logika eksekusi LED dijalankan oleh task FreeRTOS.
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("Mulai program LED multi-core...");

  xTaskCreatePinnedToCore(TaskMerah, "Task Merah", 1000, NULL, 1, &TaskLED_Merah, 0);
  xTaskCreatePinnedToCore(TaskHijau, "Task Hijau", 1000, NULL, 1, &TaskLED_Hijau, 1);
  xTaskCreatePinnedToCore(TaskBiru, "Task Biru", 1000, NULL, 1, &TaskLED_Biru, 1);
}

void loop() {
  // Kosong — seluruh logic dijalankan oleh FreeRTOS
}
```

---

## 🧪 Hasil Percobaan

Setelah program dijalankan:

- **LED Merah** berkedip setiap 500 ms pada Core 0.
- **LED Hijau** berkedip setiap 300 ms pada Core 1.
- **LED Biru** berkedip setiap 700 ms pada Core 1.

Semua LED berkedip secara simultan, membuktikan bahwa task-task berjalan paralel pada core berbeda. Pola LED konsisten tanpa jitter, dan Serial Monitor menampilkan log sesuai eksekusi masing-masing core.

---

## 📸 Bukti Visual

### Core 0: LED Merah berkedip sesuai interval

![LED Merah Core 0](FOTO)

### Core 1: LED Hijau dan Biru berkedip sesuai interval

![LED Hijau dan Biru Core 1](FOTO)

---

## 🎥 Bukti Video

**Demo LED Dual-Core ESP32-S3:**  
[🎬 Tonton Video](https://drive.google.com/file/d/1hAeKub99bPkifF8pP5IE7t1ZtmiNDyJw/view?usp=drive_link)

---