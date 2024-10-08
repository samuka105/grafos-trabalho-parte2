#include "../include/Node.hpp"

Node::Node(size_t id, float weight) : id(id), weight(weight) {}

size_t Node::getId() const {
    return id;
}

float Node::getWeight() const {
    return weight;
}

void Node::setNodeWeight(float weight) {
    this->weight = weight;
}

void Node::addEdge(size_t to) {
    if (edges.find(to) == edges.end()) {
        edges[to] = Edge(id, to);
    }
}

const std::unordered_map<size_t, Edge>& Node::getEdges() const {
    return edges;
}

bool Node::hasEdge(size_t to) const {
    return edges.find(to) != edges.end();
}

void Node::removeEdge(size_t to) {
    edges.erase(to);
}

