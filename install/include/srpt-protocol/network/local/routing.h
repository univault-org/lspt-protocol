#ifndef SRPT_NETWORK_LOCAL_ROUTING_H
#define SRPT_NETWORK_LOCAL_ROUTING_H

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "../node/node.h"  // Updated include path

namespace srpt {
namespace network {
namespace local {

class Route {
public:
    bool isEmpty() const { return nodes.empty(); }
    const std::vector<Node>& getNodes() const { return nodes; }
    std::vector<Node>& getNodesNonConst() { return nodes; } 
    void addNode(const Node& node) { nodes.push_back(node); }

private:
    std::vector<Node> nodes; 
};

class Routing {
public:
    Routing() {}
    Route findRoute(const Node& source, const Node& destination);
    bool isRemoteNode(const Node& node) const;
    
    // New methods
    void addNode(const Node& node);
    void addLink(const Node& node1, const Node& node2);

private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, std::vector<int>> adjacencyList;
};

} // namespace local
} // namespace network
} // namespace srpt

#endif // SRPT_NETWORK_LOCAL_ROUTING_H