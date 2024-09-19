#pragma once

#include <vector>
#include <cstdint>

namespace LSPT {

// Encrypt data using AES-GCM
void encryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                   const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                   std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag);

// Decrypt data using AES-GCM
std::vector<uint8_t> decryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                   const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                   const std::vector<uint8_t>& tag);

// Encrypt data using ChaCha20-Poly1305
void encryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                             const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                             std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag);

// Decrypt data using ChaCha20-Poly1305
std::vector<uint8_t> decryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                             const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                             const std::vector<uint8_t>& tag);

} // namespace LSPT