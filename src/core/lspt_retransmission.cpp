#include "lspt_retransmission.h"
#include "lspt_packet.h"

namespace LSPT {

void RetransmissionManager::packetSent(const LSPTPacket& packet) {
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

} // namespace LSPT