#include "acurite_hub.h"
#include "esphome/components/wifi/wifi_component.h"
#include <map>

namespace esphome {
namespace acurite {

static const char *const TAG = "acurite_hub";

void AcuriteHub::dump_config() {
  ESP_LOGCONFIG(TAG, "Acurite Hub:");
  ESP_LOGCONFIG(TAG, "  Buffer size: %d", this->buffer_size_);
#ifdef USE_TEXT_SENSOR
  LOG_TEXT_SENSOR("  ", "Data", this->data_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Sensor battery", this->sensor_battery_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Battery", this->battery_text_sensor_);
#endif
#ifdef USE_SENSOR
  LOG_SENSOR("  ", "Wind speed", this->wind_speed_sensor_);
  LOG_SENSOR("  ", "Wind direction", this->wind_direction_sensor_);
  LOG_SENSOR("  ", "Wind gust", this->wind_gust_sensor_);
  LOG_SENSOR("  ", "Daily rain", this->daily_rain_sensor_);
  LOG_SENSOR("  ", "Rain", this->rain_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Pressure", this->pressure_sensor_);
  LOG_SENSOR("  ", "RSSI", this->rssi_sensor_);
  LOG_SENSOR("  ", "Dew point", this->dew_point_sensor_);
  LOG_SENSOR("  ", "Average wind speed", this->average_wind_speed_sensor_);
#endif
}

void AcuriteHub::setup() {
  this->buffer_ = new char[this->buffer_size_]();
#ifdef USE_SENSOR
  if (this->rssi_sensor_ != nullptr)
    this->rssi_sensor_->set_disabled_by_default(true);
#endif
#ifdef USE_TEXT_SENSOR
  if (this->data_text_sensor_ != nullptr)
    this->data_text_sensor_->set_internal(true);
#endif
}

int AcuriteHub::readline_(int readch, char *buffer, int len) {
  int rpos;
  if (readch > 0) {
    switch (readch) {
    // case '\n': // Ignore new-lines
    //   break;
    case '\n': // Return on CR
      rpos = this->pos_;
      this->pos_ = 0;  // Reset position index ready for next time
      return rpos;
    default:
      if (this->pos_ < len-1) {
        buffer[this->pos_++] = readch;
        buffer[this->pos_] = 0;
      }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

const char *AT_RESET = "AT+RESET";
const char *AT_TEST = "AT+TEST";
const char *AT_SETTING = "AT+SETTING";
const char *AT_DATA = "AT+DATA=";
const char *AT_SETTING_Q = "AT+?SETTING";
const char *AT_WIFI_Q = "AT+?WIFI";
const char *AT_INTERNET_Q = "AT+?INTERNET";
const char *AT_RSSI_Q = "AT+?RSSI";
const char *AT_TRY_Q = "AT+?TRY";
const char *AT_OK_R = "OK\r\n";

void AcuriteHub::loop() {
  while (available()) {
    const char read_char=read();
    if(this->readline_(read_char, this->buffer_, this->buffer_size_) > 0) {
      if (strncmp(this->buffer_, AT_RESET, strlen(AT_RESET)) == 0) {
        ESP_LOGI(TAG, "Reset request.");
        flush();
      } else if(strncmp(this->buffer_, AT_TEST, strlen(AT_TEST)) == 0) {
        write_str("AT+TEST\r\n");
        write_str(AT_OK_R);
        flush();
      } else if(strncmp(this->buffer_, AT_SETTING_Q, strlen(AT_SETTING_Q)) == 0) {
        write_str("AT+SETTING=1\r\n");
        write_str(AT_OK_R);
        flush();
      } else if(strncmp(this->buffer_, AT_SETTING, strlen(AT_SETTING)) == 0) {
          write_str("AT+SETTING30\r\n");
        write_str(AT_OK_R);
        flush();
      } else if(strncmp(this->buffer_, AT_WIFI_Q, strlen(AT_WIFI_Q)) == 0) {
        int status = WiFi.status();
        std::string wifi_status = str_sprintf( "AT+WIFI=%d\r\n", status == WL_CONNECTED? 1 : 0 );
        write_str(wifi_status.c_str());
        write_str(AT_OK_R);
        flush();
      } else if(strncmp(this->buffer_, AT_INTERNET_Q, strlen(AT_INTERNET_Q)) == 0) {
          int status = WiFi.status();
        std::string wifi_status = str_sprintf( "AT+INTERNET=%d\r\n", status == WL_CONNECTED? 1 : 0 );
        write_str(wifi_status.c_str());
        write_str(AT_OK_R);
        flush();
      } else if(strncmp(this->buffer_, AT_RSSI_Q, strlen(AT_RSSI_Q)) == 0) {
        long rssi = WiFi.RSSI();
        std::string wifi_signal = str_sprintf( "AT+RSSI=%ld\r\n", rssi );
        write_str(wifi_signal.c_str());
        write_str(AT_OK_R);
        flush();
      } else if(strncmp(this->buffer_, AT_TRY_Q, strlen(AT_TRY_Q)) == 0) {
        write_str("AT+TRY=1\r\n");
        write_str(AT_OK_R);
        flush();
      } else if(strncmp(this->buffer_, AT_DATA, strlen(AT_DATA)) == 0) {
        write_str("AT+DATA\r\n");
        write_str(AT_OK_R);
        char *new_data = this->buffer_ + strlen(AT_DATA);
        if (new_data == this->data_) {
            // ESP_LOGD(TAG, "new\t%s", this->data_.c_str());
            // ESP_LOGD(TAG, "old\t%s", new_data);
            // ESP_LOGD(TAG, "No data update");
            flush();
            break;
        }
        this->data_ = new_data;

#ifdef USE_TEXT_SENSOR
        if (this->data_text_sensor_ != nullptr) {
          this->data_text_sensor_->publish_state(this->data_);
        }
#endif
        
        std::map<std::string,std::string> query;

        int pc=0;
        char *tok;
        char *otok;
        for(tok=strtok(new_data,"&"); tok!=NULL; tok=strtok(tok,"&")) {
            pc++;
            otok=tok+strlen(tok)+1;
            tok=strtok(tok,"=");
            std::string key = tok;
            tok=strtok(NULL,"=");
            std::string value = tok;
            tok=otok;
            query[key] = value;
        };
#ifdef USE_TEXT_SENSOR
        if (this->sensor_battery_text_sensor_ != nullptr) {
          std::string sensorbattery = query["sensorbattery"];
          if (this->sensor_battery_text_sensor_->state != sensorbattery) {
              this->sensor_battery_text_sensor_->publish_state(sensorbattery);
          }
        }
        
        if (this->battery_text_sensor_ != nullptr) {
          std::string hubbattery = query["hubbattery"];
          if (this->battery_text_sensor_->state != hubbattery) {
              this->battery_text_sensor_->publish_state(hubbattery);
          }
        }
#endif
#ifdef USE_SENSOR
        if (this->wind_speed_sensor_ != nullptr) {
          int windspeedmph = std::stoi(query["windspeedmph"]);
          float windspeedkph = windspeedmph * 1.609344;
          if (this->wind_speed_sensor_->state != windspeedkph) {
              this->wind_speed_sensor_->publish_state(windspeedkph);
          }
        }

        if (this->wind_direction_sensor_ != nullptr) {
          int winddir = std::stoi(query["winddir"]);
          if (this->wind_direction_sensor_->state != winddir) {
              this->wind_direction_sensor_->publish_state(winddir);
          }
        }
        
        if (this->wind_gust_sensor_ != nullptr) {
          int windgustmph = std::stoi(query["windgustmph"]);
          float windgustkph = windgustmph * 1.609344;
          if (this->wind_gust_sensor_->state != windgustkph) {
              this->wind_gust_sensor_->publish_state(windgustkph);
          }
        }

        if (this->daily_rain_sensor_ != nullptr) {
          float dailyrainin = std::stof(query["dailyrainin"]);
          float dailyrainmm = dailyrainin * 254;
          if (this->daily_rain_sensor_->state != dailyrainmm) {
              this->daily_rain_sensor_->publish_state(dailyrainmm);
          }
        }
        
        if (this->rain_sensor_ != nullptr) {
          float rainin = std::stof(query["rainin"]);
          float rainmm = rainin * 254;
          if (this->rain_sensor_->state != rainmm) {
              this->rain_sensor_->publish_state(rainmm);
          }
        }
        
        if (this->humidity_sensor_ != nullptr) {
          int humidity = std::stoi(query["humidity"]);
          if (this->humidity_sensor_->state != humidity) {
              this->humidity_sensor_->publish_state(humidity);
          }
        }
        
        if (this->temperature_sensor_ != nullptr) {
          float tempf = std::stof(query["tempf"]);
          float tempc = (tempf - 32) * 5/9;
          if (this->temperature_sensor_->state != tempc) {
              this->temperature_sensor_->publish_state(tempc);
          }
        }
        
        if (this->pressure_sensor_ != nullptr) {
          float baromhpa = std::stof(query["baromin"]) * 33.86389;
          if (this->pressure_sensor_->state != baromhpa) {
              this->pressure_sensor_->publish_state(baromhpa);
          }
        }

        if (this->rssi_sensor_ != nullptr) {
          int rssi = std::stoi(query["rssi"]);
          if (this->rssi_sensor_->state != rssi) {
              this->rssi_sensor_->publish_state(rssi);
          }
        }
        
        if (this->dew_point_sensor_ != nullptr) {
          int dewptf = std::stoi(query["dewptf"]);
          float dewptc = (float(dewptf) - 32) * 5/9;
          if (this->dew_point_sensor_->state != dewptc) {
              this->dew_point_sensor_->publish_state(dewptc);
          }
        }
        
        if (this->average_wind_speed_sensor_ != nullptr) {
          int windspeedavgmph = std::stoi(query["windspeedavgmph"]);
          float windspeedavgkph = windspeedavgmph * 1.609344;
          if (this->average_wind_speed_sensor_->state != windspeedavgkph) {
              this->average_wind_speed_sensor_->publish_state(windspeedavgkph);
          }
        }
#endif
        flush();
      } else {
          ESP_LOGE(TAG, "unkown command r: %s", this->buffer_);
      }
    }
  }
}

}  // namespace acurite
}  // namespace esphome