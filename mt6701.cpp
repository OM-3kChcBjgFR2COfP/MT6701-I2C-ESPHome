#include "mt6701.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mt6701 {

static const char *const TAG = "mt6701";

void MT6701Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up MT6701 (I2C)...");

  uint8_t test;
  if (read_register(MT6701_REG_ANGLE_MSB, &test, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "MT6701 not found!");
    mark_failed();
    return;
  }

  ESP_LOGI(TAG, "MT6701 detected");
}

void MT6701Component::set_fast_polling(bool enable) {
  fast_polling_ = enable;

  if (enable) {
    ESP_LOGI(TAG, "Fast polling ENABLED (50ms)");
    last_fast_read_ = millis();
  } else {
    ESP_LOGI(TAG, "Fast polling DISABLED");
  }
}

void MT6701Component::loop() {
  if (!fast_polling_)
    return;

  uint32_t now = millis();

  if (now - last_fast_read_ >= 50) {
    last_fast_read_ = now;
    this->read_and_process_();
  }
}

void MT6701Component::update() {
  if (fast_polling_)
    return;  // в fast режиме обычный update игнорируем

  this->read_and_process_();
}

void MT6701Component::read_and_process_() {
  uint16_t raw;
  uint8_t status;

  if (!read_angle_(raw, status))
    return;

  float angle = raw * MT6701_ANGLE_SCALE;

  last_raw_ = raw;
  last_angle_deg_ = angle;

  if (!fast_polling_) {
    if (raw_sensor_)
      raw_sensor_->publish_state(raw);

    if (angle_sensor_)
      angle_sensor_->publish_state(angle);

    if (status_sensor_)
      status_sensor_->publish_state(status);
  }

  bool crossed_zero_cw =
      (prev_angle_ > 300.0f && angle < 60.0f);

  bool crossed_zero_ccw =
      (prev_angle_ < 60.0f && angle > 300.0f);

  bool crossed_zero = crossed_zero_cw || crossed_zero_ccw;

  prev_angle_ = angle;

  if (zero_binary_sensor_)
    zero_binary_sensor_->publish_state(crossed_zero);
}

bool MT6701Component::read_angle_(uint16_t &raw_angle, uint8_t &status) {
  uint8_t data[2];

  if (read_register(MT6701_REG_ANGLE_MSB, data, 2) != i2c::ERROR_OK)
    return false;

  uint16_t angle_raw = 0;
  angle_raw |= ((data[0] >> 2) & 0x3F) << 8;
  angle_raw |= (data[1] >> 2) & 0x3F;

  raw_angle = angle_raw;
  status = data[0] & 0x03;

  return true;
}

void MT6701Component::dump_config() {
  ESP_LOGCONFIG(TAG, "MT6701:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace mt6701
}  // namespace esphome