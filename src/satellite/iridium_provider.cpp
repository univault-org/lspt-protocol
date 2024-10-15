#include "../../include/satellite/iridium_provider.h"
#include "../../include/srpt_satellite.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <deque>

namespace SRPT {
namespace Satellite {

// Add this member variable to the IridiumProvider class

bool IridiumProvider::Initialize(const std::map<std::string, std::string>& options) {
    // Implement Iridium-specific initialization
    return true;
}

bool IridiumProvider::Connect(const std::string& satellite_id) {
    // Implement Iridium connection logic
    return true;
}

bool IridiumProvider::Disconnect() {
    // Implement Iridium disconnection logic
    return true;
}

bool IridiumProvider::SendData(const ByteVector& data) {
    m_dataQueue.push_back(data);
    if(m_verboseLogging) {
        std::cout << "IridiumProvider::SendData - Data sent: ";
        for (const auto& byte : data) {
            std::cout << static_cast<int>(byte) << " ";
        }
       std::cout << std::endl;
    }
    return true;
}

bool IridiumProvider::ReceiveData(ByteVector& data) {
    if (m_dataQueue.empty()) {
        data.clear();
        return false;
    }
    data = m_dataQueue.front();
    m_dataQueue.pop_front();
    std::cout << "IridiumProvider::ReceiveData - Data received: ";
    for (const auto& byte : data) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
    return true;
}

bool IridiumProvider::ExecuteCommand(const std::string& command, std::string& response) {
    // Implement Iridium-specific command execution logic
    response = "Iridium command executed: " + command;
    return true;
}

double IridiumProvider::GetSignalStrength() const {
    // Implement signal strength measurement
    return 0.0;
}

double IridiumProvider::GetLatency() const {
    // Implement latency measurement
    return 0.0;
}

uint64_t IridiumProvider::GetBandwidth() const {
    // Implement bandwidth measurement
    return 0;
}

std::unique_ptr<SatelliteStream> IridiumProvider::CreateStream() {
    return std::make_unique<IridiumStream>(*this);
}

// IridiumStream implementation
bool IridiumProvider::IridiumStream::Write(const ByteVector& data) {
    return m_provider.SendData(data);
}

bool IridiumProvider::IridiumStream::Read(ByteVector& data) {
    return m_provider.ReceiveData(data);
}

void IridiumProvider::IridiumStream::Close() {
    // Implement Iridium-specific close logic
}

// Add this constructor to IridiumStream
IridiumProvider::IridiumStream::IridiumStream(IridiumProvider& provider) : m_provider(provider) {}

// Factory function implementation
std::unique_ptr<ISatelliteProvider> CreateIridiumProvider() {
    return std::make_unique<IridiumProvider>();
}

// Register the Iridium provider
namespace {
    __attribute__((used)) static bool registered = RegisterSatelliteProvider(Provider::IRIDIUM, CreateIridiumProvider);
}

} // namespace Satellite
} // namespace SRPT
