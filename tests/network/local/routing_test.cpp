#include <gtest/gtest.h>
#include "network/local/routing.h"
#include "network/node/node.h"  
#include <stdexcept>

using namespace srpt::network;  
using namespace srpt::network::local;

class RoutingTest : public ::testing::Test {
protected:
    Routing routing;
};

TEST_F(RoutingTest, CreateRouting) {
    EXPECT_NO_THROW(Routing());
}

TEST_F(RoutingTest, FindDirectRoute) {
    Node source(1), destination(2);
    
    // Add nodes to the routing
    routing.addNode(source);
    routing.addNode(destination);
    
    // Create a direct link between the nodes
    routing.addLink(source, destination);

    // Now find the route
    auto route = routing.findRoute(source, destination);
    
    EXPECT_FALSE(route.isEmpty());
    EXPECT_EQ(route.getNodes().size(), 2);
    EXPECT_EQ(route.getNodes().front().getId(), 1);
    EXPECT_EQ(route.getNodes().back().getId(), 2);
}

TEST_F(RoutingTest, FindMultiHopRoute) {
    Node source(1), intermediate(2), destination(3);
    routing.addNode(source);
    routing.addNode(intermediate);
    routing.addNode(destination);
    routing.addLink(source, intermediate);
    routing.addLink(intermediate, destination);

    auto route = routing.findRoute(source, destination);
    EXPECT_EQ(route.getNodes().size(), 3);
    EXPECT_EQ(route.getNodes()[0].getId(), 1);
    EXPECT_EQ(route.getNodes()[1].getId(), 2);
    EXPECT_EQ(route.getNodes()[2].getId(), 3);
}

TEST_F(RoutingTest, FindNonExistentRoute) {
    Node source(1), destination(2);
    routing.addNode(source);
    routing.addNode(destination);
    // No link between source and destination
    EXPECT_THROW(routing.findRoute(source, destination), std::runtime_error);
}

TEST_F(RoutingTest, RemoteNode) {
    Node localNode(1), remoteNode(999);
    EXPECT_FALSE(routing.isRemoteNode(localNode));
    EXPECT_TRUE(routing.isRemoteNode(remoteNode));
}

TEST_F(RoutingTest, AddDuplicateNode) {
    Node node(1);
    EXPECT_NO_THROW(routing.addNode(node));
    EXPECT_THROW(routing.addNode(node), std::invalid_argument);
}

TEST_F(RoutingTest, AddDuplicateLink) {
    Node node1(1), node2(2);
    routing.addNode(node1);
    routing.addNode(node2);
    EXPECT_NO_THROW(routing.addLink(node1, node2));
    EXPECT_THROW(routing.addLink(node1, node2), std::invalid_argument);
}

TEST_F(RoutingTest, FindShortestRoute) {
    Node a(1), b(2), c(3), d(4);
    routing.addNode(a);
    routing.addNode(b);
    routing.addNode(c);
    routing.addNode(d);
    routing.addLink(a, b);
    routing.addLink(b, d);
    routing.addLink(a, c);
    routing.addLink(c, d);

    auto route = routing.findRoute(a, d);
    EXPECT_EQ(route.getNodes().size(), 3);  // Expecting the shortest path: a -> b -> d
}
