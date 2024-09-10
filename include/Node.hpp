#ifndef GRAFO_BASICO_NODE_H
#define GRAFO_BASICO_NODE_H

#include "Edge.hpp"

#include <vector>

class Node {
public:
    // Construtor padrão
    Node() : _number_of_edges(0), _id(0), _weight(0), _first_edge(nullptr), _next_node(nullptr), _previous_node(nullptr) {}

    // Construtor com parâmetros
    Node(size_t id, float weight) 
        : _number_of_edges(0), _id(id), _weight(weight), _first_edge(nullptr), _next_node(nullptr), _previous_node(nullptr) {}

    // Getters e Setters
    size_t get_number_of_edges() const { return _number_of_edges; }
    void set_number_of_edges(size_t number_of_edges) { _number_of_edges = number_of_edges; }

    size_t get_id() const { return _id; }
    void set_id(size_t id) { _id = id; }

    float get_weight() const { return _weight; }
    void set_weight(float weight) { _weight = weight; }

    Edge* get_first_edge() const { return _first_edge; }
    void set_first_edge(Edge* first_edge) { _first_edge = first_edge; }

    Node* get_next_node() const { return _next_node; }
    void set_next_node(Node* next_node) { _next_node = next_node; }

    Node* get_previous_node() const { return _previous_node; }
    void set_previous_node(Node* previous_node) { _previous_node = previous_node; }
    //acessa leitura e escrita
    std::vector<Edge>& get_edges() { return _edges; }
    //acesso leitura
    const std::vector<Edge>& get_edges()const { return _edges; }

    void add_edge(const Edge& edge) { _edges.push_back(edge); }

private:
    size_t _number_of_edges;     // Número de arestas conectadas ao nó
    size_t _id;                  // Identificador do nó
    float _weight;               // Peso do nó
    Edge *_first_edge;           // Ponteiro para a primeira aresta (caso use uma lista de adjacência)
    Node *_next_node;            // Ponteiro para o próximo nó (caso use lista ligada de nós)
    Node *_previous_node;        // Ponteiro para o nó anterior (caso use lista duplamente ligada)
    std::vector<Edge> _edges;    // Vetor de arestas conectadas ao nó
};

#endif // GRAFO_BASICO_NODE_H
