#include "drone_handshake.h"
#include <cassert>
#include <sodium.h> // Make sure this is included for crypto_box_PUBLICKEYBYTES
#include <cstdio> // For printf

namespace LSPT::Space::Drone {

// Add this helper function at the beginning of the file, after the includes
void printFirstBytes(const char* label, const Common::ByteVector& data, size_t numBytes = 4) {
    printf("%s (first %zu bytes): ", label, numBytes);
    for (size_t i = 0; i < numBytes && i < data.size(); ++i) {
        printf("%02X ", static_cast<unsigned char>(data[i]));
    }
    printf("\n");
}

DroneHandshake::DroneHandshake() {
    // Initialize localKeyPair here
    localKeyPair = LSPT::generateKeyPair(); // Assuming you have a function like this in lspt_crypto.h
}

Common::ByteVector DroneHandshake::initiateHandshake() {
    auto message = Handshake::initiateHandshake();
    message.insert(message.begin(), LSPT::Common::SPACE_IDENTIFIER);
    
    return message;
}

Common::ByteVector DroneHandshake::handleHandshakeMessage(const Common::ByteVector& message) {
    
    Common::ByteVector processedMessage = removeIdentifier(message);

    switch (state) {
        case HandshakeState::Initial:
            // Unexpected state, initiate handshake
            return addDroneIdentifier(initiateHandshake());
        
        case HandshakeState::HelloSent:
            if (processedMessage.size() == crypto_box_PUBLICKEYBYTES) {
                // Update the base class member
                this->peerPublicKey = processedMessage;
                
                computeSharedSecretInternal();  // Use the base class method
                
                // Check if shared secret computation was successful
                if (this->sharedSecret.empty()) {
                    throw std::runtime_error("Failed to compute shared secret");
                } else {
                    printf("Drone: Shared secret computed successfully. Size: %zu\n", this->sharedSecret.size());
                }
                
                state = HandshakeState::Established;
                return Common::ByteVector{};
            } else {
                printf("Drone: Invalid peer public key size: %zu (original: %zu, processed: %zu)\n", 
                       static_cast<size_t>(crypto_box_PUBLICKEYBYTES), message.size(), processedMessage.size());
                throw std::runtime_error("Invalid peer public key size");
            }
        
        case HandshakeState::Established:
            return Common::ByteVector{};
        
        default:
            throw std::runtime_error("Invalid handshake state");
    }
}

const Common::ByteVector& DroneHandshake::getSharedSecret() const {
    const auto& secret = Handshake::getSharedSecret();
    if (secret.empty()) {
        printf("Drone: Warning - shared secret is empty\n");
    } else {
        printFirstBytes("Drone: Shared secret", secret);
    }
    return secret;
}

HandshakeState DroneHandshake::getState() const {
    return Handshake::getState();  // Call the base class method
}

Common::ByteVector DroneHandshake::addDroneIdentifier(const Common::ByteVector& message) const {
    Common::ByteVector result = message;
    result.insert(result.begin(), LSPT::Common::SPACE_IDENTIFIER);
    return result;
}

Common::ByteVector DroneHandshake::removeIdentifier(const Common::ByteVector& message) const {
    if (message.size() < 2) {
        return message;
    }
    
    if (message[0] == LSPT::Common::GROUND_IDENTIFIER && message[1] == LSPT::Common::SPACE_IDENTIFIER) {
        auto result = Common::ByteVector(message.begin() + 2, message.end());
        return result;
    } else if (message[0] == LSPT::Common::GROUND_IDENTIFIER) {
        auto result = Common::ByteVector(message.begin() + 1, message.end());
        return result;
    }
    return message;
}

// Add this new method
Common::ByteVector DroneHandshake::extractPeerPublicKey(const Common::ByteVector& message) const {
    // The message should already have the identifier removed at this point
    if (message.size() != crypto_box_PUBLICKEYBYTES) {
        throw std::runtime_error("Invalid peer public key size");
    }
    return message;
}

} // namespace LSPT::Space::Drone