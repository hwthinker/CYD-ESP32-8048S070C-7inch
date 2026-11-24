#!/usr/bin/env bash
# Flash firmware ESP32-8048S070C (7" 800x480, GT911)
set -u

PORT="${1:-/dev/ttyUSB0}"
DIR="$(cd "$(dirname "$0")" && pwd)"

echo "============================================================"
echo "  Flash firmware ESP32-8048S070C (7\" 800x480, GT911)"
echo "  File : firmware-merged.bin  |  Alamat : 0x0"
echo "  Port : $PORT"
echo "============================================================"
echo

python -m esptool --chip esp32s3 --port "$PORT" --baud 460800 write_flash 0x0 "$DIR/firmware-merged.bin"

if [ $? -ne 0 ]; then
  echo
  echo "[GAGAL] Coba mode download manual:"
  echo "  1. Tahan BOOT"
  echo "  2. Klik RST sambil menahan BOOT"
  echo "  3. Lepas BOOT"
  echo "  Lalu jalankan:"
  echo "  python -m esptool --chip esp32s3 --port $PORT --baud 460800 --before no_reset --after no_reset write_flash 0x0 \"$DIR/firmware-merged.bin\""
  echo "  Setelah selesai tekan RST pada board."
  exit 1
fi

echo
echo "[SUKSES] Board akan restart otomatis. Kalau tidak, tekan tombol RST."
