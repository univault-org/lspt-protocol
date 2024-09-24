#include "ground_handshake.h"
#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <sodium.h> // Make sure this is included for crypto_box_PUBLICKEYBYTES

namespace SRPT::Ground {

GroundHandshake::GroundHandshake() {
    localKeyPair = SRPT::generateKeyPair();
}

Common::ByteVector GroundHandshake::initiateHandshake() {
    auto message = Handshake::initiateHandshake();
    
    assert(message.size() == static_cast<size_t>(crypto_box_PUBLICKEYBYTES) && "Unexpected message size in GroundHandshake::initiateHandshake");
    
    return message;
}

Common::ByteVector GroundHandshake::handleHandshakeMessage(const Common::ByteVector& message) {

    Common::ByteVector cleanMessage = removeIdentifier(message);
    printf("Clean message size: %zu\n", cleanMessage.size());

    switch (state) {
        case HandshakeState::Initial:
            if (message.size() == crypto_box_PUBLICKEYBYTES + 1 && cleanMessage.size() == crypto_box_PUBLICKEYBYTES) {
                peerPublicKey = cleanMessage;
                auto response = initiateHandshake();
                state = HandshakeState::HelloSent;
                return addGroundIdentifier(response);
            } else {
                throw std::runtime_error("Invalid peer public key size");
            }
        
        case HandshakeState::HelloSent:
            if (cleanMessage.empty()) {
                computeSharedSecretInternal();
                state = HandshakeState::Established;
                return Common::ByteVector{};
            } else {
                throw std::runtime_error("Unexpected message in HelloSent state");
            }
        
        case HandshakeState::Established:
            return Common::ByteVector{};
        
        default:
            throw std::runtime_error("Invalid handshake state");
    }
}

Common::ByteVector GroundHandshake::addGroundIdentifier(const Common::ByteVector& message) const {
    Common::ByteVector result = message;
    result.insert(result.begin(), SRPT::Common::SPACE_IDENTIFIER);
    result.insert(result.begin(), SRPT::Common::GROUND_IDENTIFIER);
    return result;
}

Common::ByteVector GroundHandshake::removeIdentifier(const Common::ByteVector& message) const {
    if (message.size() > 0 && message[0] == SRPT::Common::SPACE_IDENTIFIER) {
        return Common::ByteVector(message.begin() + 1, message.end());
    }
    return message;
}

} // namespace SRPT::Ground