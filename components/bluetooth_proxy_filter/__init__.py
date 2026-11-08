"""Blacklist/whitelist filter for ESPHome's bluetooth_proxy.

Uses the public advertisement-filter hook of bluetooth_proxy
(ESPHome >= 2026.10.0), so the core component stays untouched.

    bluetooth_proxy:

    bluetooth_proxy_filter:
      blacklist:
        - "AA:BB:CC:DD:EE:01"
      whitelist:            # if non-empty: ONLY these are forwarded,
        - "AA:BB:CC:DD:EE:02"  # blacklist is then ignored
"""

import esphome.codegen as cg
from esphome.components import bluetooth_proxy
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["bluetooth_proxy"]
CODEOWNERS = []

CONF_BLUETOOTH_PROXY_ID = "bluetooth_proxy_id"
CONF_BLACKLIST = "blacklist"
CONF_WHITELIST = "whitelist"

bluetooth_proxy_filter_ns = cg.esphome_ns.namespace("bluetooth_proxy_filter")
BluetoothProxyFilter = bluetooth_proxy_filter_ns.class_(
    "BluetoothProxyFilter", cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BluetoothProxyFilter),
        cv.GenerateID(CONF_BLUETOOTH_PROXY_ID): cv.use_id(
            bluetooth_proxy.BluetoothProxy
        ),
        cv.Optional(CONF_BLACKLIST, default=[]): cv.ensure_list(cv.mac_address),
        cv.Optional(CONF_WHITELIST, default=[]): cv.ensure_list(cv.mac_address),
    }
).extend(cv.COMPONENT_SCHEMA)


def _mac_to_uint64(mac) -> int:
    """MAC "AA:BB:CC:DD:EE:FF" -> 0xAABBCCDDEEFF (same as esp32_ble::ble_addr_to_uint64)."""
    return int(str(mac).replace(":", ""), 16)


async def to_code(config):
    # Compiles the filter hook into bluetooth_proxy (public API).
    bluetooth_proxy.enable_advertisement_filter()

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for mac in config[CONF_WHITELIST]:
        cg.add(var.add_whitelist(cg.RawExpression(f"0x{_mac_to_uint64(mac):012X}ULL")))
    for mac in config[CONF_BLACKLIST]:
        cg.add(var.add_blacklist(cg.RawExpression(f"0x{_mac_to_uint64(mac):012X}ULL")))

    proxy = await cg.get_variable(config[CONF_BLUETOOTH_PROXY_ID])
    # Registers the filter right away (before any component setup()).
    cg.add(var.attach(proxy))
