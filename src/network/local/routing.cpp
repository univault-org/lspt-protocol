#include "routing.h"
#include <queue>
#include <unordered_set>
#include <stdexcept>
#include <algorithm>

namespace srpt {
namespace network {
namespace local {

Route Routing::findRoute(const Node& source, const Node& destination) {
    // Check if both nodes exist
    if (nodes.find(source.getId()) == nodes.end() || nodes.find(destination.getId()) == nodes.end()) {
        throw std::invalid_argument("Source or destination node does not exist");
    }

    // Check for direct connection first
    auto& sourceNeighbors = adjacencyList[source.getId()];
    if (std::find(sourceNeighbors.begin(), sourceNeighbors.end(), destination.getId()) != sourceNeighbors.end()) {
        Route route;
        route.addNode(nodes[source.getId()]);
        route.addNode(nodes[destination.getId()]);
        return route;
    }

    // If no direct connection, proceed with BFS
    std::unordered_map<int, int> prev;
    std::queue<int> q;
    std::unordered_set<int> visited;

    q.push(source.getId());
    visited.insert(source.getId());

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == destination.getId()) {
            Route route;
            for (int at = destination.getId(); at != source.getId(); at = prev[at]) {
                route.addNode(nodes[at]);
            }
            route.addNode(nodes[source.getId()]);
            std::reverse(route.getNodesNonConst().begin(), route.getNodesNonConst().end()); 
            return route;
        }

        for (int neighbor : adjacencyList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                prev[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    throw std::runtime_error("No route found");
}

bool Routing::isRemoteNode(const Node& node) const {
    return node.getId() > 100; // Assume nodes with ID > 100 are remote
}

void Routing::addNode(const Node& node) {
    if (nodes.find(node.getId()) != nodes.end()) {
        throw std::invalid_argument("Node already exists");
    }
    nodes[node.getId()] = node;
    adjacencyList[node.getId()] = std::vector<int>();
}

void Routing::addLink(const Node& node1, const Node& node2) {
    if (nodes.find(node1.getId()) == nodes.end() || nodes.find(node2.getId()) == nodes.end()) {
        throw std::invalid_argument("One or both nodes do not exist");
    }
    if (std::find(adjacencyList[node1.getId()].begin(), adjacencyList[node1.getId()].end(), node2.getId()) 
        != adjacencyList[node1.getId()].end()) {
        throw std::invalid_argument("Link already exists");
    }
    adjacencyList[node1.getId()].push_back(node2.getId());
    adjacencyList[node2.getId()].push_back(node1.getId());
}

} // namespace local
} // namespace network
} // namespace srpt