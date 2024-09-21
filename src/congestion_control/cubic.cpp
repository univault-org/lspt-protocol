#include "cubic.h"
#include <algorithm>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <iostream>

namespace LSPT {
namespace CongestionControl {

Cubic::Cubic()
    : cwnd_(INITIAL_WINDOW), ssthresh_(UINT32_MAX), w_max_(INITIAL_WINDOW), last_max_cwnd_(0),
      k_(0), in_slow_start_(true), bytes_in_flight_(0), pacing_rate_(std::chrono::microseconds(1000000)) {
    last_congestion_ = std::chrono::steady_clock::now();
    last_sent_ = last_congestion_;
    last_send_time_ = last_congestion_;
    next_send_time_ = last_congestion_;
}

void Cubic::onPacketSent(uint32_t packetSize) {
    handleIdlePeriod();
    if (canSendPacket(packetSize)) {
        last_sent_ = std::chrono::steady_clock::now();
        bytes_in_flight_ += packetSize;
    }
}

void Cubic::onAckReceived(uint32_t ackedBytes, std::chrono::milliseconds rtt) {
    bytes_in_flight_ = (bytes_in_flight_ > ackedBytes) ? bytes_in_flight_ - ackedBytes : 0;
    if (in_slow_start_) {
        cwnd_ += std::min(ackedBytes, INITIAL_WINDOW);
        if (cwnd_ >= ssthresh_) {
            in_slow_start_ = false;
        }
    } else {
        updateCubic(rtt);
    }
    updatePacingRate();
}

void Cubic::onPacketLoss() {
    uint32_t previous_w_max = w_max_;
    fastConvergence();
    ssthresh_ = std::max(static_cast<uint32_t>(cwnd_ * BETA_CUBIC), 2U * INITIAL_WINDOW);
    cwnd_ = ssthresh_;
    k_ = std::cbrt((w_max_ * (1 - BETA_CUBIC)) / C);
    last_congestion_ = std::chrono::steady_clock::now();
    in_slow_start_ = false;
}

uint32_t Cubic::getCongestionWindow() const {
    return cwnd_;
}

uint32_t Cubic::getSendingRate() const {
    auto rtt = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - last_congestion_);
    return static_cast<uint32_t>((static_cast<uint64_t>(cwnd_) * 1000000) / rtt.count());
}

bool Cubic::canSendPacket(uint32_t packet_size) {
    auto now = std::chrono::steady_clock::now();
    bool time_condition = now >= next_send_time_;
    bool bytes_condition = bytes_in_flight_ + packet_size <= cwnd_ || bytes_in_flight_ == 0;
    
    if (time_condition && bytes_condition) {
        next_send_time_ = now + pacing_rate_;
        bytes_in_flight_ += packet_size;
        return true;
    }
    
    return false;
}

void Cubic::updateCubic(std::chrono::milliseconds rtt) {
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - last_congestion_);
    
    rtt = std::clamp(rtt, MIN_RTT, MAX_RTT);
    
    uint32_t new_cwnd = cubicUpdate(elapsed);
    uint32_t tcp_cwnd = w_max_ * BETA_CUBIC + 3 * (1 - BETA_CUBIC) * (elapsed.count() / rtt.count());
    
    double rtt_factor = std::sqrt(static_cast<double>(last_rtt_.count()) / rtt.count());
    uint32_t rtt_cwnd = static_cast<uint32_t>(cwnd_ * std::min(rtt_factor, 1.2));
    
    uint32_t min_increment = std::max(1U, cwnd_ / CWND_INCREASE_FACTOR);
    uint32_t limited_cwnd = cwnd_ + min_increment;
    
    cwnd_ = std::max({new_cwnd, tcp_cwnd, limited_cwnd, rtt_cwnd, cwnd_ + 1});
    w_max_ = std::min(std::max(w_max_, cwnd_), last_max_cwnd_);  // Limit w_max_ growth
    
    last_rtt_ = rtt;
}

uint32_t Cubic::cubicUpdate(std::chrono::milliseconds elapsed) {
    double t = std::max(0.001, elapsed.count() / 1000.0); // Ensure t is not zero, minimum 1ms
    double w_cubic = C * std::pow(t - k_, 3) + w_max_;
    return static_cast<uint32_t>(std::max(w_cubic, static_cast<double>(INITIAL_WINDOW)));
}

void Cubic::enterCongestionAvoidance() {
    in_slow_start_ = false;
    w_max_ = cwnd_;
    k_ = 0;
    last_congestion_ = std::chrono::steady_clock::now();
}

void Cubic::updatePacingRate() {
    auto rtt = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - last_congestion_);
    
    rtt = std::max(rtt, std::chrono::microseconds(1000)); // Minimum RTT of 1ms
    
    uint64_t target_rate = static_cast<uint64_t>(cwnd_) * 1000000 / rtt.count();
    uint64_t pacing_rate = target_rate / 2; // Pace at half the target rate for more visible pacing
    
    pacing_rate_ = std::chrono::microseconds(1000000 / std::max(pacing_rate, static_cast<uint64_t>(1)));
}

void Cubic::handleIdlePeriod() {
    auto now = std::chrono::steady_clock::now();
    auto idle_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_sent_);
    
    if (idle_time > std::chrono::seconds(1)) {
        cwnd_ = INITIAL_WINDOW;
        ssthresh_ = UINT32_MAX;
        w_max_ = 0;
        k_ = 0;
        last_congestion_ = now;
        in_slow_start_ = true;
    }
}

void Cubic::fastConvergence() {
    if (w_max_ < last_max_cwnd_) {
        last_max_cwnd_ = w_max_;
        w_max_ = w_max_ * (1.0 + BETA_CUBIC) / 2.0;
    } else {
        last_max_cwnd_ = w_max_;
        w_max_ = w_max_ * (2.0 - BETA_CUBIC) / 2.0;
    }
}

} // namespace CongestionControl
} // namespace LSPT