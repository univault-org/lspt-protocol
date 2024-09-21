#pragma once

#include "../../src/core/lspt_handshake.h"  // Include the base Handshake class

namespace LSPT {
namespace Common {
    using ByteVector = std::vector<uint8_t>;
}
}

class MockHandshake : public LSPT::Handshake {
public:
    MockHandshake() : state(LSPT::HandshakeState::Initial), messageCount(0) {}

    LSPT::Common::ByteVector initiateHandshake() override {
        if (state != LSPT::HandshakeState::Initial) {
            throw std::runtime_error("Handshake already initiated");
        }
        state = LSPT::HandshakeState::InProgress;
        return LSPT::Common::ByteVector{1}; // Non-empty response
    }

    LSPT::Common::ByteVector handleHandshakeMessage(const LSPT::Common::ByteVector& message) override {
        if (state == LSPT::HandshakeState::Initial) {
            throw std::runtime_error("Handshake not initiated");
        }
        if (state == LSPT::HandshakeState::Established) {
            throw std::runtime_error("Unexpected message in Established state");
        }
        if (message.empty() || message[0] == 0x00) {
            throw std::runtime_error("Invalid message received");
        }
        
        messageCount++;
        if (messageCount >= 2) {
            state = LSPT::HandshakeState::Established;
            return LSPT::Common::ByteVector{}; // Empty response to indicate completion
        }
        return LSPT::Common::ByteVector{3}; // Non-empty response
    }

    const LSPT::Common::ByteVector& getSharedSecret() const override {
        if (state != LSPT::HandshakeState::Established) {
            throw std::runtime_error("Shared secret not available: Handshake not completed");
        }
        // Use a static member to store the mock shared secret
        static const LSPT::Common::ByteVector mockSharedSecret{4, 5, 6};
        return mockSharedSecret;
    }

    LSPT::HandshakeState getState() const override {
        return state;
    }

    // For testing purposes
    void setInitCount(int count) { initCount = count; }
    void setState(LSPT::HandshakeState newState) { state = newState; }

    // For debugging
    int getMessageCount() const { return messageCount; }

private:
    LSPT::HandshakeState state;
    int initCount;
    int messageCount;
};