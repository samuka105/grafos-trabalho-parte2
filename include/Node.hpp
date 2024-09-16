#ifndef NODE_HPP
#define NODE_HPP

#include <unordered_map>
#include <cstddef> 
#include "Edge.hpp"

class Node {
private:
    size_t node_id; 
    float node_weight; 
    std::unordered_map<size_t, Edge> edges; // Mapa de arestas (target_id -> Edge)

public:
    // Construtor
    Node(size_t id, float weight = 1.0f);

    // Getters
    size_t getId()const;
    //float getWeight() const;
   
    float getNodeWeight()const;
    const std::unordered_map<size_t, Edge>& getEdges();

    // Setters
    void setNodeWeight(float weight);
    void addEdge(size_t target_id, float weight);
    void removeEdge(size_t target_id);
};

#endif 
