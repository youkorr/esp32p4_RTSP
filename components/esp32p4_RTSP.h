#pragma once

#include "esphome/core/component.h"
#include "esphome/components/mipi_dsi_cam/mipi_dsi_cam.h"

#ifdef USE_ESP32_VARIANT_ESP32P4
#include "esp_h264_enc.h"
#include "esp_video.h"
#include "esp_timer.h"
#include <lwip/sockets.h>
#endif

namespace esphome {
namespace esp32p4_rtsp {

class Esp32P4RTSP : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::WIFI - 1.0f; }

  void set_camera(mipi_dsi_cam::MipiDsiCam *cam) { this->camera_ = cam; }
  void set_port(uint16_t port) { this->port_ = port; }

 protected:
  mipi_dsi_cam::MipiDsiCam *camera_{nullptr};
  uint16_t port_{8554};

#ifdef USE_ESP32_VARIANT_ESP32P4
  int rtsp_sock_{-1};
  esp_h264_enc_handle_t enc_{nullptr};
  uint8_t *h264_buf_{nullptr};
  size_t h264_buf_size_{0};

  bool start_rtsp_server_();
  bool encode_frame_h264_(const uint8_t *rgb, size_t w, size_t h, size_t *out_size);
  bool send_rtp_packet_(const uint8_t *data, size_t len);
#endif
};

}  // namespace esp32p4_rtsp
}  // namespace esphome
