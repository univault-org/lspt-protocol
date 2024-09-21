#include "vectorized_data_handler.h"
#include <limits>
#include <cmath>

// Helper functions for float-to-half and half-to-float conversion
uint16_t floatToHalf(float f) {
    uint32_t x = *reinterpret_cast<uint32_t*>(&f);
    uint16_t h = ((x >> 16) & 0x8000) | ((((x & 0x7f800000) - 0x38000000) >> 13) & 0x7c00) | ((x >> 13) & 0x03ff);
    return h;
}

float halfToFloat(uint16_t h) {
    uint32_t x = ((static_cast<uint32_t>(h) & 0x8000) << 16) | 
                 (((static_cast<uint32_t>(h) & 0x7c00) + 0x1C000) << 13) |
                 ((static_cast<uint32_t>(h) & 0x03FF) << 13);
    return *reinterpret_cast<float*>(&x);
}

std::vector<uint16_t> VectorizedDataHandler::quantize16Bit(const std::vector<float>& data) {
    std::vector<uint16_t> quantized(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        quantized[i] = floatToHalf(data[i]);
    }
    return quantized;
}

std::vector<float> VectorizedDataHandler::dequantize16Bit(const std::vector<uint16_t>& data) {
    std::vector<float> dequantized(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        dequantized[i] = halfToFloat(data[i]);
    }
    return dequantized;
}

// ... other existing methods ...