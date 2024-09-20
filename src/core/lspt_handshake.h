#pragma once

#include <vector>
#include <cstdint>
#include "../crypto/lspt_crypto.h"

namespace LSPT {

enum class HandshakeState {
    Initial,
    DroneHello,
    GroundStationHello,
    DroneKeyExchange,
    GroundStationKeyExchange,
    Established
};

class Handshake {
public:
    Handshake();
    std::vector<uint8_t> initiateHandshake(bool isGroundStation);
    std::vector<uint8_t> handleHandshakeMessage(const std::vector<uint8_t>& message);
    const std::vector<uint8_t>& getSharedSecret() const;
    HandshakeState getState() const;

private:
    HandshakeState state;
    KeyPair localKeyPair;
    std::vector<uint8_t> sharedSecret;
};

} // namespace LSPT