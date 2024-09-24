#include <gtest/gtest.h>
#include "network/node/node.h"

using namespace srpt::network;

TEST(NodeTest, CreateNode) {
    Node node(1);
    EXPECT_EQ(node.getId(), 1);
}

TEST(NodeTest, ConnectNodes) {
    Node node1(1);
    Node node2(2);
    node1.connect(&node2);
    EXPECT_EQ(node1.getConnectedNodes().size(), 1);
    EXPECT_EQ(node2.getConnectedNodes().size(), 1);
}

TEST(NodeTest, SendData) {
    Node node1(1);
    Node node2(2);
    node1.connect(&node2);
    EXPECT_TRUE(node1.sendData(node2, "test data"));
}

TEST(NodeTest, SendDataFailure) {
    Node node1(1);
    Node node2(2);
    Node node3(3);
    node1.connect(&node2);
    EXPECT_FALSE(node1.sendData(node3, "test data"));
}
