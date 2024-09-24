#pragma once

#include <vector>
#include <cstdint>

namespace SRPT::Common {

// Common type for byte vectors used throughout the project
using ByteVector = std::vector<uint8_t>;

// Constants for identifiers
constexpr char SPACE_IDENTIFIER = 'E';
constexpr char GROUND_IDENTIFIER = 'G';  

// You might also want to include the HandshakeState enum here if it's used across multiple components
enum class HandshakeState {
    Initial,
    HelloSent,
    Established
};

} // namespace SRPT::Common