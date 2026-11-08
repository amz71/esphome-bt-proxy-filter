#pragma once

#include "esphome/core/defines.h"

#ifdef USE_BLUETOOTH_PROXY

#include <algorithm>
#include <cstdint>
#include <vector>

#include "esphome/components/bluetooth_proxy/bluetooth_proxy.h"
#include "esphome/core/component.h"

namespace esphome::bluetooth_proxy_filter {

/// Drops advertisements before bluetooth_proxy forwards them to the API.
/// - whitelist non-empty: only whitelisted addresses are forwarded (blacklist ignored)
/// - otherwise: blacklisted addresses are dropped
class BluetoothProxyFilter : public Component {
 public:
  void add_blacklist(uint64_t address) { insert_sorted_(this->blacklist_, address); }
  void add_whitelist(uint64_t address) { insert_sorted_(this->whitelist_, address); }

  void attach(bluetooth_proxy::BluetoothProxy *proxy) {
    proxy->set_advertisement_filter({this, [](void *self, const ble_device_base::RawAdvertisement &adv) {
                                       return static_cast<BluetoothProxyFilter *>(self)->should_forward(adv.address);
                                     }});
  }

  bool should_forward(uint64_t address) const {
    if (!this->whitelist_.empty())
      return std::binary_search(this->whitelist_.begin(), this->whitelist_.end(), address);
    return !std::binary_search(this->blacklist_.begin(), this->blacklist_.end(), address);
  }

  void dump_config() override;

 protected:
  static void insert_sorted_(std::vector<uint64_t> &list, uint64_t address) {
    auto it = std::lower_bound(list.begin(), list.end(), address);
    if (it == list.end() || *it != address)
      list.insert(it, address);
  }

  std::vector<uint64_t> blacklist_{};
  std::vector<uint64_t> whitelist_{};
};

}  // namespace esphome::bluetooth_proxy_filter

#endif  // USE_BLUETOOTH_PROXY
