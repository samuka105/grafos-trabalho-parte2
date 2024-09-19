

#ifndef EDGE_HPP
#define EDGE_HPP

#include <cstddef>

class Edge {
private:
    size_t target_id;   // ID do nó de destino
    float edge_weight;  // Peso da aresta

public:
    // Construtores
    Edge(size_t target_id, float weight);
    Edge() ;  // Inicializa membros com valores padrão

    // Getters
    size_t getTargetId() const;
    float getWeight() const; //

    // Setters
    void setWeight(float weight);
};

#endif 
