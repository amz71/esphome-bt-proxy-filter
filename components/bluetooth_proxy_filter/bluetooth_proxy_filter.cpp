#include "bluetooth_proxy_filter.h"

#ifdef USE_BLUETOOTH_PROXY

#include <cinttypes>

#include "esphome/core/log.h"

namespace esphome::bluetooth_proxy_filter {

static const char *const TAG = "bluetooth_proxy_filter";

static void dump_list(const char *name, const std::vector<uint64_t> &list) {
  ESP_LOGCONFIG(TAG, "  %s (%u):", name, static_cast<unsigned>(list.size()));
  for (uint64_t a : list) {
    ESP_LOGCONFIG(TAG, "    %02X:%02X:%02X:%02X:%02X:%02X", (unsigned) (a >> 40) & 0xFF, (unsigned) (a >> 32) & 0xFF,
                  (unsigned) (a >> 24) & 0xFF, (unsigned) (a >> 16) & 0xFF, (unsigned) (a >> 8) & 0xFF,
                  (unsigned) a & 0xFF);
  }
}

void BluetoothProxyFilter::dump_config() {
  ESP_LOGCONFIG(TAG, "Bluetooth Proxy Filter:");
  if (!this->whitelist_.empty()) {
    ESP_LOGCONFIG(TAG, "  Mode: whitelist (only listed devices are forwarded)");
    dump_list("Whitelist", this->whitelist_);
    if (!this->blacklist_.empty())
      ESP_LOGCONFIG(TAG, "  Blacklist (%u entries) ignored because whitelist is set",
                    static_cast<unsigned>(this->blacklist_.size()));
  } else {
    ESP_LOGCONFIG(TAG, "  Mode: blacklist");
    dump_list("Blacklist", this->blacklist_);
  }
}

}  // namespace esphome::bluetooth_proxy_filter

#endif  // USE_BLUETOOTH_PROXY
