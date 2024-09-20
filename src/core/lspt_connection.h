#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <chrono>
#include <thread>

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

    // Flow control methods
    void setReceiveWindowSize(uint32_t size);
    uint32_t getReceiveWindowSize() const;
    void updateAvailableWindowSize(uint32_t size);
    uint32_t getAvailableWindowSize() const;
    bool canSendData(uint32_t dataSize) const;

    // Data transfer methods
    bool sendData(const std::vector<uint8_t>& data);
    void receiveData(const std::vector<uint8_t>& data);
    void sendWindowUpdate(uint32_t newSize);

    // For testing
    size_t getUnacknowledgedPacketCount() const;
    std::chrono::steady_clock::time_point getLastActivityTime() const {
        return lastActivityTime_;
    }

private:
    LSPTConnectionState state_;
    std::map<uint32_t, std::vector<uint8_t>> unacknowledgedPackets_;
    std::chrono::steady_clock::time_point lastActivityTime_;
    std::chrono::seconds keepAliveInterval_;
    uint32_t receive_window_size_;
    uint32_t available_window_size_;
    // Other private members...

    void resetConnection();
    bool isValidTransition(LSPTConnectionState newState) const;
};

} // namespace LSPT