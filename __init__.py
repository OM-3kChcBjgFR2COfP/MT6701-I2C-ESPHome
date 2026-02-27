import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor", "binary_sensor"]

mt6701_ns = cg.esphome_ns.namespace("mt6701")

MT6701Component = mt6701_ns.class_(
    "MT6701Component",
    cg.PollingComponent,
    i2c.I2CDevice,
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MT6701Component),
        }
    )
    .extend(cv.polling_component_schema("20ms"))
    .extend(i2c.i2c_device_schema(0x06))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)