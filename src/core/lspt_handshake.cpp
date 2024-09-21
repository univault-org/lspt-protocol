#include "lspt_handshake.h"
#include "../common/types.h"  // Adjust the path as necessary
#include "../crypto/lspt_crypto.h"
#include <stdexcept>
#include <cstdio> // For printf

namespace LSPT {

Handshake::Handshake() : state(HandshakeState::Initial) {
    printf("Core Handshake initialized\n");
    localKeyPair = generateKeyPair();
}

Common::ByteVector Handshake::initiateHandshake() {
    if (state != HandshakeState::Initial) {
        state = HandshakeState::Initial;
        sharedSecret.clear();
        peerPublicKey.clear();
    }
    
    auto publicKey = generatePublicKey();
    state = HandshakeState::HelloSent;
    return publicKey;
}

Common::ByteVector Handshake::generatePublicKey() {
    auto publicKey = getPublicKey(localKeyPair);
    return publicKey;
}

void Handshake::computeSharedSecretInternal() {
    if (!peerPublicKey.empty()) {

        sharedSecret = computeSharedSecret(localKeyPair, peerPublicKey);
    } else {
        printf("Cannot compute shared secret: peer public key is empty\n");
    }
}

const Common::ByteVector& Handshake::getSharedSecret() const {
    if (state != HandshakeState::Established) {
        throw std::runtime_error("Handshake not completed");
    }
    return sharedSecret;
}

HandshakeState Handshake::getState() const {
    return state;
}

} // namespace LSPT
