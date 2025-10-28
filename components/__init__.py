import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PORT
from esphome.core import coroutine_with_priority

DEPENDENCIES = ["mipi_dsi_cam", "network"]
CODEOWNERS = ["@youkorr"]

CONF_CAMERA_ID = "camera_id"

esp32p4_rtsp_ns = cg.esphome_ns.namespace("esp32p4_rtsp")
Esp32P4RTSP = esp32p4_rtsp_ns.class_("Esp32P4RTSP", cg.Component)

mipi_dsi_cam_ns = cg.esphome_ns.namespace("mipi_dsi_cam")
MipiDsiCam = mipi_dsi_cam_ns.class_("MipiDsiCam")

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Esp32P4RTSP),
        cv.Required(CONF_CAMERA_ID): cv.use_id(MipiDsiCam),
        cv.Optional(CONF_PORT, default=8554): cv.port,
    }
).extend(cv.COMPONENT_SCHEMA)

@coroutine_with_priority(60.0)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cam = await cg.get_variable(config[CONF_CAMERA_ID])
    cg.add(var.set_camera(cam))
    cg.add(var.set_port(config[CONF_PORT]))

    cg.add_library("esp_h264", None)
    cg.add_build_flag("-DBOARD_HAS_PSRAM")
