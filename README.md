Acurite Hub 06086M component for ESPHome
======================================

The Acurite Hub model 06086M is a Wi-Fi enabled display that receives RF data from the 5-in-1 RF sensor and pushes updates to wunderground PWS system. It uses a `ESP-WROOM-02` (https://docs.platformio.org/en/latest/boards/espressif8266/esp_wroom_02.html) for the internet connectivity and it receives AT commands from a epoxy blob MCU that processes the RF signal from the station. No local humidity, temperature or any other sensor is available.

### Product page

AcuRite Iris (5-in-1) Color Display with Wi-Fi Connection to Weather Underground
https://www.acurite.com/shop-all/weather-instruments/weather-sensors-and-parts/displays/wi-fi-display-for-5-in-1-sensor.html

Installation
------------

### Flashing

- Remove `R28` and `R27` to disconnect the board from the RF MCU.
- Bridge `JP7` (`IO0`) temporarily to enable UART download mode.
- Connect your UART adapter to the conveniently exposed `GND`, `TXD` and `RXD`.
- Upload your new firmware using your favorite method.
- Reinstall `R27` and `R28` to return the connection to the RF MCU.
- Be sure to unbridge `JP7` for normal operation.

### Importing

This is an external ESPHome component and therefore have to be explicitly declared before use.

```yaml
external_components:
  - source: github://elrobertocarlos/esphome@main
    components: [ acurite_hub ]
```

Configuration
-------------

### UART
`UART0` is used by the RF microcontroller so we need to disable serial logger or switch to `UART1`. `UART1_TXD` is available on the unconnected pin 7.

```yaml
# Enable logging on UART1
logger:
  hardware_uart: UART1
```

```yaml
uart:
  id: uart_bus
  tx_pin: GPIO1
  rx_pin: GPIO3
  baud_rate: 115200
```
Add the hub and specify the UART ID (optional)

```yaml
acurite_hub:
  id: hub
  uart_id: uart_bus
```


### Sensors

```yaml
sensor:
  - platform: acurite_hub
    acurite_hub_id: hub
    wind_speed:
      name: Wind Speed
    wind_direction_degrees:
      name: Wind Direction
    wind_gust:
      name: Wind Gust
    daily_rain:
      name: Daily Rain
    rain:
      name: Rain
    humidity:
      name: Humidity
    temperature:
      name: Temperature
    pressure:
      name: Pressure
    rssi:
      name: Station RSSI
    dew_point:
      name: Dew Point
    average_wind_speed:
      name: Average Wind Speed

text_sensor:
  - platform: acurite_hub
    data:
      name: Data
      internal: true #default
    sensor_battery:
      name: Sensor Battery
    battery:
      name: Hub Battery
```
