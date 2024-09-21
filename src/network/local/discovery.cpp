#include "discovery.h"
#include <algorithm>
#include <chrono>

namespace lspt {
namespace network {
namespace local {

Discovery::Discovery() : m_isDiscoveryActive(false), timeout(std::chrono::seconds(10)) {}

Discovery::~Discovery() {
    stopDiscovery();
}

bool Discovery::startDiscovery() {
    if (!m_isDiscoveryActive.exchange(true)) {
        discoveryThread = std::thread(&Discovery::simulateDiscovery, this);
        return true;
    }
    return false;
}

void Discovery::stopDiscovery() {
    m_isDiscoveryActive.store(false);
    if (discoveryThread.joinable()) {
        discoveryThread.join();
    }
}

std::vector<std::string> Discovery::getDiscoveredNodes() const {
    std::lock_guard<std::mutex> lock(discoveredNodesMutex);
    return discoveredNodes;
}

void Discovery::setTimeout(std::chrono::milliseconds newTimeout) {
    timeout = newTimeout;
}

bool Discovery::isDiscoveryActive() const {
    return m_isDiscoveryActive.load();
}

void Discovery::simulateDiscovery() {
    auto start = std::chrono::steady_clock::now();
    while (m_isDiscoveryActive.load() && 
           std::chrono::steady_clock::now() - start < timeout) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    {
        std::lock_guard<std::mutex> lock(discoveredNodesMutex);
        discoveredNodes = {"Node1", "Node2", "Node3"};
    }
    m_isDiscoveryActive.store(false);
}

} // namespace local
} // namespace network
} // namespace lspt