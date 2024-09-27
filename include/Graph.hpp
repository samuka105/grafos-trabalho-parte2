#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Node.hpp"
#include <unordered_map>
#include <memory>
#include <vector>


struct Candidate {
    size_t vertex;
    float weight;
};

struct Solution {
    std::vector<std::vector<size_t>> subgraphs;
    double total_gap;
};

class Graph {
public:
    Graph() = default;
    Graph(const std::string& filename);  // Construtor que lê o arquivo de entrada
    bool readInstance(const std::string& filename); // Função para ler instância
    

    void addNode(size_t id, float weight); // Adiciona um nó ao grafo
    void addEdge(size_t from, size_t to);  // Adiciona uma aresta ao grafo


    void printNodes() const;  // Imprime os nós
    void printEdges() const;  // Imprime as arestas
    void printAdjList() const;
    void printk() const;
    void printClusters(const std::vector<std::vector<size_t>>& clusters) const;
    
    Solution partitionGreedy(double alfa); // Algoritmo guloso para particionamento
    Solution partitionGreedyRandomizedAdaptive(double alfa, int iterations);

    double calculateTotalCost(const std::vector<std::vector<size_t>>& clusters);
    double calculateGap(const std::vector<size_t>& subgraph) const;
    std::vector<size_t> getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices);

    bool verifyAllNodesInSolution(const Solution& solution);
    bool isClusterConnected(const std::vector<size_t>& subgraph);
    bool verifyClustersConnectivity(const Solution& solution);
    void checkSolution(const Solution& solution);
    
private:
    size_t k; // Número de clusters
    std::unordered_map<size_t, std::unique_ptr<Node>> nodes; // Mapa de nós
    std::vector<float> weights;  // Pesos dos vértices
    size_t num_vertices;  // Número de vértices
    size_t num_subgraphs; // Número de subgrafos

};

#endif // GRAPH_HPP
