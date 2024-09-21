#pragma once

#include "../common/types.h"
#include "../crypto/lspt_crypto.h"
#include <cstdio> // For printf

namespace LSPT {

enum class HandshakeState {
    Initial,
    HelloSent,
    InProgress,
    AwaitingResponse,
    Established
};

class Handshake {
protected:
    Common::ByteVector peerPublicKey;
    HandshakeState state;
    Common::ByteVector sharedSecret;
    KeyPair localKeyPair;

public:
    Handshake();
    virtual ~Handshake() = default;

    virtual Common::ByteVector initiateHandshake();
    virtual Common::ByteVector handleHandshakeMessage(const Common::ByteVector& message) = 0;
    virtual const Common::ByteVector& getSharedSecret() const;
    virtual HandshakeState getState() const;

    const Common::ByteVector& getPeerPublicKey() const { return peerPublicKey; }

protected:
    Common::ByteVector generatePublicKey();
    void computeSharedSecretInternal();
};

} // namespace LSPT
