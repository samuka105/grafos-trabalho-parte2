#ifndef NODE_HPP
#define NODE_HPP

#include "Edge.hpp"

#include <cstddef>
#include <unordered_map>
#include <iostream>


class Node {
public:
    Node(size_t id, float weight);

    size_t getId() const; 
    float getWeight() const;
    void setNodeWeight(float weight); 

    void addEdge(size_t to); 
    const std::unordered_map<size_t, Edge>& getEdges() const;  

    bool hasEdge(size_t to) const; 
    void removeEdge(size_t to); 

private:
    size_t id; 
    float weight; 
    std::unordered_map<size_t, Edge> edges; 
};

#endif 
