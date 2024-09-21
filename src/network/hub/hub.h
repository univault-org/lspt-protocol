#ifndef LSPT_NETWORK_HUB_H
#define LSPT_NETWORK_HUB_H

#include <vector>
#include <string>
#include "../node/node.h"

namespace lspt {
namespace network {

class Hub {
public:
    void connectNode(const Node& node);
    bool forwardData(const Node& source, const Node& destination, const std::string& data);

private:
    std::vector<Node> connectedNodes;
};

} // namespace network
} // namespace lspt

#endif // LSPT_NETWORK_HUB_H