#pragma once

#include <string>
#include <vector>
#include <memory>

namespace SRPT {
namespace Satellite {
namespace Test {

class IridiumMockAPI {
public:
    struct MOMessage {  // Mobile Originated Message
        std::vector<uint8_t> payload;
        uint32_t momsn;  // Mobile Originated Message Sequence Number
        std::string imei;
        std::string timestamp;
    };

    struct MTMessage {  // Mobile Terminated Message
        std::vector<uint8_t> payload;
        uint32_t mtmsn;  // Mobile Terminated Message Sequence Number
        std::string imei;
        std::string timestamp;
    };

    struct SessionStatus {
        int32_t status;
        std::string description;
    };

    virtual ~IridiumMockAPI() = default;

    virtual SessionStatus sendMOMessage(const MOMessage& message) = 0;
    virtual SessionStatus checkMTMessages(const std::string& imei, std::vector<MTMessage>& messages) = 0;
    virtual int32_t getSignalQuality(const std::string& imei) = 0;
    virtual SessionStatus initiateSession(const std::string& imei) = 0;
    virtual SessionStatus terminateSession(const std::string& imei) = 0;
    virtual std::string getIMEI() = 0;
    virtual SessionStatus clearMOQueue() = 0;
    virtual SessionStatus clearMTQueue() = 0;
};

// Add this line at the end of the file, inside the namespaces
std::unique_ptr<IridiumMockAPI> createIridiumMockAPI();

} // namespace Test
} // namespace Satellite
} // namespace SRPT