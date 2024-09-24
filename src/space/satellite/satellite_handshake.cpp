#include "satellite_handshake.h"
#include <iostream>

namespace SRPT::Space::Satellite {

SatelliteHandshake::SatelliteHandshake() {
    // Initialization specific to SatelliteHandshake if needed
}

Common::ByteVector SatelliteHandshake::initiateHandshake() {
    std::cout << "Initiating satellite handshake" << std::endl;
    // Call the base class method or implement specific logic
    return Handshake::initiateHandshake();  // You can customize this further if needed
}

Common::ByteVector SatelliteHandshake::handleHandshakeMessage(const Common::ByteVector& message) {
    std::cout << "Handling satellite handshake message" << std::endl;
    // Implement specific logic for handling messages
    return Handshake::handleHandshakeMessage(message);  // You can customize this further if needed
}

const Common::ByteVector& SatelliteHandshake::getSharedSecret() const {  // Fixed return type
    return Handshake::getSharedSecret();  // Call the base class method
}

HandshakeState SatelliteHandshake::getState() const {
    return Handshake::getState();  // Call the base class method
}

Common::ByteVector SatelliteHandshake::addSatelliteIdentifier(const Common::ByteVector& message) const {
    // Implement logic to add a satellite-specific identifier to the message
    return message;  // Modify this as needed
}

Common::ByteVector SatelliteHandshake::removeIdentifier(const Common::ByteVector& message) const {
    // Implement logic to remove the satellite-specific identifier from the message
    return message;  // Modify this as needed
}

} // namespace SRPT::Space::Satellite