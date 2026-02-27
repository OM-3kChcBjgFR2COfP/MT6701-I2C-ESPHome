import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

from . import MT6701Component

DEPENDENCIES = ["mt6701"]

CONF_MT6701_ID = "mt6701_id"
CONF_ANGLE = "angle"
CONF_RAW = "raw"
CONF_STATUS = "status"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_MT6701_ID): cv.use_id(MT6701Component),

        cv.Optional(CONF_ANGLE): sensor.sensor_schema(
            unit_of_measurement="°",
            accuracy_decimals=1,
            state_class="measurement",
        ),

        cv.Optional(CONF_RAW): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class="measurement",
        ),

        cv.Optional(CONF_STATUS): sensor.sensor_schema(
            accuracy_decimals=0,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MT6701_ID])

    if CONF_ANGLE in config:
        sens = await sensor.new_sensor(config[CONF_ANGLE])
        cg.add(parent.set_angle_sensor(sens))

    if CONF_RAW in config:
        sens = await sensor.new_sensor(config[CONF_RAW])
        cg.add(parent.set_raw_sensor(sens))

    if CONF_STATUS in config:
        sens = await sensor.new_sensor(config[CONF_STATUS])
        cg.add(parent.set_status_sensor(sens))