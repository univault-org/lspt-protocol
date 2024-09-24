#include "srpt_package.h"
#include <random>
#include <sstream>
#include <iomanip>

SRPTPackage::SRPTPackage(const std::vector<uint8_t>& data) : data(data) {
    generateId();
}

SRPTPackage::SRPTPackage(size_t size) : data(size) {
    generateId();
}

void SRPTPackage::generateId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::stringstream ss;
    for (int i = 0; i < 32; i++) {
        ss << std::hex << dis(gen);
    }
    id = ss.str();
}

uint64_t SRPTPackage::getSize() const {
    return data.size();
}

std::string SRPTPackage::getId() const {
    return id;
}

void SRPTPackage::setMetadata(const std::string& key, const std::string& value) {
    metadata[key] = value;
}

std::string SRPTPackage::getMetadata(const std::string& key) const {
    auto it = metadata.find(key);
    if (it != metadata.end()) {
        return it->second;
    }
    return "";
}

const std::vector<uint8_t>& SRPTPackage::getData() const {
    return data;
}