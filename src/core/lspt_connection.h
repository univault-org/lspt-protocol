#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <chrono>

namespace LSPT {

enum class LSPTConnectionState {
    CLOSED,
    SYN_SENT,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT_1,
    FIN_WAIT_2,
    CLOSE_WAIT,
    CLOSING,
    LAST_ACK,
    TIME_WAIT
};

class LSPTConnection {
public:
    LSPTConnection();
    ~LSPTConnection();

    // Connection establishment
    bool initiate();
    bool handleIncomingSYN();
    bool handleIncomingSYNACK();
    bool handleIncomingACK();

    // Connection termination
    bool initiateClose();
    bool handleIncomingFIN();
    bool close();

    // Error handling and edge cases
    bool handleReset();
    void simulateTimeout();

    // State management
    LSPTConnectionState getState() const;
    void setState(LSPTConnectionState newState);

    // Simulation methods for testing
    void simulateTimeWaitTimeout();

    // Packet retransmission
    bool sendPacket(uint32_t sequenceNumber, const std::vector<uint8_t>& data);
    bool acknowledgePacket(uint32_t sequenceNumber);
    void retransmitUnacknowledgedPackets();

    // Keep-alive
    void setKeepAliveInterval(std::chrono::seconds interval);
    void sendKeepAlive();
    bool handleKeepAlive();
    bool isConnectionAlive() const;

    // For testing
    size_t getUnacknowledgedPacketCount() const;

private:
    LSPTConnectionState state_;
    std::map<uint32_t, std::vector<uint8_t>> unacknowledgedPackets_;
    std::chrono::steady_clock::time_point lastActivityTime_;
    std::chrono::seconds keepAliveInterval_;
    // Other private members...

    void resetConnection();
    bool isValidTransition(LSPTConnectionState newState) const;
};

} // namespace LSPT