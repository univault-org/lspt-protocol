#include "lspt_connection.h"

namespace LSPT {

LSPTConnection::LSPTConnection() 
    : state_(LSPTConnectionState::CLOSED),
      lastActivityTime_(std::chrono::steady_clock::now()),
      keepAliveInterval_(std::chrono::seconds(60)) // Default to 60 seconds
{
}

LSPTConnection::~LSPTConnection() {}

bool LSPTConnection::initiate() {
    if (state_ != LSPTConnectionState::CLOSED) {
        return false;
    }
    state_ = LSPTConnectionState::SYN_SENT;
    return true;
}

bool LSPTConnection::handleIncomingSYN() {
    if (state_ == LSPTConnectionState::CLOSED) {
        state_ = LSPTConnectionState::SYN_RECEIVED;
        return true;
    }
    return false; // Unexpected SYN in other states
}

bool LSPTConnection::handleIncomingSYNACK() {
    if (state_ == LSPTConnectionState::SYN_SENT) {
        state_ = LSPTConnectionState::ESTABLISHED;
        return true;
    }
    return false; // Unexpected SYNACK in other states
}

bool LSPTConnection::handleIncomingACK() {
    switch (state_) {
        case LSPTConnectionState::SYN_RECEIVED:
            state_ = LSPTConnectionState::ESTABLISHED;
            return true;
        case LSPTConnectionState::FIN_WAIT_1:
            state_ = LSPTConnectionState::FIN_WAIT_2;
            return true;
        case LSPTConnectionState::CLOSING:
            state_ = LSPTConnectionState::TIME_WAIT;
            return true;
        case LSPTConnectionState::LAST_ACK:
            state_ = LSPTConnectionState::CLOSED;
            return true;
        default:
            return false;
    }
}

bool LSPTConnection::initiateClose() {
    if (state_ != LSPTConnectionState::ESTABLISHED) {
        return false;
    }
    state_ = LSPTConnectionState::FIN_WAIT_1;
    return true;
}

bool LSPTConnection::handleIncomingFIN() {
    switch (state_) {
        case LSPTConnectionState::ESTABLISHED:
            state_ = LSPTConnectionState::CLOSE_WAIT;
            return true;
        case LSPTConnectionState::FIN_WAIT_1:
            state_ = LSPTConnectionState::CLOSING;
            return true;
        case LSPTConnectionState::FIN_WAIT_2:
            state_ = LSPTConnectionState::TIME_WAIT;
            return true;
        default:
            return false; // Unexpected FIN in other states
    }
}

bool LSPTConnection::close() {
    if (state_ == LSPTConnectionState::CLOSE_WAIT) {
        state_ = LSPTConnectionState::LAST_ACK;
        return true;
    }
    return false;
}

LSPTConnectionState LSPTConnection::getState() const { return state_; }
void LSPTConnection::setState(LSPTConnectionState newState) { state_ = newState; }

void LSPTConnection::simulateTimeWaitTimeout() {
    if (state_ == LSPTConnectionState::TIME_WAIT) {
        state_ = LSPTConnectionState::CLOSED;
    }
}

bool LSPTConnection::handleReset() {
    resetConnection();
    return true;
}

void LSPTConnection::simulateTimeout() {
    switch (state_) {
        case LSPTConnectionState::SYN_SENT:
        case LSPTConnectionState::SYN_RECEIVED:
        case LSPTConnectionState::FIN_WAIT_1:
        case LSPTConnectionState::FIN_WAIT_2:
        case LSPTConnectionState::CLOSING:
        case LSPTConnectionState::LAST_ACK:
        case LSPTConnectionState::TIME_WAIT:
            resetConnection();
            break;
        case LSPTConnectionState::CLOSE_WAIT:
            // In CLOSE_WAIT, we might want to force the connection closed
            state_ = LSPTConnectionState::CLOSED;
            break;
        default:
            // Do nothing for ESTABLISHED and CLOSED states
            break;
    }
}

void LSPTConnection::resetConnection() {
    state_ = LSPTConnectionState::CLOSED;
    // Reset any other connection-specific data here
}

bool LSPTConnection::isValidTransition(LSPTConnectionState newState) const { return true; }

// Packet retransmission
bool LSPTConnection::sendPacket(uint32_t sequenceNumber, const std::vector<uint8_t>& data) {
    unacknowledgedPackets_[sequenceNumber] = data;
    lastActivityTime_ = std::chrono::steady_clock::now();
    return true;
}

bool LSPTConnection::acknowledgePacket(uint32_t sequenceNumber) {
    auto it = unacknowledgedPackets_.find(sequenceNumber);
    if (it != unacknowledgedPackets_.end()) {
        unacknowledgedPackets_.erase(it);
        lastActivityTime_ = std::chrono::steady_clock::now();
        return true;
    }
    return false;
}

void LSPTConnection::retransmitUnacknowledgedPackets() {
    // In a real implementation, you would actually resend the packets here
    lastActivityTime_ = std::chrono::steady_clock::now();
}

// Keep-alive
void LSPTConnection::sendKeepAlive() {
    lastActivityTime_ = std::chrono::steady_clock::now();
}

bool LSPTConnection::handleKeepAlive() {
    lastActivityTime_ = std::chrono::steady_clock::now();
    return true;
}

void LSPTConnection::setKeepAliveInterval(std::chrono::seconds interval) {
    keepAliveInterval_ = interval;
    lastActivityTime_ = std::chrono::steady_clock::now();
}

// For testing
size_t LSPTConnection::getUnacknowledgedPacketCount() const {
    return unacknowledgedPackets_.size();
}

bool LSPTConnection::isConnectionAlive() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastActivityTime_);
    return elapsed <= (keepAliveInterval_ + std::chrono::milliseconds(100)); // Add 100ms buffer
}

} // namespace LSPT