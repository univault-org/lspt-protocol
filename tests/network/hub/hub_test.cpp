#include <gtest/gtest.h>
#include "network/hub/hub.h"
#include "network/local/routing.h"

using namespace srpt::network;
using namespace srpt::network::local;

TEST(HubTest, CreateHub) {
    Hub hub;
    Node node(1);
    EXPECT_NO_THROW(hub.connectNode(node));
}

TEST(HubTest, ForwardData) {
    Hub hub;
    Node source(1), destination(2);
    hub.connectNode(source);
    hub.connectNode(destination);
    EXPECT_TRUE(hub.forwardData(source, destination, "test data"));
}

TEST(HubTest, ForwardDataFailure) {
    Hub hub;
    Node source(1), destination(2), unconnectedNode(3);
    hub.connectNode(source);
    hub.connectNode(destination);
    EXPECT_FALSE(hub.forwardData(source, unconnectedNode, "test data"));
}
