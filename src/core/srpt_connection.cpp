#include "srpt_connection.h"

namespace SRPT {

SRPTConnection::SRPTConnection() 
    : state_(SRPTConnectionState::CLOSED),
      lastActivityTime_(std::chrono::steady_clock::now()),
      keepAliveInterval_(std::chrono::seconds(60)) // Default to 60 seconds
{
}

SRPTConnection::~SRPTConnection() {}

bool SRPTConnection::initiate() {
    if (state_ != SRPTConnectionState::CLOSED) {
        return false;
    }
    state_ = SRPTConnectionState::SYN_SENT;
    return true;
}

bool SRPTConnection::handleIncomingSYN() {
    if (state_ == SRPTConnectionState::CLOSED) {
        state_ = SRPTConnectionState::SYN_RECEIVED;
        return true;
    }
    return false; // Unexpected SYN in other states
}

bool SRPTConnection::handleIncomingSYNACK() {
    if (state_ == SRPTConnectionState::SYN_SENT) {
        state_ = SRPTConnectionState::ESTABLISHED;
        return true;
    }
    return false; // Unexpected SYNACK in other states
}

bool SRPTConnection::handleIncomingACK() {
    switch (state_) {
        case SRPTConnectionState::SYN_RECEIVED:
            state_ = SRPTConnectionState::ESTABLISHED;
            return true;
        case SRPTConnectionState::FIN_WAIT_1:
            state_ = SRPTConnectionState::FIN_WAIT_2;
            return true;
        case SRPTConnectionState::CLOSING:
            state_ = SRPTConnectionState::TIME_WAIT;
            return true;
        case SRPTConnectionState::LAST_ACK:
            state_ = SRPTConnectionState::CLOSED;
            return true;
        default:
            return false;
    }
}

bool SRPTConnection::initiateClose() {
    if (state_ != SRPTConnectionState::ESTABLISHED) {
        return false;
    }
    state_ = SRPTConnectionState::FIN_WAIT_1;
    return true;
}

bool SRPTConnection::handleIncomingFIN() {
    switch (state_) {
        case SRPTConnectionState::ESTABLISHED:
            state_ = SRPTConnectionState::CLOSE_WAIT;
            return true;
        case SRPTConnectionState::FIN_WAIT_1:
            state_ = SRPTConnectionState::CLOSING;
            return true;
        case SRPTConnectionState::FIN_WAIT_2:
            state_ = SRPTConnectionState::TIME_WAIT;
            return true;
        default:
            return false; // Unexpected FIN in other states
    }
}

bool SRPTConnection::close() {
    if (state_ == SRPTConnectionState::CLOSE_WAIT) {
        state_ = SRPTConnectionState::LAST_ACK;
        return true;
    }
    return false;
}

SRPTConnectionState SRPTConnection::getState() const { return state_; }
void SRPTConnection::setState(SRPTConnectionState newState) { state_ = newState; }

void SRPTConnection::simulateTimeWaitTimeout() {
    if (state_ == SRPTConnectionState::TIME_WAIT) {
        state_ = SRPTConnectionState::CLOSED;
    }
}

bool SRPTConnection::handleReset() {
    resetConnection();
    return true;
}

void SRPTConnection::simulateTimeout() {
    switch (state_) {
        case SRPTConnectionState::SYN_SENT:
        case SRPTConnectionState::SYN_RECEIVED:
        case SRPTConnectionState::FIN_WAIT_1:
        case SRPTConnectionState::FIN_WAIT_2:
        case SRPTConnectionState::CLOSING:
        case SRPTConnectionState::LAST_ACK:
        case SRPTConnectionState::TIME_WAIT:
            resetConnection();
            break;
        case SRPTConnectionState::CLOSE_WAIT:
            // In CLOSE_WAIT, we might want to force the connection closed
            state_ = SRPTConnectionState::CLOSED;
            break;
        default:
            // Do nothing for ESTABLISHED and CLOSED states
            break;
    }
}

void SRPTConnection::resetConnection() {
    state_ = SRPTConnectionState::CLOSED;
    // Reset any other connection-specific data here
}

bool SRPTConnection::isValidTransition(SRPTConnectionState newState) const { return true; }

// Packet retransmission
bool SRPTConnection::sendPacket(uint32_t sequenceNumber, const std::vector<uint8_t>& data) {
    unacknowledgedPackets_[sequenceNumber] = data;
    lastActivityTime_ = std::chrono::steady_clock::now();
    return true;
}

bool SRPTConnection::acknowledgePacket(uint32_t sequenceNumber) {
    auto it = unacknowledgedPackets_.find(sequenceNumber);
    if (it != unacknowledgedPackets_.end()) {
        unacknowledgedPackets_.erase(it);
        lastActivityTime_ = std::chrono::steady_clock::now();
        return true;
    }
    return false;
}

void SRPTConnection::retransmitUnacknowledgedPackets() {
    // In a real implementation, you would actually resend the packets here
    lastActivityTime_ = std::chrono::steady_clock::now();
}

// Keep-alive
void SRPTConnection::sendKeepAlive() {
    lastActivityTime_ = std::chrono::steady_clock::now();
}

bool SRPTConnection::handleKeepAlive() {
    lastActivityTime_ = std::chrono::steady_clock::now();
    return true;
}

void SRPTConnection::setKeepAliveInterval(std::chrono::seconds interval) {
    keepAliveInterval_ = interval;
    lastActivityTime_ = std::chrono::steady_clock::now();
}

// For testing
size_t SRPTConnection::getUnacknowledgedPacketCount() const {
    return unacknowledgedPackets_.size();
}

bool SRPTConnection::isConnectionAlive() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastActivityTime_);
    return elapsed <= (keepAliveInterval_ + std::chrono::milliseconds(100)); // Add 100ms buffer
}

void SRPTConnection::setReceiveWindowSize(uint32_t size) {
    receive_window_size_ = size;
    available_window_size_ = size;
}

uint32_t SRPTConnection::getReceiveWindowSize() const {
    return receive_window_size_;
}

void SRPTConnection::updateAvailableWindowSize(uint32_t size) {
    available_window_size_ = std::min(size, receive_window_size_);
}

uint32_t SRPTConnection::getAvailableWindowSize() const {
    return available_window_size_;
}

bool SRPTConnection::canSendData(uint32_t dataSize) const {
    return dataSize <= available_window_size_;
}

bool SRPTConnection::sendData(const std::vector<uint8_t>& data) {
    if (!canSendData(data.size())) {
        return false;
    }
    // Existing send logic...
    available_window_size_ -= data.size();
    lastActivityTime_ = std::chrono::steady_clock::now(); // Update last activity time
    return true;
}

void SRPTConnection::receiveData(const std::vector<uint8_t>& data) {
    if (state_ != SRPTConnectionState::ESTABLISHED) {
        return; // Only receive data in ESTABLISHED state
    }
    // Existing receive logic...
    available_window_size_ -= data.size();
    // Logic to process received data...
    // After processing:
    uint32_t processed_size = data.size(); // Assume all data is processed
    available_window_size_ += processed_size;
    lastActivityTime_ = std::chrono::steady_clock::now(); // Update last activity time
    // Notify peer about updated window size
    sendWindowUpdate(available_window_size_);
}

void SRPTConnection::sendWindowUpdate(uint32_t newSize) {
    // In a real implementation, you would create and send a window update packet
    // For now, we'll just update the local state
    available_window_size_ = newSize;
    lastActivityTime_ = std::chrono::steady_clock::now(); // Update last activity time
}

} // namespace SRPT