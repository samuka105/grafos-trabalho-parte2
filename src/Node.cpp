#include "../include/Node.hpp"

Node::Node(int id, float weight)
{
    this->id = id;
    this->weight = weight;
}

int Node::getId() {
    return this->id;
}

float Node::getWeight() {
    return this->weight;
}

std::unordered_map<int, Edge>& Node::getEdges() {
    return this->edges;
}

void Node::setWeight(float weight) {
    this->weight = weight;
}

void Node::addEdge(const Edge& edge) {
    
}

void Node::removeEdge(int target_id) {
    
}
