#ifndef LSPT_NETWORK_NODE_H
#define LSPT_NETWORK_NODE_H

#include <string>
#include <vector>

namespace lspt {
namespace network {

class Node {
public:
    Node();  // Default constructor
    Node(int id);
    Node(const Node& other) = default;  // Copy constructor
    Node(Node&& other) = default;  // Move constructor
    Node& operator=(const Node& other) = default;  // Copy assignment
    Node& operator=(Node&& other) = default;  // Move assignment

    int getId() const;
    void connect(Node* other);
    bool sendData(const Node& destination, const std::string& data);
    bool receiveData(const std::string& data);
    std::vector<Node*> getConnectedNodes() const;

private:
    int id_;
    std::vector<Node*> connectedNodes_;
};

} // namespace network
} // namespace lspt

#endif // LSPT_NETWORK_NODE_H