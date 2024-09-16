#include "../include/Node.hpp"
#include "../include/Edge.hpp"
#include <cstddef>

Node::Node(size_t id, float weight)
{
    this->node_id = id;
    this->node_weight = weight;
}

size_t Node::getId()const {
    return node_id;
}

float Node::getNodeWeight()const {
    return node_weight;
}


const std::unordered_map<size_t, Edge>& Node::getEdges() {
    return edges;
}

void Node::setNodeWeight(float weight) {
    this->node_weight = weight;
}

void Node::addEdge(size_t target_id, float weight) {
    edges[target_id] = Edge(target_id, weight);
}

void Node::removeEdge(size_t target_id) {
    edges.erase(target_id);
}
