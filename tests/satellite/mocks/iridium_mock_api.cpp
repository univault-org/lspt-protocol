#include "iridium_mock_api.h"
#include <stdexcept>

namespace LSPT {
namespace Satellite {
namespace Test {

class IridiumMockAPIImpl : public IridiumMockAPI {
public:
    SessionStatus sendMOMessage(const MOMessage& message) override {
        return {0, "Message sent successfully"};
    }

    SessionStatus checkMTMessages(const std::string& imei, std::vector<MTMessage>& messages) override {
        messages.push_back({std::vector<uint8_t>{1, 2, 3}, 1, imei, "2023-05-10T12:00:00Z"});
        return {0, "Messages retrieved successfully"};
    }

    int32_t getSignalQuality(const std::string& imei) override {
        return 5; // Assuming maximum signal quality
    }

    SessionStatus initiateSession(const std::string& imei) override {
        return {0, "Session initiated successfully"};
    }

    SessionStatus terminateSession(const std::string& imei) override {
        return {0, "Session terminated successfully"};
    }

    std::string getIMEI() override {
        return "300234060109206"; // Example IMEI
    }

    SessionStatus clearMOQueue() override {
        return {0, "MO queue cleared successfully"};
    }

    SessionStatus clearMTQueue() override {
        return {0, "MT queue cleared successfully"};
    }
};

// Implement the factory function
std::unique_ptr<IridiumMockAPI> createIridiumMockAPI() {
    return std::make_unique<IridiumMockAPIImpl>();
}

} // namespace Test
} // namespace Satellite
} // namespace LSPT