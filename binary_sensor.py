import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID

from . import mt6701_ns, MT6701Component

DEPENDENCIES = ["mt6701"]

CONF_PARENT_ID = "mt6701_id"

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema().extend(
    {
        cv.GenerateID(): cv.declare_id(binary_sensor.BinarySensor),
        cv.Required(CONF_PARENT_ID): cv.use_id(MT6701Component),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PARENT_ID])

    var = cg.new_Pvariable(config[CONF_ID])
    await binary_sensor.register_binary_sensor(var, config)

    cg.add(parent.set_zero_binary_sensor(var))