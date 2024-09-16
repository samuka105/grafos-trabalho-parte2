#include "../include/Edge.hpp"

Edge::Edge(size_t id, float weight)
{
    this->target_id = id;
    this->edge_weight = weight;
}

Edge::Edge() {
    this->target_id = 0;
    this->edge_weight = 1.0f;
}

int Edge::getTargetId() {
    return this->target_id;
}

float Edge::getWeight() {
    return this->edge_weight;
}

void Edge::setWeight(float weight) {
    this->edge_weight = weight;
}
