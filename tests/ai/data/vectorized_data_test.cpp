#include <gtest/gtest.h>
#include "ai/data/vectorized_data_handler.h"
#include "crypto/encryption.h"
#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include <limits>
#include <cmath>

using json = nlohmann::json;


class VectorizedDataTest : public ::testing::Test {
protected:
    VectorizedDataHandler handler;
    Encryption encryptor;
    
    json createOpenAIEmbedding(size_t size) {
        json embedding;
        embedding["object"] = "embedding";
        embedding["model"] = "text-embedding-ada-002";
        embedding["data"] = json::array();
        
        json dataItem;
        dataItem["object"] = "embedding";
        dataItem["index"] = 0;
        dataItem["embedding"] = generateRandomVector(size);
        
        embedding["data"].push_back(dataItem);
        embedding["usage"]["prompt_tokens"] = 5;
        embedding["usage"]["total_tokens"] = 5;
        
        return embedding;
    }
    
    json createLlamaEmbedding(size_t size) {
        json embedding;
        embedding["request_id"] = "abc12345";
        embedding["embeddings"] = json::array();
        
        json dataItem;
        dataItem["input_text"] = "The quick brown fox";
        dataItem["vector"] = generateRandomVector(size);
        
        embedding["embeddings"].push_back(dataItem);
        
        return embedding;
    }
    
