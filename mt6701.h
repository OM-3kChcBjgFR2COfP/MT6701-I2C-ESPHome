#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace mt6701 {

// регистр угла
static const uint8_t MT6701_REG_ANGLE_MSB = 0x03;

// масштаб 14 бит → градусы
static const float MT6701_ANGLE_SCALE = 360.0f / 16384.0f;

class MT6701Component : public PollingComponent, public i2c::I2CDevice {
 public:
  MT6701Component() : PollingComponent(20) {}

  // ---- setters ----
  void set_angle_sensor(sensor::Sensor *s) { angle_sensor_ = s; }
  void set_raw_sensor(sensor::Sensor *s) { raw_sensor_ = s; }
  void set_status_sensor(sensor::Sensor *s) { status_sensor_ = s; }
  void set_zero_binary_sensor(binary_sensor::BinarySensor *s) { zero_binary_sensor_ = s; }

  // fast polling control
  void set_fast_polling(bool enable);

  // ---- getters ----
  float get_last_angle_deg() const { return last_angle_deg_; }
  uint16_t get_last_raw() const { return last_raw_; }
  bool is_fast_polling() const { return fast_polling_; }

  void setup() override;
  void update() override;
  void loop() override;                 
  void dump_config() override;

  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  sensor::Sensor *angle_sensor_{nullptr};
  sensor::Sensor *raw_sensor_{nullptr};
  sensor::Sensor *status_sensor_{nullptr};
  binary_sensor::BinarySensor *zero_binary_sensor_{nullptr};

  float prev_angle_{0.0f};
  float last_angle_deg_{0.0f};
  uint16_t last_raw_{0};

  bool fast_polling_{false};
  uint32_t last_fast_read_{0};          

  void read_and_process_();             
  bool read_angle_(uint16_t &raw_angle, uint8_t &status);
};

}  // namespace mt6701
}  // namespace esphome