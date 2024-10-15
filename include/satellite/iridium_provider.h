#ifndef SRPT_IRIDIUM_PROVIDER_H
#define SRPT_IRIDIUM_PROVIDER_H

#include "../srpt_satellite.h"
#include <deque>

namespace SRPT {
namespace Satellite {

class IridiumProvider : public ISatelliteProvider {
public:
    IridiumProvider() = default;
    ~IridiumProvider() override = default;

    bool Initialize(const std::map<std::string, std::string>& options) override;
    bool Connect(const std::string& satellite_id) override;
    bool Disconnect() override;
    bool SendData(const ByteVector& data) override;
    bool ReceiveData(ByteVector& data) override;
    bool ExecuteCommand(const std::string& command, std::string& response) override;
    double GetSignalStrength() const override;
    double GetLatency() const override;
    uint64_t GetBandwidth() const override;
    std::unique_ptr<SatelliteStream> CreateStream() override;
    void setVerboseLogging(bool verbose) override { m_verboseLogging = verbose; }

private:
    std::deque<ByteVector> m_dataQueue;
    bool m_verboseLogging = false;

    class IridiumStream : public SatelliteStream {
    public:
        explicit IridiumStream(IridiumProvider& provider);
        bool Write(const ByteVector& data) override;
        bool Read(ByteVector& data) override;
        void Close() override;

    private:
        IridiumProvider& m_provider;
    };
};

// Factory function for creating IridiumProvider instances
std::unique_ptr<ISatelliteProvider> CreateIridiumProvider();

} // namespace Satellite
} // namespace SRPT

#endif // SRPT_IRIDIUM_PROVIDER_H