    std::vector<float> generateRandomVector(size_t size) {
        std::vector<float> vec(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        for (auto& element : vec) {
            element = dis(gen);
        }
        return vec;
    }
};

TEST_F(VectorizedDataTest, SerializeDeserializeOpenAIEmbedding) {
    json originalData = createOpenAIEmbedding(1536);
    auto serialized = handler.serializeOpenAI(originalData);
    auto deserialized = handler.deserializeOpenAI(serialized);
    
    ASSERT_EQ(originalData["data"][0]["embedding"].size(), deserialized["data"][0]["embedding"].size());
    for (size_t i = 0; i < originalData["data"][0]["embedding"].size(); ++i) {
        EXPECT_FLOAT_EQ(originalData["data"][0]["embedding"][i], deserialized["data"][0]["embedding"][i]);
    }
}

TEST_F(VectorizedDataTest, SerializeDeserializeLlamaEmbedding) {
    json originalData = createLlamaEmbedding(4096);
    auto serialized = handler.serializeLlama(originalData);
    auto deserialized = handler.deserializeLlama(serialized);
    
    ASSERT_EQ(originalData["embeddings"][0]["vector"].size(), deserialized["embeddings"][0]["vector"].size());
    for (size_t i = 0; i < originalData["embeddings"][0]["vector"].size(); ++i) {
        EXPECT_FLOAT_EQ(originalData["embeddings"][0]["vector"][i], deserialized["embeddings"][0]["vector"][i]);
    }
}

TEST_F(VectorizedDataTest, CompressDecompressOpenAIEmbedding) {
    json originalData = createOpenAIEmbedding(1536);
    auto compressed = handler.compressOpenAI(originalData);
    auto decompressed = handler.decompressOpenAI(compressed);
    
    ASSERT_EQ(originalData["data"][0]["embedding"].size(), decompressed["data"][0]["embedding"].size());
    for (size_t i = 0; i < originalData["data"][0]["embedding"].size(); ++i) {
        EXPECT_FLOAT_EQ(originalData["data"][0]["embedding"][i], decompressed["data"][0]["embedding"][i]);
    }
    EXPECT_LT(compressed.size(), originalData.dump().size());
}

TEST_F(VectorizedDataTest, CompressDecompressLlamaEmbedding) {
    json originalData = createLlamaEmbedding(4096);
    auto compressed = handler.compressLlama(originalData);
    auto decompressed = handler.decompressLlama(compressed);
    
    ASSERT_EQ(originalData["embeddings"][0]["vector"].size(), decompressed["embeddings"][0]["vector"].size());
    for (size_t i = 0; i < originalData["embeddings"][0]["vector"].size(); ++i) {
        EXPECT_FLOAT_EQ(originalData["embeddings"][0]["vector"][i], decompressed["embeddings"][0]["vector"][i]);
    }
    EXPECT_LT(compressed.size(), originalData.dump().size());
}

TEST_F(VectorizedDataTest, EncryptDecryptOpenAIEmbedding) {
    json originalData = createOpenAIEmbedding(1536);
    auto key = encryptor.generateKey();
    auto encrypted = encryptor.encrypt(handler.serializeOpenAI(originalData), key);
    auto decrypted = encryptor.decrypt(encrypted, key);
    auto decryptedData = handler.deserializeOpenAI(decrypted);
    
    ASSERT_EQ(originalData["data"][0]["embedding"].size(), decryptedData["data"][0]["embedding"].size());
    for (size_t i = 0; i < originalData["data"][0]["embedding"].size(); ++i) {
        EXPECT_FLOAT_EQ(originalData["data"][0]["embedding"][i], decryptedData["data"][0]["embedding"][i]);
    }
}

TEST_F(VectorizedDataTest, TransmitLargeVectorizedData) {
    json originalData = createLlamaEmbedding(1000000);
    auto serialized = handler.serializeLlama(originalData);
    auto compressed = handler.compressLlama(serialized);
    
    // Simulate transmission in chunks
    const size_t chunkSize = 1024 * 1024; // 1MB chunks
    std::vector<char> receivedData;
    for (size_t i = 0; i < compressed.size(); i += chunkSize) {
        auto chunk = std::vector<char>(
            compressed.begin() + i,
            compressed.begin() + std::min(i + chunkSize, compressed.size())
        );
        // Simulate network transmission
        receivedData.insert(receivedData.end(), chunk.begin(), chunk.end());
    }
    
    auto decompressed = handler.decompressLlama(receivedData);
    auto finalData = handler.deserializeLlama(decompressed);
    
    ASSERT_EQ(originalData["embeddings"][0]["vector"].size(), finalData["embeddings"][0]["vector"].size());
    for (size_t i = 0; i < originalData["embeddings"][0]["vector"].size(); ++i) {
        EXPECT_FLOAT_EQ(originalData["embeddings"][0]["vector"][i], finalData["embeddings"][0]["vector"][i]);
    }
}

TEST_F(VectorizedDataTest, HandleMixedPrecisionData) {
    std::vector<float> floatData = generateRandomVector(1000000);
    std::vector<double> doubleData(floatData.begin(), floatData.end());
    
    auto serializedFloat = handler.serialize(floatData);
    auto serializedDouble = handler.serialize(doubleData);
    
    auto deserializedFloat = handler.deserialize<float>(serializedFloat);
    auto deserializedDouble = handler.deserialize<double>(serializedDouble);
    
    ASSERT_EQ(floatData.size(), deserializedFloat.size());
    ASSERT_EQ(doubleData.size(), deserializedDouble.size());
    
    for (size_t i = 0; i < floatData.size(); ++i) {
        EXPECT_FLOAT_EQ(floatData[i], deserializedFloat[i]);
        EXPECT_DOUBLE_EQ(doubleData[i], deserializedDouble[i]);
    }
}

TEST_F(VectorizedDataTest, PartialProcessing) {
    std::vector<float> originalData = generateRandomVector(1000000);
    auto serialized = handler.serialize(originalData);
    
    // Simulate processing in parts
    const size_t partSize = 250000;
    for (size_t i = 0; i < originalData.size(); i += partSize) {
        auto partSerialized = std::vector<char>(
            serialized.begin() + i * sizeof(float),
            serialized.begin() + std::min((i + partSize) * sizeof(float), serialized.size())
        );
        auto partDeserialized = handler.deserialize<float>(partSerialized);
        
        ASSERT_EQ(partDeserialized.size(), std::min(partSize, originalData.size() - i));
        for (size_t j = 0; j < partDeserialized.size(); ++j) {
            EXPECT_FLOAT_EQ(originalData[i + j], partDeserialized[j]);
        }
    }
}

TEST_F(VectorizedDataTest, Quantize16Bit) {
    std::vector<float> originalData = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};
    auto quantized = handler.quantize16Bit(originalData);
    auto dequantized = handler.dequantize16Bit(quantized);
    
