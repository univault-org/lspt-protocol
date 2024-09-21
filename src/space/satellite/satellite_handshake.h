#pragma once

#include "../../common/types.h"  // Include any necessary common types
#include "../../core/lspt_handshake.h"  // Include the base Handshake class

namespace LSPT::Space::Satellite {

class SatelliteHandshake : public LSPT::Handshake {
public:
SatelliteHandshake();

// Override the initiateHandshake method
Common::ByteVector initiateHandshake() override;

// Override the handleHandshakeMessage method
Common::ByteVector handleHandshakeMessage(const Common::ByteVector& message) override;

// Get the shared secret after handshake is established
const Common::ByteVector& getSharedSecret() const override;  // Fixed return type

// Get the current handshake state
HandshakeState getState() const override;

private:
// Add satellite-specific identifier to outgoing messages
Common::ByteVector addSatelliteIdentifier(const Common::ByteVector& message) const;  // Declaration added

// Remove identifier from incoming messages
Common::ByteVector removeIdentifier(const Common::ByteVector& message) const;  // Declaration added
};

} // namespace LSPT::Space::Satellite