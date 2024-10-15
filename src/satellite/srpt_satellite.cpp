#include "../../include/srpt_satellite.h"
#include "../../include/satellite/starlink_provider.h"
#include "../../include/satellite/iridium_provider.h"
#include <stdexcept>
#include <iostream>
#include <unordered_map>

namespace SRPT {
namespace Satellite {

static std::unordered_map<Provider, std::function<std::unique_ptr<ISatelliteProvider>()>>& GetProviderFactories() {
    static std::unordered_map<Provider, std::function<std::unique_ptr<ISatelliteProvider>()>> g_providerFactories;
    return g_providerFactories;
}

bool RegisterSatelliteProvider(Provider provider, std::function<std::unique_ptr<ISatelliteProvider>()> factory) {
    std::cout << "Registering provider: " << static_cast<int>(provider) << std::endl;
    try {
        auto result = GetProviderFactories().insert({provider, std::move(factory)});
        std::cout << "Registration " << (result.second ? "successful" : "failed (already exists)") << std::endl;
        return result.second;
    } catch (const std::exception& e) {
        std::cerr << "Exception during provider registration: " << e.what() << std::endl;
        return false;
    }
}

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
    void setVerboseLogging(bool verbose);  // Add this line

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

void SatelliteSession::setVerboseLogging(bool verbose) {
    pImpl->setVerboseLogging(verbose);
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

void SatelliteSession::Impl::setVerboseLogging(bool verbose) {
    if (provider_) {
        provider_->setVerboseLogging(verbose);
    }
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
              << static_cast<int>(provider) << std::endl;
    
    auto& factories = GetProviderFactories();
    auto it = factories.find(provider);
    if (it != factories.end()) {
        std::cout << "Provider found in factory map" << std::endl;
        return it->second();
    }
    
    std::cout << "Provider not found in factory map, using switch statement" << std::endl;
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

namespace {
    __attribute__((used)) static bool starlinkRegistered = SRPT::Satellite::RegisterSatelliteProvider(
        SRPT::Satellite::Provider::STARLINK,
        []() { return std::make_unique<SRPT::Satellite::StarlinkProvider>(); }
    );

    __attribute__((used)) static bool iridiumRegistered = SRPT::Satellite::RegisterSatelliteProvider(
        SRPT::Satellite::Provider::IRIDIUM,
        []() { return std::make_unique<SRPT::Satellite::IridiumProvider>(); }
    );
}
