import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from . import acurite_ns, AcuriteHub, CONF_ACURITE_HUB_ID
from esphome.const import (
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_BATTERY,
)

CONF_DATA = "data"
CONF_SENSOR_BATTERY = "sensor_battery"
CONF_BATTERY = "battery"

ICON_BATTERY_CHARGING_WIRELESS = "mdi:battery-charging-wireless"
ICON_AMPERSAND = "mdi:ampersand"

DEPENDENCIES = ["acurite_hub"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ACURITE_HUB_ID): cv.use_id(AcuriteHub),
        cv.Optional(CONF_DATA): text_sensor.text_sensor_schema(
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_AMPERSAND,
            ),
        cv.Optional(CONF_SENSOR_BATTERY): text_sensor.text_sensor_schema(
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_BATTERY_CHARGING_WIRELESS,
            ),
        cv.Optional(CONF_BATTERY): text_sensor.text_sensor_schema(
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_BATTERY,
            ),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    acurite_hub = await cg.get_variable(config[CONF_ACURITE_HUB_ID])

    if data := config.get(CONF_DATA):
        s = await text_sensor.new_text_sensor(data)
        cg.add(acurite_hub.set_data_text_sensor(s))

    if sensor_battery := config.get(CONF_SENSOR_BATTERY):
        s = await text_sensor.new_text_sensor(sensor_battery)
        cg.add(acurite_hub.set_sensor_battery_text_sensor(s))
        
    if battery := config.get(CONF_BATTERY):
        s = await text_sensor.new_text_sensor(battery)
        cg.add(acurite_hub.set_battery_text_sensor(s))
