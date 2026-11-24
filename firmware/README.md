# Firmware siap-flash — ESP32-8048S070C (7" 800×480, GT911 kapasitif)

Folder ini berisi firmware **LVGL 9 Widgets Demo** yang sudah jadi, untuk di-flash **tanpa VS Code / PlatformIO**, hanya dengan **esptool**.

## Isi folder

| File | Alamat flash | Keterangan |
|---|---|---|
| `firmware-merged.bin` | **0x0** | Semua bagian digabung jadi satu file — **paling mudah** |
| `bootloader.bin` | 0x0 | Bootloader ESP32-S3 |
| `partitions.bin` | 0x8000 | Tabel partisi (16 MB) |
| `boot_app0.bin` | 0xe000 | Data OTA boot |
| `firmware.bin` | 0x10000 | Aplikasi (demo LVGL) |
| `SHA256SUMS.txt` | — | Checksum untuk verifikasi file |
| `flash.bat` | — | Script bantu Windows |
| `flash.sh` | — | Script bantu Linux/macOS |

Firmware dibangun dari source terbaru repository ini, dengan tuning performa: **PCLK 15 MHz**, draw buffer **800×110 di RAM internal**, heap LVGL di **PSRAM**, refresh **16 ms**, dan flush pipelined — versi paling halus yang dicapai untuk board ini.

## Yang dibutuhkan

1. **Python 3** — https://www.python.org/downloads/ (centang *Add Python to PATH* saat install)
2. **esptool**:
   ```bash
   pip install esptool
   ```
3. **Driver CH340** (kalau port COM belum muncul) — https://www.wch-ic.com/downloads/CH341SER_EXE.html
4. Kabel USB **data**

Cek instalasi:

```bash
python -m esptool version
```

## Langkah 1 — Flash firmware

Sesuaikan `COM33` dengan port board Anda (lihat **Device Manager → Ports (COM & LPT) → USB-SERIAL CH340 (COMxx)**).

Board 7" ini umumnya bisa masuk mode download **otomatis**, jadi cukup jalankan:

### Cara A — satu file (disarankan)

```bash
python -m esptool --chip esp32s3 --port COM33 --baud 460800 write_flash 0x0 firmware-merged.bin
```

### Cara B — file terpisah

```bash
python -m esptool --chip esp32s3 --port COM33 --baud 460800 write_flash 0x0 bootloader.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 firmware.bin
```

### Kalau gagal connect (mode download manual)

Beberapa unit tidak bisa auto-reset. Lakukan:

1. **Tekan dan tahan** tombol **BOOT**.
2. Sambil menahan **BOOT**, **tekan lalu lepas** tombol **RST**.
3. **Lepas** tombol **BOOT** (board di mode download, layar gelap).
4. Jalankan perintah dengan tambahan `--before no_reset --after no_reset`:

```bash
python -m esptool --chip esp32s3 --port COM33 --baud 460800 --before no_reset --after no_reset write_flash 0x0 firmware-merged.bin
```

5. Setelah `Hash of data verified`, **tekan RST** untuk menjalankan program.

### Hapus flash dulu (opsional, kalau firmware lama bermasalah)

```bash
python -m esptool --chip esp32s3 --port COM33 --baud 460800 erase_flash
```

Setelah itu jalankan Cara A lagi.

## Langkah 2 — Jalankan program

Kalau memakai perintah tanpa `--after no_reset`, board otomatis restart dan langsung menampilkan demo. Kalau tidak, **tekan tombol RST** sekali.

## Menggunakan script bantu

Windows (PowerShell / CMD), dari folder ini:

```bat
flash.bat COM33
```

Linux/macOS:

```bash
chmod +x flash.sh
./flash.sh /dev/ttyUSB0
```

## Troubleshooting

| Masalah | Solusi |
|---|---|
| `Failed to connect to ESP32-S3` / `Wrong boot mode detected` | Lakukan urutan **BOOT + RST** lalu pakai perintah dengan `--before no_reset --after no_reset`. |
| `Timed out waiting for packet header` | Sama seperti di atas. Coba turunkan baud ke `115200`. |
| `could not open port 'COM33'` | Port salah atau dipakai aplikasi lain (Arduino IDE, serial monitor, PuTTY). Tutup aplikasi itu. |
| `esptool: command not found` / `No module named esptool` | Jalankan `pip install esptool` (atau `py -m pip install esptool` di Windows). |
| Port COM tidak muncul | Install driver CH340 dan pakai kabel USB data. |
| Flash sukses tapi layar hitam | Tekan **RST**. Pastikan board varian **8048S070C** (bukan N/R). |
| Layar menyala tapi warna aneh | Firmware ini untuk panel RGB board 7"; jangan pakai firmware board lain. |

## Verifikasi checksum (opsional)

Windows:

```powershell
Get-FileHash firmware-merged.bin -Algorithm SHA256
```

Linux/macOS:

```bash
sha256sum -c SHA256SUMS.txt
```

## Catatan

- `firmware-merged.bin` dibuat dengan flash mode **DIO** (sesuai header bootloader board ini). **Jangan** membuat ulang merged image dengan `--flash_mode qio` — header bootloader akan berubah dan board gagal boot (layar gelap).
- Alamat flash di atas **khusus ESP32-S3**. Jangan pakai firmware ini untuk board ESP32 klasik (mis. 3.5" 3248S035C).
- Jangan flash `firmware-merged.bin` ke alamat selain **0x0**.
- Setelah flash, seluruh isi flash (termasuk partisi OTA) diganti; data pengguna di flash akan hilang.
