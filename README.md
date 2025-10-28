```
esphome:
  name: esp32p4_rtsp
  platformio_options:
    board_build.flash_mode: dio

esp32:
  board: esp32p4_devkit
  variant: esp32p4

logger:
api:
ota:
wifi:
  ssid: "TonSSID"
  password: "TonMotDePasse"

mipi_dsi_cam:
  - id: tab5_cam
    sensor: sc202cs
    external_clock_pin: GPIO36
    frequency: 24000000
    address: 0x36
    resolution: 720P
    pixel_format: RGB565
    framerate: 30

esp32p4_rtsp:
  camera_id: tab5_cam
  port: 8554
```
