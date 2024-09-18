#include "lspt_package.h"
#include <random>
#include <sstream>
#include <iomanip>

LSPTPackage::LSPTPackage(const std::vector<uint8_t>& data) : data(data) {
    generateId();
}

LSPTPackage::LSPTPackage(size_t size) : data(size) {
    generateId();
}

void LSPTPackage::generateId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::stringstream ss;
    for (int i = 0; i < 32; i++) {
        ss << std::hex << dis(gen);
    }
    id = ss.str();
}

uint64_t LSPTPackage::getSize() const {
    return data.size();
}

std::string LSPTPackage::getId() const {
    return id;
}

void LSPTPackage::setMetadata(const std::string& key, const std::string& value) {
    metadata[key] = value;
}

std::string LSPTPackage::getMetadata(const std::string& key) const {
    auto it = metadata.find(key);
    if (it != metadata.end()) {
        return it->second;
    }
    return "";
}

const std::vector<uint8_t>& LSPTPackage::getData() const {
    return data;
}