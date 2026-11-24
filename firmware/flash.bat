@echo off
setlocal
set PORT=%~1
if "%PORT%"=="" set PORT=COM33

echo ============================================================
echo   Flash firmware ESP32-8048S070C (7" 800x480, GT911)
echo   File : firmware-merged.bin  ^|  Alamat : 0x0
echo   Port : %PORT%
echo ============================================================
echo.

python -m esptool --chip esp32s3 --port %PORT% --baud 460800 write_flash 0x0 "%~dp0firmware-merged.bin"

if errorlevel 1 (
    echo.
    echo [GAGAL] Coba lagi dengan mode download manual:
    echo   1. Tahan BOOT
    echo   2. Klik RST sambil menahan BOOT
    echo   3. Lepas BOOT
    echo   Lalu jalankan:
    echo   python -m esptool --chip esp32s3 --port %PORT% --baud 460800 --before no_reset --after no_reset write_flash 0x0 "%~dp0firmware-merged.bin"
    echo   Setelah selesai tekan RST pada board.
) else (
    echo.
    echo [SUKSES] Board akan restart otomatis. Kalau tidak, tekan tombol RST.
)
echo.
pause
