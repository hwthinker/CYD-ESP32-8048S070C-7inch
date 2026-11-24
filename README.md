# ESP32-S3 7" CYD (ESP32-8048S070C) — LVGL 9 Widgets Demo

Demo **LVGL 9 Widgets** (halaman *Profile* "Elena Smith" yang legendaris, plus tab *Analytics* dan *Shop*) untuk board **Sunton ESP32-8048S070C** — LCD 7 inch 800×480 dengan **touch kapasitif GT911**.

Project ini siap pakai dengan **PlatformIO**: clone → build → upload, tanpa setting tambahan.

## Hardware

| Item | Keterangan |
|---|---|
| Board | Sunton **ESP32-8048S070C** (huruf **C** = capacitive touch) |
| MCU | ESP32-S3, 16 MB flash, 8 MB PSRAM |
| Layar | 7" 800×480, RGB paralel 16-bit (ST7262) |
| Touch | GT911 (I²C: SDA 19, SCL 20, RST 38) |
| Backlight | GPIO 2 |
| USB | chip USB-serial **CH340** |

> Varian **N** (tanpa touch) dan **R** (touch resistif) **tidak** cocok dengan project ini tanpa mengganti file board.

## Yang dibutuhkan

1. **VS Code** — https://code.visualstudio.com
2. Extension **PlatformIO IDE** (cari "PlatformIO" di tab Extensions VS Code)
3. **Driver CH340** jika board tidak muncul sebagai port COM — https://www.wch-ic.com/downloads/CH341SER_EXE.html
4. Kabel USB **data** (bukan kabel charge saja)

## Firmware siap-flash (tanpa PlatformIO)

Kalau hanya ingin mem-flash board tanpa install VS Code/PlatformIO, gunakan folder [`firmware/`](firmware/). Di dalamnya sudah ada `firmware-merged.bin` (siap flash di alamat `0x0`), binary pendukung, script bantu, dan panduan lengkap esptool: **[firmware/README.md](firmware/README.md)**.

## Dua environment PlatformIO

| Env | Isi | Perintah upload |
|---|---|---|
| `esp32-8048S070C` (**default**) | Demo LVGL Widgets "Elena Smith" (`src/main.cpp`) | `pio run -t upload --upload-port COM33` |
| `simple_demo` | Label sederhana "Halo dari ESP32-S3!" (`src/simple_demo.cpp`) | `pio run -e simple_demo -t upload --upload-port COM33` |

Kalau `-e` tidak ditulis, PlatformIO memakai env default. Folder [`firmware/`](firmware/) tetap berisi **demo Widgets** (bukan `simple_demo`).

## Langkah-langkah

### 1. Download project

```bash
git clone https://github.com/hwthinker/hwthinker-CYD-ESP32-8048SO70C-7inch.git
```

atau klik **Code → Download ZIP** di GitHub, lalu extract.

> ⚠️ **Windows:** taruh project di folder dengan path pendek, misalnya `C:\Users\<nama>\Documents\<nama-repo>`. Path yang sangat panjang (>~80 karakter) bisa membuat compile gagal dengan error `No such file or directory` karena batas 260 karakter Windows.

### 2. Buka di VS Code

**File → Open Folder…** → pilih folder project (yang berisi `platformio.ini`).
PlatformIO otomatis men-download platform ESP32, toolchain, dan LVGL. **Proses pertama bisa 5–15 menit**, tunggu sampai selesai (lihat status bar bawah).

### 3. Build

Klik ikon **✓ (Build)** di status bar bawah, atau di terminal:

```bash
pio run
```

Berhasil jika muncul `[SUCCESS]`.

### 4. Upload ke board

1. Colok board ke PC lewat USB.
2. Klik ikon **→ (Upload)** di status bar, atau:

```bash
pio run -t upload
```

PlatformIO mendeteksi port otomatis. Jika PC punya beberapa port COM dan yang terpilih salah, tentukan sendiri:

```bash
pio run -t upload --upload-port COM33
```

(Lihat nomor port di **Device Manager → Ports (COM & LPT) → USB-SERIAL CH340 (COMxx)**.)

### 5. Selesai 🎉

Setelah upload, board restart dan menampilkan halaman **Profile Elena Smith**. Coba sentuh tab, slider, switch, dan textbox (keyboard muncul di layar).

Serial monitor (opsional): `pio device monitor`

## Troubleshooting

| Masalah | Solusi |
|---|---|
| `Could not open COMxx ... Access is denied` | Port sedang dipakai aplikasi lain (Arduino IDE, serial monitor, Cura, dll). Tutup aplikasi itu, lalu upload lagi. |
| Port COM tidak muncul | Install driver CH340, ganti kabel USB (harus kabel data). |
| `Failed to connect to ESP32` | Tahan tombol **BOOT**, tekan & lepas **RST**, lalu lepas **BOOT**; upload ulang. |
| Compile error `No such file or directory` pada file yang sebenarnya ada | Path folder project terlalu panjang — pindahkan ke folder yang lebih pendek. |
| Layar hitam tapi upload sukses | Cek serial monitor; tekan RST. Pastikan board benar varian **8048S070C**. |

## Struktur project

