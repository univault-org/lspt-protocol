#ifndef SRPT_H
#define SRPT_H

#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <cstdint>
#include <functional>

namespace SRPT {

using ByteVector = std::vector<uint8_t>;

enum class ConnectionState {
    CLOSED,
    CONNECTING,
    ESTABLISHED,
    CLOSING
};

class Config {
public:
    void setKeepAliveInterval(std::chrono::seconds interval);
    void setReceiveWindowSize(uint32_t size);
    void setMaxPacketSize(uint32_t size);
    // Add other configuration options as needed
};

class Stream {
public:
    virtual ~Stream() = default;
    virtual bool Write(const ByteVector& data) = 0;
    virtual bool Read(ByteVector& data) = 0;
    virtual void Close() = 0;
};

class Session {
public:
    Session(const Config& config);
    virtual ~Session();

    bool Connect(const std::string& host, uint16_t port);
    bool Listen(uint16_t port);
    std::unique_ptr<Stream> CreateStream();

    ConnectionState GetState() const;
    bool IsEstablished() const;
    void Close();

    uint32_t GetAvailableWindowSize() const;
    std::string GetLastError() const;

    // Callback setters
    void SetConnectionCallback(std::function<void(bool)> callback);
    void SetErrorCallback(std::function<void(const std::string&)> callback);

    // Statistics
    struct Stats {
        uint64_t bytes_sent;
        uint64_t bytes_received;
        uint64_t packets_sent;
        uint64_t packets_received;
        // Add other relevant statistics
    };
    Stats GetStats() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// Utility functions
uint32_t ComputeChecksum(const ByteVector& data);

} // namespace SRPT

#endif // SRPT_H