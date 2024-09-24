#include "node.h"
#include <algorithm>

namespace srpt {
namespace network {

Node::Node() : id_(-1) {}

Node::Node(int id) : id_(id) {}

int Node::getId() const {
    return id_;
}

void Node::connect(Node* other) {
    if (std::find(connectedNodes_.begin(), connectedNodes_.end(), other) == connectedNodes_.end()) {
        connectedNodes_.push_back(other);
        other->connect(this);
    }
}

bool Node::sendData(const Node& destination, const std::string& data) {
    // This is a placeholder implementation
    // In a real scenario, this would implement actual data sending logic
    auto it = std::find_if(connectedNodes_.begin(), connectedNodes_.end(),
                           [&](const Node* n) { return n->getId() == destination.getId(); });
    if (it != connectedNodes_.end()) {
        return (*it)->receiveData(data);
    }
    return false;
}

bool Node::receiveData(const std::string& data) {
    // This is a placeholder implementation
    // In a real scenario, this would process the received data
    return true;
}

std::vector<Node*> Node::getConnectedNodes() const {
    return connectedNodes_;
}

} // namespace network
} // namespace srpt