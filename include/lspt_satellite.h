#ifndef LSPT_SATELLITE_H
#define LSPT_SATELLITE_H

#include "lspt.h"
#include <memory>
#include <string>
#include <map>
#include <functional>

namespace LSPT {
namespace Satellite {

// Enum for supported satellite providers
enum class Provider {
    STARLINK,
    IRIDIUM,
    // INMARSAT,
    // VIASAT,
    // Add other providers as needed
};

class SatelliteConfig {
public:
    const Provider& getProvider() const { return provider_; };
    void setProvider(Provider provider);
    void setProviderSpecificOption(const std::string& key, const std::string& value);
    // ... other common configuration options
private:
    Provider provider_;
    std::map<std::string, std::string> providerOptions_;
};

class SatelliteStream : public LSPT::Stream {
public:
    virtual ~SatelliteStream() = default;
    virtual bool Write(const ByteVector& data) override = 0;
    virtual bool Read(ByteVector& data) override = 0;
    virtual void Close() override = 0;
};

class ISatelliteProvider {
public:
    virtual ~ISatelliteProvider() = default;
    virtual bool Initialize(const std::map<std::string, std::string>& options) = 0;
    virtual bool Connect(const std::string& satellite_id) = 0;
    virtual bool Disconnect() = 0;
    virtual bool SendData(const ByteVector& data) = 0;
    virtual bool ReceiveData(ByteVector& data) = 0;
    virtual bool ExecuteCommand(const std::string& command, std::string& response) = 0;
    virtual double GetSignalStrength() const = 0;
    virtual double GetLatency() const = 0;
    virtual uint64_t GetBandwidth() const = 0;
    virtual std::unique_ptr<SatelliteStream> CreateStream() = 0;  // Add this line
};

class SatelliteSession {
public:
    SatelliteSession(const SatelliteConfig& config);
    virtual ~SatelliteSession();

    bool Connect(const std::string& satellite_id);
    bool Disconnect();
    std::unique_ptr<SatelliteStream> CreateSatelliteStream();

    // Provider-specific operations
    bool ExecuteProviderCommand(const std::string& command, std::string& response);

    // Methods for querying satellite status and performance
    double GetSignalStrength() const;
    double GetLatency() const;
    uint64_t GetBandwidth() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// Factory function to create satellite sessions
std::unique_ptr<SatelliteSession> CreateSatelliteSession(const SatelliteConfig& config);

// Factory function to create satellite providers
std::unique_ptr<ISatelliteProvider> CreateSatelliteProvider(Provider provider);

// Function to register satellite providers
bool RegisterSatelliteProvider(Provider provider, std::function<std::unique_ptr<ISatelliteProvider>()> factory);

// Satellite-specific error codes
enum class SatelliteError {
    NO_ERROR,
    CONNECTION_FAILED,
    SIGNAL_LOST,
    BANDWIDTH_EXCEEDED,
    // ... other error codes
};

// Get the last error
SatelliteError GetLastSatelliteError();

} // namespace Satellite
} // namespace LSPT

#endif // LSPT_SATELLITE_H