#include <gtest/gtest.h>
#include "network/local/discovery.h"
#include <thread>
#include <chrono>

using namespace lspt::network::local;

class DiscoveryTest : public ::testing::Test {
protected:
    Discovery discovery;
};

TEST_F(DiscoveryTest, StartDiscovery) {
    EXPECT_TRUE(discovery.startDiscovery());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(discovery.isDiscoveryActive());
}

TEST_F(DiscoveryTest, StartDiscoveryTwice) {
    EXPECT_TRUE(discovery.startDiscovery());
    EXPECT_FALSE(discovery.startDiscovery());
}

TEST_F(DiscoveryTest, GetDiscoveredNodes) {
    discovery.startDiscovery();
    std::this_thread::sleep_for(std::chrono::milliseconds(11000));  // Wait for discovery to complete
    auto nodes = discovery.getDiscoveredNodes();
    EXPECT_FALSE(nodes.empty());
    EXPECT_EQ(nodes.size(), 3);
}

TEST_F(DiscoveryTest, StopDiscovery) {
    discovery.startDiscovery();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    discovery.stopDiscovery();
    EXPECT_FALSE(discovery.isDiscoveryActive());
    auto nodes = discovery.getDiscoveredNodes();
    EXPECT_EQ(nodes.size(), 3);
}

TEST_F(DiscoveryTest, GetDiscoveredNodesBeforeStart) {
    auto nodes = discovery.getDiscoveredNodes();
    EXPECT_TRUE(nodes.empty());
}

TEST_F(DiscoveryTest, ConcurrentDiscovery) {
    std::thread t1([&]() { discovery.startDiscovery(); });
    std::thread t2([&]() { discovery.startDiscovery(); });
    t1.join();
    t2.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(11000));  // Wait for discovery to complete
    auto nodes = discovery.getDiscoveredNodes();
    EXPECT_EQ(nodes.size(), 3);
}

TEST_F(DiscoveryTest, DiscoveryTimeout) {
    discovery.setTimeout(std::chrono::milliseconds(100));
    discovery.startDiscovery();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_FALSE(discovery.isDiscoveryActive());
}
