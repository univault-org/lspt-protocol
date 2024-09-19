#pragma once

#include "interface.h"
#include <chrono>
#include <cstdint>

namespace LSPT {
namespace CongestionControl {

class Cubic : public ICongestionControl {
public:
    static constexpr uint32_t INITIAL_WINDOW = 10; // In packets
    static constexpr double BETA_CUBIC = 0.7;

    Cubic();
    void onPacketSent(uint32_t packetSize) override;
    void onAckReceived(uint32_t ackedBytes, std::chrono::milliseconds rtt) override;
    void onPacketLoss() override;
    uint32_t getCongestionWindow() const override;
    uint32_t getSendingRate() const override;
    bool canSendPacket(uint32_t packet_size);
    
    uint32_t getBytes_in_flight() const { return bytes_in_flight_; }

    // New method to set SSThresh
    void setSSThresh(uint32_t new_ssthresh) { ssthresh_ = new_ssthresh; }

    // New method to set congestion window
    void setCwnd(uint32_t new_cwnd) { cwnd_ = new_cwnd; }

protected:
    // Protected getters for testing
    uint32_t getSsthresh() const { return ssthresh_; }
    uint32_t getWMax() const { return w_max_; }

private:
    static constexpr double C = 0.4;
    static constexpr std::chrono::milliseconds MAX_RTT{1000}; // 1 second
    static constexpr std::chrono::milliseconds MIN_RTT{10};   // 10 milliseconds

    uint32_t cwnd_;           // Congestion window
    uint32_t ssthresh_;       // Slow start threshold
    uint32_t w_max_;          // Window size before last reduction
    uint32_t last_max_cwnd_;  // Last maximum window size for fast convergence
    std::chrono::steady_clock::time_point last_congestion_; // Time of last congestion event
    std::chrono::steady_clock::time_point last_sent_;       // Time of last packet sent
    uint32_t k_;              // Time period for window growth
    bool in_slow_start_;      // Flag for slow start phase
    uint32_t bytes_in_flight_;
    std::chrono::steady_clock::time_point last_send_time_;
    std::chrono::microseconds pacing_rate_;
    std::chrono::steady_clock::time_point next_send_time_;
    std::chrono::milliseconds last_rtt_{100};  // Initialize with a default value

    static constexpr uint32_t CWND_INCREASE_FACTOR = 1000;

    void updateCubic(std::chrono::milliseconds rtt);
    uint32_t cubicUpdate(std::chrono::milliseconds elapsed);
    void enterCongestionAvoidance();
    void updatePacingRate();
    void handleIdlePeriod();
    void fastConvergence();
};

} // namespace CongestionControl
} // namespace LSPT