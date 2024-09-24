#pragma once
#include <vector>
#include <sodium.h>

namespace SRPT {

struct KeyPair {
    std::vector<uint8_t> privateKey;
    std::vector<uint8_t> publicKey;
};

void encryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                   const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                   std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag);

std::vector<uint8_t> decryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                   const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                   const std::vector<uint8_t>& tag);

KeyPair generateKeyPair();
std::vector<uint8_t> getPublicKey(const KeyPair& keyPair);
std::vector<uint8_t> computeSharedSecret(const KeyPair& localKeyPair, const std::vector<uint8_t>& peerPublicKey);

void encryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                             const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                             std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag);

std::vector<uint8_t> decryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                             const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                             const std::vector<uint8_t>& tag);

} // namespace SRPT