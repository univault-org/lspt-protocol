#pragma once

#include <cstdint>
#include <chrono>

namespace LSPT {
namespace CongestionControl {

class ICongestionControl {
public:
    virtual ~ICongestionControl() = default;

    virtual void onPacketSent(uint32_t packetSize) = 0;
    virtual void onAckReceived(uint32_t ackedBytes, std::chrono::milliseconds rtt) = 0;
    virtual void onPacketLoss() = 0;
    virtual uint32_t getCongestionWindow() const = 0;
    virtual uint32_t getSendingRate() const = 0;
};

} // namespace CongestionControl
} // namespace LSPT