#pragma once

#include "../core/srpt_handshake.h"
#include "../crypto/srpt_crypto.h"

namespace SRPT::Ground {

class GroundHandshake : public SRPT::Handshake {
private:
    SRPT::KeyPair localKeyPair;

public:
    GroundHandshake();
    virtual ~GroundHandshake() = default;

    Common::ByteVector initiateHandshake() override;
    Common::ByteVector handleHandshakeMessage(const Common::ByteVector& message) override;

private:
    Common::ByteVector addGroundIdentifier(const Common::ByteVector& message) const;
    Common::ByteVector removeIdentifier(const Common::ByteVector& message) const;
};

} // namespace SRPT::Ground