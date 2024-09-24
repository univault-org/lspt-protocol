#include "hub.h"
#include <algorithm>

namespace srpt {
namespace network {

void Hub::connectNode(const Node& node) {
    connectedNodes.push_back(node);
}

bool Hub::forwardData(const Node& source, const Node& destination, const std::string& data) {
    // This is a placeholder implementation
    // In a real scenario, this would implement actual data forwarding logic
    auto sourceIt = std::find_if(connectedNodes.begin(), connectedNodes.end(),
                                 [&](const Node& n) { return n.getId() == source.getId(); });
    auto destIt = std::find_if(connectedNodes.begin(), connectedNodes.end(),
                               [&](const Node& n) { return n.getId() == destination.getId(); });
    
    return sourceIt != connectedNodes.end() && destIt != connectedNodes.end();
}

} // namespace network
} // namespace srpt