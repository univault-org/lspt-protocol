#pragma once
#include <stdexcept>
#include "../../src/core/srpt_handshake.h"  // Include the base Handshake class

namespace SRPT {
namespace Common {
    using ByteVector = std::vector<uint8_t>;
}
}

class MockHandshake : public SRPT::Handshake {
public:
    MockHandshake() : state(SRPT::HandshakeState::Initial), messageCount(0) {}

    SRPT::Common::ByteVector initiateHandshake() override {
        if (state != SRPT::HandshakeState::Initial) {
            throw std::runtime_error("Handshake already initiated");
        }
        state = SRPT::HandshakeState::InProgress;
        return SRPT::Common::ByteVector{1}; // Non-empty response
    }

    SRPT::Common::ByteVector handleHandshakeMessage(const SRPT::Common::ByteVector& message) override {
        if (state == SRPT::HandshakeState::Initial) {
            throw std::runtime_error("Handshake not initiated");
        }
        if (state == SRPT::HandshakeState::Established) {
            throw std::runtime_error("Unexpected message in Established state");
        }
        if (message.empty() || message[0] == 0x00) {
            throw std::runtime_error("Invalid message received");
        }
        
        messageCount++;
        if (messageCount >= 2) {
            state = SRPT::HandshakeState::Established;
            return SRPT::Common::ByteVector{}; // Empty response to indicate completion
        }
        return SRPT::Common::ByteVector{3}; // Non-empty response
    }

    const SRPT::Common::ByteVector& getSharedSecret() const override {
        if (state != SRPT::HandshakeState::Established) {
            throw std::runtime_error("Shared secret not available: Handshake not completed");
        }
        // Use a static member to store the mock shared secret
        static const SRPT::Common::ByteVector mockSharedSecret{4, 5, 6};
        return mockSharedSecret;
    }

    SRPT::HandshakeState getState() const override {
        return state;
    }

    // For testing purposes
    void setInitCount(int count) { initCount = count; }
    void setState(SRPT::HandshakeState newState) { state = newState; }

    // For debugging
    int getMessageCount() const { return messageCount; }

private:
    SRPT::HandshakeState state;
    int initCount;
    int messageCount;
};