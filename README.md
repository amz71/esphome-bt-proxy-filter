# esphome-bt-proxy-filter

Blacklist / whitelist for the ESPHome `bluetooth_proxy`, as a standalone
external component — **no patch to ESPHome core**.

Since ESPHome **2026.10.0**, `bluetooth_proxy` provides an official advertisement
filter hook (`enable_advertisement_filter()` / `set_advertisement_filter()`,
marked as "Public API for external components"). This component plugs into that
hook, so it survives ESPHome updates without re-patching anything.

## Usage

```yaml
external_components:
  - source: github://amz71/esphome-bt-proxy-filter
    components: [bluetooth_proxy_filter]

esp32_ble_tracker:

bluetooth_proxy:

bluetooth_proxy_filter:
  blacklist:                 # advertisements from these devices are dropped
    - "AA:BB:CC:DD:EE:01"
  whitelist:                 # optional: if set, ONLY these devices are forwarded
    - "AA:BB:CC:DD:EE:02"    #           (blacklist is ignored then)
```

| Option      | Description |
|-------------|-------------|
| `blacklist` | List of MAC addresses whose advertisements are not forwarded to Home Assistant. |
| `whitelist` | List of MAC addresses. If non-empty, only these devices are forwarded and `blacklist` is ignored. |
| `bluetooth_proxy_id` | Optional, ID of the `bluetooth_proxy` to attach to (auto-detected). |

Requirements: ESPHome >= 2026.10.0, a configured `bluetooth_proxy`.

The active mode and address lists are printed in the device log at boot
(`Bluetooth Proxy Filter:` section).
