#ifndef GRAFO_BASICO_EDGE_H
#define GRAFO_BASICO_EDGE_H

#include <cstddef>

class Edge {
public:
    // Construtor padrão
    Edge() : _next_edge(nullptr), _weight(0), _target_id(0), _source_id(0) {}

    // Construtor com parâmetros
    Edge(float weight, size_t target_id, size_t source_id) 
        : _next_edge(nullptr), _weight(weight), _target_id(target_id), _source_id(source_id) {}

    // Métodos getters e setters para os atributos
    Edge* get_next_edge() const { return _next_edge; }
    void set_next_edge(Edge* next_edge) { _next_edge = next_edge; }

    float get_weight() const { return _weight; }
    void set_weight(float weight) { _weight = weight; }

    size_t get_target_id() const { return _target_id; }
    void set_target_id(size_t target_id) { _target_id = target_id; }

    size_t get_source_id() const { return _source_id; }
    void set_source_id(size_t source_id) { _source_id = source_id; }

private:
    Edge  *_next_edge;  // Ponteiro para a próxima aresta (caso esteja implementando uma lista de adjacência)
    float  _weight;     // Peso da aresta
    size_t _target_id;  // ID do nó de destino
    size_t _source_id;  // ID do nó de origem
};

#endif /* GRAFO_BASICO_EDGE_H */

