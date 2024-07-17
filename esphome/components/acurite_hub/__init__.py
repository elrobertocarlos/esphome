import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart", "wifi"]

AUTO_LOAD = ["sensor", "text_sensor"]

acurite_ns = cg.esphome_ns.namespace("acurite")
AcuriteHub = acurite_ns.class_("AcuriteHub", cg.Component, uart.UARTDevice)

CONF_ACURITE_HUB_ID = "acurite_hub_id"
CONF_BUFFER_SIZE = "buffer_size"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AcuriteHub),
            cv.Optional(CONF_BUFFER_SIZE, default=256): cv.validate_bytes,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_buffer_size(config[CONF_BUFFER_SIZE]))
