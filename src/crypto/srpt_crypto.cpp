#include "srpt_crypto.h"
#include <stdexcept>
#include <random>
#include <algorithm>
#include <sodium.h>

namespace SRPT {

void encryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                   const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                   std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag) {
    // Implementation using libsodium
    if (sodium_init() < 0) {
        throw std::runtime_error("libsodium initialization failed");
    }

    ciphertext.resize(plaintext.size());
    tag.resize(crypto_aead_aes256gcm_ABYTES);

    if (crypto_aead_aes256gcm_encrypt_detached(
            ciphertext.data(), tag.data(), nullptr,
            plaintext.data(), plaintext.size(),
            aad.data(), aad.size(),
            nullptr, iv.data(), key.data()) != 0) {
        throw std::runtime_error("AES-GCM encryption failed");
    }
}

std::vector<uint8_t> decryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                   const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                   const std::vector<uint8_t>& tag) {
    // Implementation using libsodium
    if (sodium_init() < 0) {
        throw std::runtime_error("libsodium initialization failed");
    }

    std::vector<uint8_t> plaintext(ciphertext.size());

    if (crypto_aead_aes256gcm_decrypt_detached(
            plaintext.data(),
            nullptr,
            ciphertext.data(), ciphertext.size(),
            tag.data(),
            aad.data(), aad.size(),
            iv.data(), key.data()) != 0) {
        throw std::runtime_error("AES-GCM decryption failed");
    }

    return plaintext;
}

KeyPair generateKeyPair() {
    std::vector<uint8_t> privateKey(crypto_box_SECRETKEYBYTES);
    std::vector<uint8_t> publicKey(crypto_box_PUBLICKEYBYTES);

    if (sodium_init() < 0) {
        throw std::runtime_error("libsodium initialization failed");
    }

    crypto_box_keypair(publicKey.data(), privateKey.data());

    return {std::move(privateKey), std::move(publicKey)};
}

std::vector<uint8_t> getPublicKey(const KeyPair& keyPair) {
    return keyPair.publicKey;
}

std::vector<uint8_t> computeSharedSecret(const KeyPair& localKeyPair, const std::vector<uint8_t>& peerPublicKey) {
    if (peerPublicKey.size() != crypto_box_PUBLICKEYBYTES) {
        throw std::invalid_argument("Invalid peer public key size");
    }

    std::vector<uint8_t> sharedSecret(crypto_box_BEFORENMBYTES);

    if (crypto_box_beforenm(sharedSecret.data(), peerPublicKey.data(), localKeyPair.privateKey.data()) != 0) {
        throw std::runtime_error("Shared secret computation failed");
    }

    return sharedSecret;
}

void encryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                             const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                             std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag) {
    if (key.size() != crypto_aead_chacha20poly1305_KEYBYTES) {
        throw std::invalid_argument("Invalid key size");
    }
    if (iv.size() != crypto_aead_chacha20poly1305_NPUBBYTES) {
        throw std::invalid_argument("Invalid IV size");
    }

    ciphertext.resize(plaintext.size() + crypto_aead_chacha20poly1305_ABYTES);
    tag.resize(crypto_aead_chacha20poly1305_ABYTES);

    unsigned long long ciphertext_len;

    if (crypto_aead_chacha20poly1305_encrypt_detached(
            ciphertext.data(), tag.data(), &ciphertext_len,
            plaintext.data(), plaintext.size(),
            aad.data(), aad.size(),
            nullptr, iv.data(), key.data()) != 0) {
        throw std::runtime_error("Encryption failed");
    }

    ciphertext.resize(ciphertext_len);
}

std::vector<uint8_t> decryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                             const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                             const std::vector<uint8_t>& tag) {
    if (key.size() != crypto_aead_chacha20poly1305_KEYBYTES) {
        throw std::invalid_argument("Invalid key size");
    }
    if (iv.size() != crypto_aead_chacha20poly1305_NPUBBYTES) {
        throw std::invalid_argument("Invalid IV size");
    }
    if (tag.size() != crypto_aead_chacha20poly1305_ABYTES) {
        throw std::invalid_argument("Invalid tag size");
    }

    std::vector<uint8_t> plaintext(ciphertext.size());
    unsigned long long plaintext_len;

    if (crypto_aead_chacha20poly1305_decrypt_detached(
            plaintext.data(),
            nullptr,
            ciphertext.data(), ciphertext.size(),
            tag.data(),
            aad.data(), aad.size(),
            iv.data(), key.data()) != 0) {
        throw std::runtime_error("Decryption failed");
    }

    // The plaintext size is the same as the ciphertext size in this case
    plaintext_len = ciphertext.size();
    plaintext.resize(plaintext_len);
    return plaintext;
}

} // namespace SRPT