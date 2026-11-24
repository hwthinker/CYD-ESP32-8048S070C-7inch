# Third-Party Notices

Project ini secara keseluruhan dirilis di bawah **GNU GPL v3** (lihat [`LICENSE`](LICENSE)), karena menyertakan dan memodifikasi komponen berlisensi GPL-3.0. Berikut semua komponen pihak ketiga, lisensinya, dan perubahan yang dibuat.

## Komponen yang disertakan di repository ini

| Komponen | Lokasi | Lisensi | Sumber | Diubah? |
|---|---|---|---|---|
| esp32-smartdisplay v2.1.2 (Rene Zeldenthuis) | `lib/esp32_smartdisplay/` | GPL-3.0 (`lib/esp32_smartdisplay/LICENSE`) | https://github.com/rzeldent/esp32-smartdisplay | Ya, lihat di bawah |
| Driver Espressif (`esp_lcd_touch`, `esp_io_expander`, `esp_lcd_panel_io_additions`, `esp_lcd_panel_io_3wire_spi`), bagian dari esp32-smartdisplay | `lib/esp32_smartdisplay/src`, `.../include` | Apache-2.0 (`LICENSES/Apache-2.0.txt`), © Espressif Systems (Shanghai) CO LTD | ESP-IDF / esp-bsp | Tidak |
| Definisi board `esp32-8048S070C` | `boards/esp32-8048S070C.json`, `include/board_8048S070C.h` | GPL-3.0 | https://github.com/rzeldent/platformio-espressif32-sunton | Ya, lihat di bawah |
| `lv_conf.h` (turunan `lv_conf_template.h` LVGL, via esp32-smartdisplay-demo) | `include/lv_conf.h` | MIT (`LICENSES/MIT-LVGL.txt`) + GPL-3.0 | https://github.com/lvgl/lvgl, https://github.com/rzeldent/esp32-smartdisplay-demo | Ya, lihat di bawah |

> Catatan: `library.json` milik esp32-smartdisplay mencantumkan `"license": "MIT"`, tetapi file `LICENSE` di repository-nya berisi GPL-3.0. Project ini mengikuti yang lebih ketat, yaitu **GPL-3.0**.

## Komponen yang di-download saat build (tidak ada di repository ini)

| Komponen | Lisensi | Keterangan |
|---|---|---|
| LVGL 9.6.0, termasuk demo *Widgets* (profil "Elena Smith", gambar avatar, dan aset lainnya) | MIT, © LVGL Kft | Di-download otomatis oleh PlatformIO (`lib_deps`). File di `src/lvgl_demo/` hanya `#include` source demo LVGL, tidak menyalinnya. Lisensi komponen lain di dalam LVGL ada di `COPYRIGHTS.md` milik LVGL. |
| Font Montserrat (dalam font bawaan LVGL) | SIL OFL-1.1 | Ikut ter-compile ke firmware. |
| Font Awesome Free (simbol `LV_SYMBOL_*` dalam font bawaan LVGL) | SIL OFL-1.1 (font) | Ikut ter-compile ke firmware. |
| Arduino-ESP32 2.0.17, ESP-IDF 4.4 (via platform `espressif32@6.13.0`) | LGPL-2.1 / Apache-2.0 | Framework dari PlatformIO. |

## Daftar modifikasi (GPL-3.0 §5a)

Semua modifikasi dibuat pada **2026-11-24**:

1. `lib/esp32_smartdisplay/include/lvgl_panel_common.h`: `lvgl_setup_panel()` sekarang mengabaikan `ESP_ERR_NOT_SUPPORTED` dari `esp_lcd_panel_disp_on_off()`. Tanpa perubahan ini, panel RGB pada ESP-IDF 4.4 tanpa pin DISP mengalami boot loop.
2. `lib/esp32_smartdisplay/include/lvgl_panel_common.h` + `src/lvgl_panel_st7262_par.c`: flush RGB dibuat pipelined — `lv_display_flush_ready()` dipanggil langsung setelah `esp_lcd_panel_draw_bitmap()` dan callback `on_frame_trans_done` dilepas. Tanpa ini setiap flush menunggu vsync sehingga scroll/grafik patah-patah.
3. `include/board_8048S070C.h`: PCLK dinaikkan ke **15 MHz** dan draw buffer dipindah ke RAM internal **800×110 (176 KB)**.
4. `include/lv_conf.h`: pool LVGL **80 KB dialokasikan di PSRAM** (`LV_MEM_POOL_ALLOC`), `LV_DEF_REFR_PERIOD` **16 ms**, demo Widgets + font Montserrat 12–24 aktif, QR code nonaktif.
5. `lib/esp32_smartdisplay/`: folder `assets/`, `boards/`, `test/`, `platformio.ini`, dan `BOARD_MATRIX.md` dihapus karena tidak dibutuhkan untuk build.
6. `boards/esp32-8048S070C.json`: define hardware dipindah dari `extra_flags` ke `include/board_8048S070C.h`. Nilai pin tidak diubah.

## Jika membagikan firmware hasil compile (`.bin`)

GPL-3.0 mewajibkan source code ikut tersedia. Cukup sertakan link ke repository ini bersama file `.bin`, beserta file `LICENSE`, folder `LICENSES/`, dan file ini.
