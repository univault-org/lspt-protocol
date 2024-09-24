# SRPT Protocol Usage Example: Drone-Satellite-Ground Station Communication

This example demonstrates how to use the SRPT (Satellite Relay Package Transfer) protocol in a real-world scenario involving a drone, a satellite relay, and a ground station.

## Setup

- Drone: MacBook running C++ application
- Satellite: iPhone running Swift application
- Ground Station: iMac running C++ application

## Prerequisites

- SRPT protocol library installed
- CMake (version 3.12 or higher)
- Xcode (for iOS development)
- Boost library (for networking in C++ applications)

## Flow Explanation - Drone-initiated Handshake

1. The Ground Station starts and waits for a connection from the Drone.
2. The Drone initiates a connection to the Ground Station for key exchange.
3. Both Drone and Ground Station generate their public keys and exchange them.
4. Both sides compute a shared secret using the exchanged public keys.
5. The Drone collects sensor data, encrypts it using the shared secret, and creates a packet.
6. The Drone sends the encrypted packet to the Satellite.
7. The Satellite receives the packet and relays it to the Ground Station.
8. The Ground Station receives the packet, parses it, and decrypts the data using the shared secret.

## Flow Explanation - Ground Station-initiated Handshake

1. The Ground Station initiates a connection to the Satellite, requesting data from a specific Drone mission.
2. The Satellite acknowledges the request and initiates a connection to the Drone.
3. The Drone and Ground Station perform a key exchange through the Satellite:
   a. The Drone generates a new public key and sends it to the Satellite.
   b. The Satellite relays the Drone's public key to the Ground Station.
   c. The Ground Station generates its public key and sends it back through the Satellite.
   d. The Drone receives the Ground Station's public key from the Satellite.
4. Both the Drone and Ground Station compute a shared secret using the exchanged public keys.
5. The Drone encrypts the requested mission data using the shared secret.
6. The Drone sends the encrypted data to the Satellite.
7. The Satellite receives the encrypted data and relays it to the Ground Station.
8. The Ground Station receives the encrypted data from the Satellite.
9. The Ground Station decrypts the data using the shared secret.
10. The Ground Station processes and stores the decrypted mission data.

## Current Handshake Implementation

The current implementation provides a flexible handshake process that works for both immediate and delayed data transmission scenarios. It supports both Drone and Ground Station initiated handshakes.

Key features:
- Uses X25519 for key exchange, which is efficient and secure.
- Completes the handshake in two messages, suitable for high-latency satellite communications.
- Derives the shared secret using ECDH (Elliptic Curve Diffie-Hellman) key exchange.
- Implements a state machine to ensure correct handshake progression regardless of initiator.

### Implemented Features

1. **Handshake Initiation**: Both Drone and Ground Station can initiate the handshake.
2. **Key Generation**: Each party generates its own key pair.
3. **Public Key Exchange**: The initiator sends its public key in the first message.
4. **Shared Secret Computation**: Both parties compute the shared secret after receiving the peer's public key.
5. **State Management**: A state machine tracks the handshake progress.
6. **Error Handling**: Throws exceptions for unexpected messages or premature shared secret access.

