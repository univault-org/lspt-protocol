#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class SRPTPackage {
public:
    explicit SRPTPackage(const std::vector<uint8_t>& data);  // Make sure this constructor is declared
    explicit SRPTPackage(size_t size);
    uint64_t getSize() const;
    std::string getId() const;
    void setMetadata(const std::string& key, const std::string& value);
    std::string getMetadata(const std::string& key) const;
    const std::vector<uint8_t>& getData() const;

private:
    void generateId();
    std::vector<uint8_t> data;
    std::string id;
    std::unordered_map<std::string, std::string> metadata;
};