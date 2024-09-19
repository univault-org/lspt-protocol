#include <gtest/gtest.h>
#include "../../src/crypto/lspt_crypto.h"
#include <vector>
#include <cstdint>
#include <stdexcept>

TEST(LSPTCryptoTest, EncryptDecryptAESGCM) {
    std::vector<uint8_t> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    std::vector<uint8_t> iv = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B};
    std::vector<uint8_t> plaintext = {'H', 'e', 'l', 'l', 'o'};
    std::vector<uint8_t> aad = {'A', 'D', 'D', 'I', 'T', 'I', 'O', 'N', 'A', 'L'};
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> tag;

    LSPT::encryptAESGCM(key, iv, plaintext, aad, ciphertext, tag);

    std::vector<uint8_t> decrypted;
    EXPECT_NO_THROW(decrypted = LSPT::decryptAESGCM(key, iv, ciphertext, aad, tag));
    EXPECT_EQ(plaintext, decrypted);
}

TEST(LSPTCryptoTest, DecryptAESGCMWithTamperedData) {
    std::vector<uint8_t> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    std::vector<uint8_t> iv = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B};
    std::vector<uint8_t> plaintext = {'H', 'e', 'l', 'l', 'o'};
    std::vector<uint8_t> aad = {'A', 'D', 'D', 'I', 'T', 'I', 'O', 'N', 'A', 'L'};
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> tag;

    LSPT::encryptAESGCM(key, iv, plaintext, aad, ciphertext, tag);

    // Tamper with the ciphertext
    ciphertext[0] ^= 0xFF;

    std::vector<uint8_t> decrypted;
    EXPECT_THROW(decrypted = LSPT::decryptAESGCM(key, iv, ciphertext, aad, tag), std::runtime_error);
}

TEST(LSPTCryptoTest, EncryptDecryptChaCha20Poly1305) {
    std::vector<uint8_t> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
    std::vector<uint8_t> iv = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B};
    std::vector<uint8_t> plaintext = {'H', 'e', 'l', 'l', 'o'};
    std::vector<uint8_t> aad = {'A', 'D', 'D', 'I', 'T', 'I', 'O', 'N', 'A', 'L'};
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> tag;

    LSPT::encryptChaCha20Poly1305(key, iv, plaintext, aad, ciphertext, tag);

    std::vector<uint8_t> decrypted;
    EXPECT_NO_THROW(decrypted = LSPT::decryptChaCha20Poly1305(key, iv, ciphertext, aad, tag));
    EXPECT_EQ(plaintext, decrypted);
}

TEST(LSPTCryptoTest, DecryptChaCha20Poly1305WithTamperedData) {
    std::vector<uint8_t> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
    std::vector<uint8_t> iv = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B};
    std::vector<uint8_t> plaintext = {'H', 'e', 'l', 'l', 'o'};
    std::vector<uint8_t> aad = {'A', 'D', 'D', 'I', 'T', 'I', 'O', 'N', 'A', 'L'};
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> tag;

    LSPT::encryptChaCha20Poly1305(key, iv, plaintext, aad, ciphertext, tag);

    // Tamper with the ciphertext
    ciphertext[0] ^= 0xFF;

    std::vector<uint8_t> decrypted;
    EXPECT_THROW(decrypted = LSPT::decryptChaCha20Poly1305(key, iv, ciphertext, aad, tag), std::runtime_error);
}