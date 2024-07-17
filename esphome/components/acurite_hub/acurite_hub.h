#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

namespace esphome {
namespace acurite {

class AcuriteHub : public uart::UARTDevice, public Component {
public:
  AcuriteHub() = default;
#ifdef USE_TEXT_SENSOR
  SUB_TEXT_SENSOR(data);
  SUB_TEXT_SENSOR(sensor_battery);
  SUB_TEXT_SENSOR(battery);
#endif
#ifdef USE_SENSOR
  SUB_SENSOR(wind_speed);
  SUB_SENSOR(wind_direction);
  SUB_SENSOR(wind_gust);
  SUB_SENSOR(daily_rain);
  SUB_SENSOR(rain);
  SUB_SENSOR(humidity);
  SUB_SENSOR(temperature);
  SUB_SENSOR(pressure);
  SUB_SENSOR(rssi);
  SUB_SENSOR(dew_point);
  SUB_SENSOR(average_wind_speed);
#endif

  void set_buffer_size(size_t buffer_size) { this->buffer_size_ = buffer_size; }

protected:
  void setup() override;
  void loop() override;
  void dump_config() override;

  int readline_(int readch, char *buffer, int len);
  int pos_{0};
  size_t buffer_size_{256};
  char *buffer_{};
  std::string data_{""};
};

}  // namespace acurite
}  // namespace esphome