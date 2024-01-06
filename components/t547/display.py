import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA,
    CONF_PAGES,
    CONF_FULL_UPDATE_EVERY,
)
from esphome.const import __version__ as ESPHOME_VERSION

DEPENDENCIES = ["esp32"]

CONF_GREYSCALE = "greyscale"
CONF_QUICK_UPDATING = "quick_updating"

t547_ns = cg.esphome_ns.namespace("t547")
T547 = t547_ns.class_(
    "T547", cg.PollingComponent, display.DisplayBuffer
)

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(T547),
            cv.Optional(CONF_GREYSCALE, default=False): cv.boolean,
            cv.Optional(CONF_FULL_UPDATE_EVERY, default=5): cv.uint32_t,
            cv.Optional(CONF_QUICK_UPDATING, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("5s")),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
    cv.only_with_arduino,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if cv.Version.parse(ESPHOME_VERSION) < cv.Version.parse("2023.12.0"):
        await cg.register_component(var, config)
    await display.register_display(var, config)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

    cg.add(var.set_greyscale(config[CONF_GREYSCALE]))
    cg.add(var.set_quick_updating(config[CONF_QUICK_UPDATING]))
    cg.add(var.set_full_update_every(config[CONF_FULL_UPDATE_EVERY]))
    cg.add_build_flag("-DBOARD_HAS_PSRAM")

    cg.add_library("Wire", version="2.0.0")  # required by LilyGoEPD47
    cg.add_library("LilyGoEPD47", repository="https://github.com/Xinyuan-LilyGO/LilyGo-EPD47", version="v0.3.0")
