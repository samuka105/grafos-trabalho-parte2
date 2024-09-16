#ifndef EDGE_HPP
#define EDGE_HPP

#include <cstddef>
class Edge {
private:
    size_t target_id; 
    float edge_weight;  

public:
    // Construtor
    Edge(size_t target_id, float weight);
    Edge();

    // Getters
    int getTargetId();
    float getWeight();

    // Setters
    void setWeight(float weight);
};

#endif 
