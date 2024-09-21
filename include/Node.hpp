#ifndef NODE_HPP
#define NODE_HPP

#include <cstddef>
#include <unordered_map>
#include "Edge.hpp"

class Node {
public:
    Node(size_t id, float weight);

    size_t getId() const; // Retorna o ID do nó
    float getWeight() const;// Retorna o peso do nó
    void setNodeWeight(float weight); // Define o peso do nó

    void addEdge(size_t to); // Adiciona uma aresta para outro nó
    const std::unordered_map<size_t, Edge>& getEdges() const;  // Retorna as arestas

    bool edgeExists(size_t to) const; // Verifica se uma aresta para o nó `to` existe
    void removeEdge(size_t to); // Remove uma aresta para o nó `to`

private:
    size_t id; // Identificador do nó
    float weight; // Peso do nó
    std::unordered_map<size_t, Edge> edges; // Arestas conectadas ao nó
};

#endif // NODE_HPP
