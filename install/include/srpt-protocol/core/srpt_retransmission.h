#pragma once

#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include "srpt_packet.h" 

namespace SRPT {

class RetransmissionManager {
public:
    void packetSent(const SRPTPacket& packet);
    void packetReceived(uint32_t sequenceNumber);
    bool needsRetransmission(uint32_t sequenceNumber) const;

private:
    std::unordered_set<uint32_t> sentPackets;
    std::unordered_set<uint32_t> receivedPackets;
    std::unordered_map<uint32_t, int> retransmissionCount;
};

} // namespace SRPT