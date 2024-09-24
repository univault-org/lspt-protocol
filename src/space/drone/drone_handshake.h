#pragma once

#include "../../core/srpt_handshake.h"
#include "../../crypto/srpt_crypto.h"

namespace SRPT::Space::Drone {

class DroneHandshake : public SRPT::Handshake {
public:
    DroneHandshake();

    // Override the initiateHandshake method
    Common::ByteVector initiateHandshake() override;

    // Override the handleHandshakeMessage method
    Common::ByteVector handleHandshakeMessage(const Common::ByteVector& message) override;

    // Get the shared secret after handshake is established
    const Common::ByteVector& getSharedSecret() const override;

    // Get the current handshake state
    HandshakeState getState() const override;

    // Extract peer public key from message
    Common::ByteVector extractPeerPublicKey(const Common::ByteVector& message) const;

private:
    // Add drone-specific identifier to outgoing messages
    Common::ByteVector addDroneIdentifier(const Common::ByteVector& message) const;

    // Remove identifier from incoming messages
    Common::ByteVector removeIdentifier(const Common::ByteVector& message) const;
};

} // namespace SRPT::Space::Drone