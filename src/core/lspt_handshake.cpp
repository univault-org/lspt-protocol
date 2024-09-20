#include "lspt_handshake.h"
#include "../crypto/lspt_crypto.h"
#include <stdexcept>
#include <iostream> // For debug logs

namespace LSPT {

Handshake::Handshake() : state(HandshakeState::Initial) {
    std::cout << "Handshake initialized" << std::endl;
}

std::vector<uint8_t> Handshake::initiateHandshake(bool isGroundStation) {
    std::cout << "Initiating handshake. Is Ground Station: " << (isGroundStation ? "Yes" : "No") << std::endl;
    if (state != HandshakeState::Initial) {
        throw std::runtime_error("Handshake already initiated");
    }
    
    localKeyPair = generateKeyPair();

    std::vector<uint8_t> message;
    message.push_back(isGroundStation ? 0x01 : 0x00);
    auto publicKey = getPublicKey(localKeyPair);
    message.insert(message.end(), publicKey.begin(), publicKey.end());
    
    state = isGroundStation ? HandshakeState::GroundStationHello : HandshakeState::DroneHello;
    return message;
}

std::vector<uint8_t> Handshake::handleHandshakeMessage(const std::vector<uint8_t>& message) {
    if (state == HandshakeState::Initial || state == HandshakeState::DroneHello || state == HandshakeState::GroundStationHello) {
        bool isFromGroundStation = message[0] == 0x01;
        std::cout << "Received handshake message from: " << (isFromGroundStation ? "Ground Station" : "Drone") << std::endl;
        
        std::vector<uint8_t> peerPublicKey(message.begin() + 1, message.end());
        if (state == HandshakeState::Initial) {
            localKeyPair = generateKeyPair();
        }

        sharedSecret = computeSharedSecret(localKeyPair, peerPublicKey);

        std::vector<uint8_t> response;
        if (state == HandshakeState::Initial) {
            response.push_back(isFromGroundStation ? 0x00 : 0x01);
            auto publicKey = getPublicKey(localKeyPair);
            response.insert(response.end(), publicKey.begin(), publicKey.end());
        }

        state = HandshakeState::Established;

        return response;  // This will be empty if it's not the initial response
    } else if (state == HandshakeState::Established) {
        throw std::runtime_error("Unexpected handshake message");
    } else {
        throw std::runtime_error("Invalid handshake state");
    }
}

const std::vector<uint8_t>& Handshake::getSharedSecret() const {
    if (state != HandshakeState::Established) {
        throw std::runtime_error("Handshake not completed");
    }
    return sharedSecret;
}

HandshakeState Handshake::getState() const {
    return state;
}

} // namespace LSPT