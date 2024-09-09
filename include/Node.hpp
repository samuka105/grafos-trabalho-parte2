#ifndef NODE_HPP
#define NODE_HPP

#include <unordered_map>
#include "Edge.hpp"

class Node {
private:
    int id;                      // Identificador do nó
    float weight;                // Peso do nó
    std::unordered_map<int, Edge> edges; // Mapa de arestas (target_id -> Edge)

public:
    // Construtor
    Node(int id, float weight = 1.0f);

    // Getters
    int getId();
    float getWeight();
    std::unordered_map<int, Edge>& getEdges();

    // Setters
    void setWeight(float weight);
    void addEdge(const Edge& edge);
    void removeEdge(int target_id);
};

#endif // NODE_HPP
