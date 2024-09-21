#pragma once

#include "../core/lspt_handshake.h"
#include "../crypto/lspt_crypto.h"

namespace LSPT::Ground {

class GroundHandshake : public LSPT::Handshake {
private:
    LSPT::KeyPair localKeyPair;

public:
    GroundHandshake();
    virtual ~GroundHandshake() = default;

    Common::ByteVector initiateHandshake() override;
    Common::ByteVector handleHandshakeMessage(const Common::ByteVector& message) override;

private:
    Common::ByteVector addGroundIdentifier(const Common::ByteVector& message) const;
    Common::ByteVector removeIdentifier(const Common::ByteVector& message) const;
};

} // namespace LSPT::Ground