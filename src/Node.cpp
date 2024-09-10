#include "Node.hpp"

// Construtor padrão
Node::Node() 
    : _number_of_edges(0), _id(0), _weight(0), _first_edge(nullptr), _next_node(nullptr), _previous_node(nullptr) {}

// Construtor com parâmetros
Node::Node(size_t id, float weight) 
    : _number_of_edges(0), _id(id), _weight(weight), _first_edge(nullptr), _next_node(nullptr), _previous_node(nullptr) {}

// Getter para _number_of_edges
size_t Node::get_number_of_edges() const {
    return _number_of_edges;
}

// Setter para _number_of_edges
void Node::set_number_of_edges(size_t number_of_edges) {
    _number_of_edges = number_of_edges;
}

// Getter para _id
size_t Node::get_id() const {
    return _id;
}

// Setter para _id
void Node::set_id(size_t id) {
    _id = id;
}

// Getter para _weight
float Node::get_weight() const {
    return _weight;
}

// Setter para _weight
void Node::set_weight(float weight) {
    _weight = weight;
}

// Getter para _first_edge
Edge* Node::get_first_edge() const {
    return _first_edge;
}

// Setter para _first_edge
void Node::set_first_edge(Edge* first_edge) {
    _first_edge = first_edge;
}

// Getter para _next_node
Node* Node::get_next_node() const {
    return _next_node;
}

// Setter para _next_node
void Node::set_next_node(Node* next_node) {
    _next_node = next_node;
}

// Getter para _previous_node
Node* Node::get_previous_node() const {
    return _previous_node;
}

// Setter para _previous_node
void Node::set_previous_node(Node* previous_node) {
    _previous_node = previous_node;
}

// Getter para o vetor de arestas _edges
std::vector<Edge>& Node::get_edges() {
    return _edges;
}

// Método para adicionar uma aresta ao vetor _edges
void Node::add_edge(const Edge& edge) {
    _edges.push_back(edge);
}
