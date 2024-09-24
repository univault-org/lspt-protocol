#include "../../include/srpt_satellite.h"
#include "iridium_provider.h"  
#include "starlink_provider.h"  
#include "starlink_provider.cpp"
#include "iridium_provider.cpp"
#include <stdexcept>
#include <iostream> 

namespace SRPT {
namespace Satellite {

class SatelliteSession::Impl {
public:
    Impl(const SatelliteConfig& config) : config_(config) {
        std::cout << "SatelliteSession::Impl constructor called" << std::endl;
        std::cout << "Provider from config: " 
                  << (config_.getProvider() == Provider::STARLINK ? "STARLINK" : "IRIDIUM") << std::endl;
        provider_ = CreateSatelliteProvider(config_.getProvider());
        if (!provider_) {
            throw std::runtime_error("Failed to create satellite provider");
        }
        std::cout << "Provider created successfully" << std::endl;
    }
    
    bool Connect(const std::string& satellite_id);
    bool Disconnect();
    std::unique_ptr<SatelliteStream> CreateSatelliteStream();
    bool ExecuteProviderCommand(const std::string& command, std::string& response);
    double GetSignalStrength() const;
    double GetLatency() const;
    uint64_t GetBandwidth() const;

private:
    std::unique_ptr<ISatelliteProvider> provider_;
    SatelliteConfig config_;
};

// SatelliteSession method implementations
SatelliteSession::SatelliteSession(const SatelliteConfig& config) : pImpl(std::make_unique<Impl>(config)) {}

SatelliteSession::~SatelliteSession() = default;

bool SatelliteSession::Connect(const std::string& satellite_id) {
    return pImpl->Connect(satellite_id);
}

bool SatelliteSession::Disconnect() {
    return pImpl->Disconnect();
}

std::unique_ptr<SatelliteStream> SatelliteSession::CreateSatelliteStream() {
    return pImpl->CreateSatelliteStream();
}

// bool SatelliteSession::ExecuteProviderCommand(const std::string& command, std::string& response) {
//     if (!m_impl->m_provider) {
//         return false;
//     }
//     return m_impl->m_provider->ExecuteCommand(command, response);
// }

double SatelliteSession::GetSignalStrength() const {
    return pImpl->GetSignalStrength();
}

double SatelliteSession::GetLatency() const {
    return pImpl->GetLatency();
}

uint64_t SatelliteSession::GetBandwidth() const {
    return pImpl->GetBandwidth();
}

// Implementation of SatelliteSession::Impl methods
bool SatelliteSession::Impl::Connect(const std::string& satellite_id) {
    return provider_->Connect(satellite_id);
}

bool SatelliteSession::Impl::Disconnect() {
    return provider_->Disconnect();
}

std::unique_ptr<SatelliteStream> SatelliteSession::Impl::CreateSatelliteStream() {
    // Implementation details...
    // Removed BasicSatelliteStream as it's not defined
    return provider_->CreateStream();  // Assuming this method exists in ISatelliteProvider
}

bool SatelliteSession::Impl::ExecuteProviderCommand(const std::string& command, std::string& response) {
    return provider_->ExecuteCommand(command, response);
}

double SatelliteSession::Impl::GetSignalStrength() const {
    return provider_->GetSignalStrength();
}

double SatelliteSession::Impl::GetLatency() const {
    return provider_->GetLatency();
}

uint64_t SatelliteSession::Impl::GetBandwidth() const {
    return provider_->GetBandwidth();
}

// Other function implementations
std::unique_ptr<SatelliteSession> CreateSatelliteSession(const SatelliteConfig& config) {
    return std::make_unique<SatelliteSession>(config);
}

SatelliteError GetLastSatelliteError() {
    // Implementation
    return SatelliteError::NO_ERROR;
}

std::unique_ptr<ISatelliteProvider> CreateSatelliteProvider(Provider provider) {
    std::cout << "CreateSatelliteProvider called with provider: " 
              << (provider == Provider::STARLINK ? "STARLINK" : "IRIDIUM") << std::endl;
    switch (provider) {
        case Provider::STARLINK:
            std::cout << "Creating StarlinkProvider" << std::endl;
            return std::make_unique<StarlinkProvider>();
        case Provider::IRIDIUM:
            std::cout << "Creating IridiumProvider" << std::endl;
            return std::make_unique<IridiumProvider>();
        default:
            std::cout << "Unsupported provider" << std::endl;
            throw std::runtime_error("Unsupported satellite provider");
    }
}

// Add implementations for SatelliteConfig methods
void SatelliteConfig::setProvider(Provider provider) {
    provider_ = provider;
}

void SatelliteConfig::setProviderSpecificOption(const std::string& key, const std::string& value) {
    providerOptions_[key] = value;
}

// Implement other virtual functions as needed

} // namespace Satellite
} // namespace SRPT