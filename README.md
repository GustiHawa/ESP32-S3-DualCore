# ⚙️ ESP32-S3 — Multi-Core Peripheral Experiments

## 🧩 Identitas
**Nama:** Gusti Hawa Sa'bania Bukian  
**NRP:** 3223600024  
**Kelas:** 3 D4 Teknik Komputer A  

---

## 📘 Deskripsi
Repository ini merupakan kumpulan eksperimen praktikum berbasis **ESP32-S3**,  
yang berfokus pada pemanfaatan **dual-core processing (Core 0 & Core 1)**  
menggunakan **FreeRTOS** untuk mengontrol berbagai jenis **peripheral I/O**.

Tujuan utama eksperimen ini adalah untuk:
- Menghubungkan dan menguji seluruh peripheral ke board ESP32-S3.  
- Menjalankan setiap I/O pada **core berbeda** untuk memahami konsep multitasking.  
- Menganalisis performa, sinkronisasi, dan respon antar task.  
- Mendokumentasikan hasil berupa **langkah percobaan, wiring, gambar, dan video**.

---

## 🧠 Struktur Repository
Setiap eksperimen memiliki **folder terpisah** yang berisi:
- Penjelasan singkat fungsi dan konsep percobaan.  
- Konfigurasi hardware / pin mapping.  
- Langkah percobaan dan hasil output.  
- File program Arduino (`program.ino`).  
- Simulasi **Wokwi** untuk verifikasi koneksi.

📂 **Daftar eksperimen:**
1. 🟢 [1-LED](./1-LED)
2. 🔘 [2-PushButton](./2-PushButton)
3. 🔔 [3-Buzzer](./3-Buzzer)
4. 🎚️ [4-Potensiometer](./4-Potensiometer)
5. ⚙️ [5-MotorServo](./5-MotorServo)
6. 🔄 [6-MotorStepper](./6-MotorStepper)
7. 🎛️ [7-RotaryEncoder](./7-RotaryEncoder)
8. 🖥️ [8-OLED](./8-OLED)

---

## 🧪 Simulasi Wokwi
Simulasi seluruh rangkaian dapat diakses melalui link berikut:  
👉 [Wokwi Project Simulation](https://wokwi.com/projects/447206591545559041)

---

## 🏁 Tujuan Akhir
Mampu memahami dan mengimplementasikan:
- Konsep **dual-core multitasking** pada ESP32-S3.  
- Komunikasi antar-peripheral berbasis **FreeRTOS tasks**.  
- Praktik dokumentasi dan version control berbasis **Git & GitHub**.  

---