    ASSERT_EQ(originalData.size(), dequantized.size());
    for (size_t i = 0; i < originalData.size(); ++i) {
        EXPECT_NEAR(originalData[i], dequantized[i], 0.001f);
    }
    EXPECT_EQ(quantized.size() * sizeof(uint16_t), originalData.size() * sizeof(float) / 2);
}
TEST_F(VectorizedDataTest, Quantize16BitLargeVector) {
    std::vector<float> originalData = generateRandomVector(1000000);
    auto quantized = handler.quantize16Bit(originalData);
    auto dequantized = handler.dequantize16Bit(quantized);
    
    ASSERT_EQ(originalData.size(), dequantized.size());
    float maxError = 0.0f;
    for (size_t i = 0; i < originalData.size(); ++i) {
        float error = std::abs(originalData[i] - dequantized[i]);
        maxError = std::max(maxError, error);
        EXPECT_NEAR(originalData[i], dequantized[i], 0.001f);
    }
    EXPECT_LT(maxError, 0.001f);
    EXPECT_EQ(quantized.size() * sizeof(uint16_t), originalData.size() * sizeof(float) / 2);
}

TEST_F(VectorizedDataTest, Quantize16BitEdgeCases) {
    std::vector<float> edgeCases = {
        0.0f,
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::epsilon(),
        -std::numeric_limits<float>::epsilon()
    };
    auto quantized = handler.quantize16Bit(edgeCases);
    auto dequantized = handler.dequantize16Bit(quantized);
    
    ASSERT_EQ(edgeCases.size(), dequantized.size());
    for (size_t i = 0; i < edgeCases.size(); ++i) {
        if (std::isfinite(edgeCases[i])) {
            EXPECT_NEAR(edgeCases[i], dequantized[i], std::abs(edgeCases[i]) * 0.01f);
        } else {
            EXPECT_EQ(std::isfinite(edgeCases[i]), std::isfinite(dequantized[i]));
        }
    }
}

TEST_F(VectorizedDataTest, Quantize16BitOpenAIEmbedding) {
    json originalData = createOpenAIEmbedding(1536);
    auto quantized = handler.quantizeOpenAI16Bit(originalData);
    auto dequantized = handler.dequantizeOpenAI16Bit(quantized);
    
    ASSERT_EQ(originalData["data"][0]["embedding"].size(), dequantized["data"][0]["embedding"].size());
    for (size_t i = 0; i < originalData["data"][0]["embedding"].size(); ++i) {
        EXPECT_NEAR(originalData["data"][0]["embedding"][i].get<float>(), 
                    dequantized["data"][0]["embedding"][i].get<float>(), 0.001f);
    }
    EXPECT_LT(quantized.size(), originalData.dump().size());
}

TEST_F(VectorizedDataTest, Quantize16BitLlamaEmbedding) {
    json originalData = createLlamaEmbedding(4096);
    auto quantized = handler.quantizeLlama16Bit(originalData);
    auto dequantized = handler.dequantizeLlama16Bit(quantized);
    
    ASSERT_EQ(originalData["embeddings"][0]["vector"].size(), dequantized["embeddings"][0]["vector"].size());
    for (size_t i = 0; i < originalData["embeddings"][0]["vector"].size(); ++i) {
        EXPECT_NEAR(originalData["embeddings"][0]["vector"][i].get<float>(), 
                    dequantized["embeddings"][0]["vector"][i].get<float>(), 0.001f);
    }
    EXPECT_LT(quantized.size(), originalData.dump().size());
}
