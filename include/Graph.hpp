#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Node.hpp"
#include <unordered_map>
#include <memory>
#include <vector>

class Graph {
public:
    Graph() = default;
    Graph(const std::string& filename);  // Construtor que lê o arquivo de entrada
    bool readInstance(const std::string& filename); // Função para ler instância
    

    void addNode(size_t id, float weight); // Adiciona um nó ao grafo
    void addEdge(size_t from, size_t to);  // Adiciona uma aresta ao grafo
    void printNodes() const;  // Imprime os nós
    void printEdges() const;  // Imprime as arestas
    void printClusters(const std::vector<std::vector<size_t>>& clusters) const;

    bool isGraphConnected() const; // Verifica se o grafo é conectado
    
    size_t getClusters() const; // Retorna o número de clusters

    double partitionGreedy(); // Algoritmo guloso para particionamento
    double calculateTotalCost(const std::vector<std::vector<size_t>>& clusters);
    double calculateClusterGap(const std::vector<size_t>& cluster) const;
    size_t findEmptyCluster(const std::vector<std::vector<size_t>>& clusters);
    size_t findConnectedNode(size_t node_id, const std::vector<size_t>& cluster);

    bool verifyAllNodesInSolution(const std::vector<std::vector<size_t>>& clusters);
    bool isClusterConnected(const std::vector<size_t>& cluster);
    bool verifyClustersConnectivity(const std::vector<std::vector<size_t>>& clusters);
    void checkSolution();

private:
    size_t k; // Número de clusters
    std::unordered_map<size_t, std::unique_ptr<Node>> nodes; // Mapa de nós
    std::vector<std::vector<size_t>> clusters;  // Para armazenar os clusters

};

#endif // GRAPH_HPP
