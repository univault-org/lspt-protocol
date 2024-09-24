#include <gtest/gtest.h>
#include "../../src/congestion_control/cubic.h"
#include <thread>
#include <chrono>
#include <stdexcept>

using namespace SRPT::CongestionControl;

class CubicTest : public ::testing::Test, public Cubic {
protected:
    // Add any additional setup or teardown here if needed
};

class TestTimeout : public std::runtime_error {
public:
    TestTimeout() : std::runtime_error("Test timed out") {}
};

#define RUN_WITH_TIMEOUT(statement, timeout_ms) \
    { \
        auto start = std::chrono::steady_clock::now(); \
        statement; \
        auto end = std::chrono::steady_clock::now(); \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        if (duration.count() > timeout_ms) { \
            throw TestTimeout(); \
        } \
    }

TEST_F(CubicTest, InitialState) {
    EXPECT_EQ(getCongestionWindow(), INITIAL_WINDOW);
    EXPECT_EQ(getSsthresh(), UINT32_MAX);
}

TEST_F(CubicTest, SlowStart) {
    uint32_t initial_cwnd = getCongestionWindow();
    for (int i = 0; i < 5; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    EXPECT_GT(getCongestionWindow(), initial_cwnd);
    EXPECT_LE(getCongestionWindow(), getSsthresh());
}

TEST_F(CubicTest, CongestionAvoidance) {
    // Set a lower ssthresh to ensure we exit slow start
    setSSThresh(INITIAL_WINDOW * 2);
    
    // Force exit from slow start
    while (getCongestionWindow() < getSsthresh()) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    
    // We should now be in congestion avoidance
    uint32_t cwnd_before = getCongestionWindow();
    for (int i = 0; i < 10; ++i) {  // Run multiple iterations to ensure growth
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    uint32_t cwnd_after = getCongestionWindow();
    
    EXPECT_GT(cwnd_after, cwnd_before) << "Congestion window should increase in congestion avoidance";
    EXPECT_LT(cwnd_after, cwnd_before + 1460 * 10) << "Congestion window should increase by less than one full packet per ACK in congestion avoidance";
}

TEST_F(CubicTest, PacketLoss) {
    // Grow the window
    for (int i = 0; i < 1000; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    uint32_t cwnd_before_loss = getCongestionWindow();
    uint32_t w_max_before_loss = getWMax();
    
    onPacketLoss();
    
    EXPECT_LT(getCongestionWindow(), cwnd_before_loss);
    EXPECT_GE(getCongestionWindow(), static_cast<uint32_t>(cwnd_before_loss * BETA_CUBIC));
    EXPECT_LE(getWMax(), w_max_before_loss);
}

TEST_F(CubicTest, PacketPacing) {
    // Grow the window and update pacing rate
    for (int i = 0; i < 10; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    
    auto start = std::chrono::steady_clock::now();
    int packets_sent = 0;
    for (int i = 0; i < 1000; ++i) {
        if (canSendPacket(1460)) {
            packets_sent++;
            onPacketSent(1460);
        }
        
        // Simulate acknowledging packets
        if (i % 10 == 0) {
            onAckReceived(1460, std::chrono::milliseconds(100));
        }
        
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        
        if (i % 100 == 0) {
            std::cout << "Iteration " << i << ", Packets sent: " << packets_sent << std::endl;
        }
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    
    EXPECT_LT(packets_sent, 1000);
    EXPECT_GT(duration.count(), 0);
    EXPECT_GT(packets_sent, 10);
}

TEST_F(CubicTest, IdlePeriodHandling) {
    // Grow the window
    for (int i = 0; i < 100; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    uint32_t cwnd_before_idle = getCongestionWindow();
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate idle period
    onPacketSent(1460);
    EXPECT_EQ(getCongestionWindow(), INITIAL_WINDOW);
    EXPECT_EQ(getSsthresh(), UINT32_MAX);
    EXPECT_EQ(getWMax(), 0);
}

TEST_F(CubicTest, FastConvergence) {
    // Grow the window
    for (int i = 0; i < 1000; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    uint32_t w_max_before = getWMax();

    onPacketLoss();
    uint32_t w_max_after_first_loss = getWMax();
    EXPECT_LT(w_max_after_first_loss, w_max_before) << "w_max should decrease after first loss";
    
    // Grow the window again, but not as large as before
    for (int i = 0; i < 500; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    uint32_t w_max_before_second_loss = getWMax();
    EXPECT_GT(w_max_before_second_loss, w_max_after_first_loss) << "w_max should increase after recovery";
    EXPECT_LE(w_max_before_second_loss, w_max_before) << "w_max should not exceed previous max";
    
    onPacketLoss();
    uint32_t w_max_after_second_loss = getWMax();
    EXPECT_LT(w_max_after_second_loss, w_max_before_second_loss) << "w_max should decrease after second loss";
}

TEST_F(CubicTest, LongRTTHandling) {
    uint32_t cwnd_before = INITIAL_WINDOW;
    setCwnd(cwnd_before);
    onAckReceived(1460, std::chrono::milliseconds(2000)); // Very long RTT
    uint32_t cwnd_after = getCongestionWindow();
    EXPECT_GT(cwnd_after, cwnd_before);
    EXPECT_LE(cwnd_after, cwnd_before * 2);  // Allow at most doubling
}

TEST_F(CubicTest, BandwidthFluctuation) {
    // Start with normal conditions
    for (int i = 0; i < 50; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    uint32_t cwnd_normal = getCongestionWindow();

    // Simulate bandwidth drop (longer RTT, some packet loss)
    onPacketLoss();
    for (int i = 0; i < 20; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(200));
    }
    uint32_t cwnd_low = getCongestionWindow();

    // Simulate bandwidth increase (shorter RTT, no loss)
    for (int i = 0; i < 50; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(50));
    }
    uint32_t cwnd_high = getCongestionWindow();

    // Check that CUBIC responded appropriately to network changes
    EXPECT_LT(cwnd_low, cwnd_normal) << "CUBIC should reduce window when bandwidth drops";
    EXPECT_GT(cwnd_high, cwnd_low * 1.1) << "CUBIC should increase window when bandwidth improves";
    EXPECT_LT(cwnd_high, cwnd_normal * 2) << "CUBIC shouldn't overreact to bandwidth increase";

    // Simulate return to normal conditions for a longer period
    for (int i = 0; i < 200; ++i) {
        onAckReceived(1460, std::chrono::milliseconds(100));
    }
    uint32_t cwnd_final = getCongestionWindow();

    // Check that CUBIC converges back towards normal window size
    EXPECT_NEAR(cwnd_final, cwnd_normal, cwnd_normal * 0.5) 
        << "CUBIC should converge back to near normal window size";

}

// Add more tests as needed
