#include "Edge.hpp"

// Construtor padrão
Edge::Edge() : _next_edge(nullptr), _weight(0), _target_id(0), _source_id(0) {}

// Construtor com parâmetros
Edge::Edge(float weight, size_t target_id, size_t source_id)
    : _next_edge(nullptr), _weight(weight), _target_id(target_id), _source_id(source_id) {}

// Getter para _next_edge
Edge* Edge::get_next_edge() const {
    return _next_edge;
}

// Setter para _next_edge
void Edge::set_next_edge(Edge* next_edge) {
    _next_edge = next_edge;
}

// Getter para _weight
float Edge::get_weight() const {
    return _weight;
}

// Setter para _weight
void Edge::set_weight(float weight) {
    _weight = weight;
}

// Getter para _target_id
size_t Edge::get_target_id() const {
    return _target_id;
}

// Setter para _target_id
void Edge::set_target_id(size_t target_id) {
    _target_id = target_id;
}

// Getter para _source_id
size_t Edge::get_source_id() const {
    return _source_id;
}

// Setter para _source_id
void Edge::set_source_id(size_t source_id) {
    _source_id = source_id;
}
