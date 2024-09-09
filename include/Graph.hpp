#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include "Node.hpp"
#include "Edge.hpp"

class Graph {
private:
    std::unordered_map<int, Node> nodes;  // Mapa de nós (id -> Node)

public:
    // Adiciona um nó ao grafo
    void addNode(int id, float weight = 1.0f);

    // Adiciona uma aresta ao grafo
    void addEdge(int source, int target, float weight = 1.0f);

    // Obtém um nó pelo ID
    Node* getNode(int id);

    // Algoritmos para particionamento
    int Greedy();
    int GreedyRandomizedAdaptative();
    int GreedyRandomizedAdaptativeReactive();

    // Funções auxiliares para impressão dos resultados
    void printGreedy();
    void printGreedyRandomizedAdaptative();
    void printGreedyRandomizedAdaptativeReactive();

};

#endif // GRAPH_HPP
