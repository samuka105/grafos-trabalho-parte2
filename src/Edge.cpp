#include "../include/Edge.hpp"

Edge::Edge(int target_id, float weight)
{
    this->target_id = target_id;
    this->weight = weight;
}

int Edge::getTargetId() {
    return this->target_id;
}

float Edge::getWeight() {
    return this->weight;
}

void Edge::setWeight(float weight) {
    this->weight = weight;
}
