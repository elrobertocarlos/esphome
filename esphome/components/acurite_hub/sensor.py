import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from . import acurite_ns, AcuriteHub, CONF_ACURITE_HUB_ID
from esphome.const import (
    CONF_WIND_SPEED,
    CONF_WIND_DIRECTION_DEGREES,
    CONF_HUMIDITY,
    CONF_TEMPERATURE,
    CONF_PRESSURE,

    DEVICE_CLASS_WIND_SPEED,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_ATMOSPHERIC_PRESSURE,
    DEVICE_CLASS_PRECIPITATION,
    DEVICE_CLASS_SIGNAL_STRENGTH,

    UNIT_KILOMETER_PER_HOUR,
    UNIT_PERCENT,
    UNIT_DEGREES,
    UNIT_CELSIUS,
    UNIT_MILLIMETER,
    UNIT_HECTOPASCAL,

    STATE_CLASS_MEASUREMENT,

    ENTITY_CATEGORY_DIAGNOSTIC,
)

DEPENDENCIES = ["acurite_hub"]

CONF_WIND_GUST = "wind_gust"
CONF_DAILY_RAIN = "daily_rain"
CONF_RAIN = "rain"
CONF_RSSI = "rssi"
CONF_DEW_POINT = "dew_point"
CONF_AVERAGE_WIND_SPEED = "average_wind_speed"

ICON_COMPASS_ROSE = "mdi:compass-rose"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ACURITE_HUB_ID): cv.use_id(AcuriteHub),
        cv.Optional(CONF_WIND_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_WIND_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_WIND_DIRECTION_DEGREES): sensor.sensor_schema(
            unit_of_measurement=UNIT_DEGREES,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon=ICON_COMPASS_ROSE,
            ),
        cv.Optional(CONF_WIND_GUST): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_WIND_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_DAILY_RAIN): sensor.sensor_schema(
            unit_of_measurement=UNIT_MILLIMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_PRECIPITATION,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_RAIN): sensor.sensor_schema(
            unit_of_measurement=UNIT_MILLIMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_PRECIPITATION,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_HUMIDITY,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_PRESSURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_HECTOPASCAL,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_ATMOSPHERIC_PRESSURE,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_RSSI): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SIGNAL_STRENGTH,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
        cv.Optional(CONF_DEW_POINT): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
        cv.Optional(CONF_AVERAGE_WIND_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_WIND_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    acurite_hub = await cg.get_variable(config[CONF_ACURITE_HUB_ID])

    if wind_speed := config.get(CONF_WIND_SPEED):
        s = await sensor.new_sensor(wind_speed)
        cg.add(acurite_hub.set_wind_speed_sensor(s))

    if wind_direction := config.get(CONF_WIND_DIRECTION_DEGREES):
        s = await sensor.new_sensor(wind_direction)
        cg.add(acurite_hub.set_wind_direction_sensor(s))

    if wind_gust := config.get(CONF_WIND_GUST):
        s = await sensor.new_sensor(wind_gust)
        cg.add(acurite_hub.set_wind_gust_sensor(s))

    if daily_rain := config.get(CONF_DAILY_RAIN):
        s = await sensor.new_sensor(daily_rain)
        cg.add(acurite_hub.set_daily_rain_sensor(s))

    if rain := config.get(CONF_RAIN):
        s = await sensor.new_sensor(rain)
        cg.add(acurite_hub.set_rain_sensor(s))

    if humidity := config.get(CONF_HUMIDITY):
        s = await sensor.new_sensor(humidity)
        cg.add(acurite_hub.set_humidity_sensor(s))

    if temperature := config.get(CONF_TEMPERATURE):
        s = await sensor.new_sensor(temperature)
        cg.add(acurite_hub.set_temperature_sensor(s))

    if pressure := config.get(CONF_PRESSURE):
        s = await sensor.new_sensor(pressure)
        cg.add(acurite_hub.set_pressure_sensor(s))

    if rssi := config.get(CONF_RSSI):
        s = await sensor.new_sensor(rssi)
        cg.add(acurite_hub.set_rssi_sensor(s))
        
    if dew_point := config.get(CONF_DEW_POINT):
        s = await sensor.new_sensor(dew_point)
        cg.add(acurite_hub.set_dew_point_sensor(s))

    if average_wind_speed := config.get(CONF_AVERAGE_WIND_SPEED):
        s = await sensor.new_sensor(average_wind_speed)
        cg.add(acurite_hub.set_average_wind_speed_sensor(s))
