#pragma once

#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

class VectorizedDataHandler {
public:
    // ... existing methods ...

    std::vector<uint16_t> quantize16Bit(const std::vector<float>& data);
    std::vector<float> dequantize16Bit(const std::vector<uint16_t>& data);

    // ... other methods ...
};