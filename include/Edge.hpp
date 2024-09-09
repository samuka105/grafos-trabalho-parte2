#ifndef EDGE_HPP
#define EDGE_HPP

class Edge {
private:
    int target_id; // ID do vértice de destino
    float weight;  // Peso da aresta

public:
    // Construtor
    Edge(int target_id, float weight = 1.0f);

    // Getters
    int getTargetId();
    float getWeight();

    // Setters
    void setWeight(float weight);
};

#endif 
