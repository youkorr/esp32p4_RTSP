#include "esp32p4_RTSP.h"
#ifdef USE_ESP32_VARIANT_ESP32P4

#include "esphome/core/log.h"
#include <string>
#include <lwip/netdb.h>
#include <lwip/sockets.h>

namespace esphome {
namespace esp32p4_rtsp {

static const char *const TAG = "esp32p4_rtsp";

void Esp32P4RTSP::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ESP32-P4 RTSP server...");
  if (!this->camera_) {
    ESP_LOGE(TAG, "Camera not set");
    this->mark_failed();
    return;
  }

  // H.264 encoder init
  esp_h264_enc_cfg_t cfg = {
      .width = this->camera_->get_image_width(),
      .height = this->camera_->get_image_height(),
      .bitrate = 2000000,
      .fps = this->camera_->get_fps(),
      .gop_size = 30,
  };
  if (esp_h264_enc_open(&cfg, &this->enc_) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to open H.264 encoder");
    this->mark_failed();
    return;
  }

  this->h264_buf_size_ = 512 * 1024;
  this->h264_buf_ = (uint8_t *) heap_caps_malloc(this->h264_buf_size_, MALLOC_CAP_SPIRAM);
  this->start_rtsp_server_();
}

void Esp32P4RTSP::loop() {}

void Esp32P4RTSP::dump_config() {
  ESP_LOGCONFIG(TAG, "ESP32-P4 RTSP:");
  ESP_LOGCONFIG(TAG, "  Port: %u", this->port_);
}

bool Esp32P4RTSP::start_rtsp_server_() {
  this->rtsp_sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (this->rtsp_sock_ < 0) return false;

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(this->port_);

  bind(this->rtsp_sock_, (struct sockaddr *)&addr, sizeof(addr));
  listen(this->rtsp_sock_, 1);

  ESP_LOGI(TAG, "RTSP server started on rtsp://0.0.0.0:%d/stream", this->port_);
  return true;
}

bool Esp32P4RTSP::encode_frame_h264_(const uint8_t *rgb, size_t w, size_t h, size_t *out_size) {
  esp_h264_enc_frame_t frame = {.input = rgb, .input_size = w * h * 2,
                                .output = this->h264_buf_, .output_size = this->h264_buf_size_};
  esp_err_t ret = esp_h264_enc_process(this->enc_, &frame);
  if (ret == ESP_OK) {
    *out_size = frame.bytes_encoded;
    return true;
  }
  return false;
}

bool Esp32P4RTSP::send_rtp_packet_(const uint8_t *data, size_t len) {
  // TODO: ajouter paquet RTP + envoi via UDP
  return true;
}

}  // namespace esp32p4_rtsp
}  // namespace esphome

#endif
