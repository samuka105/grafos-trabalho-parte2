


#include "../include/Edge.hpp"

// Construtor com parâmetros
Edge::Edge(size_t target_id, float weight)
    : target_id(target_id), edge_weight(weight) {}

// Construtor padrão

Edge::Edge() : target_id(0), edge_weight(1.0f) {}
// Getter para target_id
size_t Edge::getTargetId() const {
    return target_id;
}

// Getter para edge_weight
float Edge::getWeight() const {
    return edge_weight;
}

// Setter para edge_weight
void Edge::setWeight(float weight) {
    edge_weight = weight;
}