```
├── LICENSE                     # GPL-3.0
├── THIRD_PARTY_NOTICES.md      # lisensi pihak ketiga + daftar modifikasi
├── LICENSES/                   # teks lisensi Apache-2.0 & MIT (LVGL)
├── platformio.ini              # konfigurasi build (espressif32@6.13.0, LVGL 9.6.0)
├── boards/esp32-8048S070C.json # definisi board untuk PlatformIO
├── include/
│   ├── board_8048S070C.h       # semua pin & timing LCD/touch/SD card
│   └── lv_conf.h               # konfigurasi LVGL (demo widgets aktif)
├── lib/esp32_smartdisplay/     # driver LCD + touch (sudah di-patch, lihat bawah)
└── src/
    ├── main.cpp                # env default: init display + lv_demo_widgets()
    ├── simple_demo.cpp         # env simple_demo: contoh UI label sederhana
    └── lvgl_demo/              # wrapper agar source demo LVGL ikut di-compile
```

## Membuat UI sendiri

Ganti **hanya** `lv_demo_widgets();` di `src/main.cpp` dengan widget buatan sendiri. `smartdisplay_init()` dan `smartdisplay_lcd_set_backlight()` **wajib tetap ada** — keduanya yang menginisialisasi LVGL, panel LCD, touch, dan menyalakan backlight. Contoh lengkap yang sudah siap dipakai ada di [`src/simple_demo.cpp`](src/simple_demo.cpp) (env `simple_demo`).

```cpp
#include <Arduino.h>
#include <esp32_smartdisplay.h>
// #include <demos/lv_demos.h>   // tidak perlu kalau tidak memakai demo

void setup()
{
    Serial.begin(115200);

    smartdisplay_init();                  // WAJIB
    smartdisplay_lcd_set_backlight(1.0f); // WAJIB (kecerahan penuh)

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Halo dari ESP32-S3!");
    lv_obj_center(label);
}

auto lv_last_tick = millis();

void loop()
{
    auto const now = millis();
    lv_tick_inc(now - lv_last_tick);
    lv_last_tick = now;
    lv_timer_handler();
}
```

> ⚠️ Kalau `smartdisplay_init()` ikut dihapus/dikomentari, layar akan **gelap**: LVGL belum di-`init`, panel belum dibuat, dan backlight tidak menyala.

Jika demo tidak dipakai lagi, hapus folder `src/lvgl_demo/` dan set `LV_BUILD_DEMOS 0` di `include/lv_conf.h` agar firmware lebih kecil.

## Catatan teknis

- **Platform `espressif32@6.13.0`** (Arduino 2.x / ESP-IDF 4.4) wajib — library `esp32-smartdisplay` belum kompatibel dengan Arduino 3.x / IDF 5.
- **Patch 1** — `lib/esp32_smartdisplay/include/lvgl_panel_common.h`: `esp_lcd_panel_disp_on_off()` mengembalikan `ESP_ERR_NOT_SUPPORTED` pada panel RGB tanpa pin DISP sehingga board boot-loop. Error tersebut sekarang diabaikan.
- **Patch 2 (performa)** — `lv_display_flush_ready()` dipanggil langsung setelah `esp_lcd_panel_draw_bitmap()` (copy RGB sinkron), dan `on_frame_trans_done` dilepas dari `lvgl_panel_st7262_par.c`. Tanpa ini setiap flush menunggu vsync sehingga scroll/grafik patah-patah.
- **Tuning tampilan 7"** — PCLK **15 MHz**, draw buffer **800×60 (96 KB) di RAM internal**, heap LVGL **80 KB di RAM internal**, `LV_DEF_REFR_PERIOD` **33 ms**, flush pipelined, dan overlay **FPS/CPU** (`LV_USE_PERF_MONITOR`) aktif. PCLK 16 MHz sempat tearing. Pada tab Analytics bagian chart, LVGL 9 tetap lebih berat daripada demo manufaktur (LVGL 8) — FPS ±11–16 vs 35–47.
- **Define hardware** dipindah dari board JSON ke `include/board_8048S070C.h` (force-include via `build_flags`) agar command line compiler tidak melebihi batas 32K karakter Windows.
- Source demo LVGL ada di folder `demos/` yang tidak di-compile otomatis oleh PlatformIO, karena itu ada wrapper di `src/lvgl_demo/`.

## Kredit & lisensi

Project ini dirilis di bawah **GNU GPL v3** (lihat [`LICENSE`](LICENSE)). Boleh dipakai, diubah, dan dibagikan, termasuk untuk komersial, dengan syarat versi yang dibagikan juga tetap GPL-3.0 dan source code-nya tersedia.

- [rzeldent/esp32-smartdisplay](https://github.com/rzeldent/esp32-smartdisplay): driver display/touch (GPL-3.0, sudah dimodifikasi). Di dalamnya ada driver Espressif (Apache-2.0).
- [rzeldent/platformio-espressif32-sunton](https://github.com/rzeldent/platformio-espressif32-sunton): definisi board (GPL-3.0)
- [LVGL](https://lvgl.io): library grafis dan demo Widgets "Elena Smith" (MIT), di-download otomatis saat build.

Daftar lengkap lisensi pihak ketiga dan modifikasi yang dibuat ada di [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md). Teks lisensi pendukungnya ada di folder [`LICENSES/`](LICENSES).
