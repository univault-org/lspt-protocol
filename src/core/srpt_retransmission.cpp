#include "srpt_retransmission.h"
#include "srpt_packet.h"

namespace SRPT {

void RetransmissionManager::packetSent(const SRPTPacket& packet) {
    uint32_t sequenceNumber = packet.getSequenceNumber();
    sentPackets.insert(sequenceNumber);
    if (retransmissionCount.find(sequenceNumber) == retransmissionCount.end()) {
        retransmissionCount[sequenceNumber] = 0;
    } else {
        retransmissionCount[sequenceNumber]++;
    }
}

void RetransmissionManager::packetReceived(uint32_t sequenceNumber) {
    receivedPackets.insert(sequenceNumber);
    sentPackets.erase(sequenceNumber);
}

bool RetransmissionManager::needsRetransmission(uint32_t sequenceNumber) const {
    return sentPackets.find(sequenceNumber) != sentPackets.end() &&
           receivedPackets.find(sequenceNumber) == receivedPackets.end();
}

} // namespace SRPT