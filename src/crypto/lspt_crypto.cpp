#include "lspt_crypto.h"
#include <openssl/evp.h>
#include <stdexcept>

namespace LSPT {

void encryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                   const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                   std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr) != 1)
        throw std::runtime_error("Failed to initialize AES-GCM encryption");

    if (EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("Failed to set AES-GCM key and IV");

    int len;
    if (EVP_EncryptUpdate(ctx, nullptr, &len, aad.data(), aad.size()) != 1)
        throw std::runtime_error("Failed to set AES-GCM AAD");

    ciphertext.resize(plaintext.size());
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()) != 1)
        throw std::runtime_error("Failed to encrypt plaintext");
    int ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1)
        throw std::runtime_error("Failed to finalize AES-GCM encryption");
    ciphertext_len += len;

    tag.resize(16);
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag.data()) != 1)
        throw std::runtime_error("Failed to get AES-GCM tag");

    EVP_CIPHER_CTX_free(ctx);
}

std::vector<uint8_t> decryptAESGCM(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                   const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                   const std::vector<uint8_t>& tag) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr) != 1)
        throw std::runtime_error("Failed to initialize AES-GCM decryption");

    if (EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("Failed to set AES-GCM key and IV");

    int len;
    if (EVP_DecryptUpdate(ctx, nullptr, &len, aad.data(), aad.size()) != 1)
        throw std::runtime_error("Failed to set AES-GCM AAD");

    std::vector<uint8_t> plaintext(ciphertext.size());
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1)
        throw std::runtime_error("Failed to decrypt ciphertext");
    int plaintext_len = len;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, const_cast<uint8_t*>(tag.data())) != 1)
        throw std::runtime_error("Failed to set AES-GCM tag");

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1)
        throw std::runtime_error("Failed to finalize AES-GCM decryption");
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

void encryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                             const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad,
                             std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& tag) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

    if (EVP_EncryptInit_ex(ctx, EVP_chacha20_poly1305(), nullptr, nullptr, nullptr) != 1)
        throw std::runtime_error("Failed to initialize ChaCha20-Poly1305 encryption");

    if (EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("Failed to set ChaCha20-Poly1305 key and IV");

    int len;
    if (EVP_EncryptUpdate(ctx, nullptr, &len, aad.data(), aad.size()) != 1)
        throw std::runtime_error("Failed to set ChaCha20-Poly1305 AAD");

    ciphertext.resize(plaintext.size());
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()) != 1)
        throw std::runtime_error("Failed to encrypt plaintext");
    int ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1)
        throw std::runtime_error("Failed to finalize ChaCha20-Poly1305 encryption");
    ciphertext_len += len;

    tag.resize(16);
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_GET_TAG, 16, tag.data()) != 1)
        throw std::runtime_error("Failed to get ChaCha20-Poly1305 tag");

    EVP_CIPHER_CTX_free(ctx);
}

std::vector<uint8_t> decryptChaCha20Poly1305(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv,
                                             const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad,
                                             const std::vector<uint8_t>& tag) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

    if (EVP_DecryptInit_ex(ctx, EVP_chacha20_poly1305(), nullptr, nullptr, nullptr) != 1)
        throw std::runtime_error("Failed to initialize ChaCha20-Poly1305 decryption");

    if (EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("Failed to set ChaCha20-Poly1305 key and IV");

    int len;
    if (EVP_DecryptUpdate(ctx, nullptr, &len, aad.data(), aad.size()) != 1)
        throw std::runtime_error("Failed to set ChaCha20-Poly1305 AAD");

    std::vector<uint8_t> plaintext(ciphertext.size());
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1)
        throw std::runtime_error("Failed to decrypt ciphertext");
    int plaintext_len = len;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG, 16, const_cast<uint8_t*>(tag.data())) != 1)
        throw std::runtime_error("Failed to set ChaCha20-Poly1305 tag");

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1)
        throw std::runtime_error("Failed to finalize ChaCha20-Poly1305 decryption");
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

} // namespace LSPT