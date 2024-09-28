#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Node.hpp"
#include <unordered_map>
#include <memory>
#include <limits>
#include <vector>


// Representa um vértice candidato com seu peso
struct Candidate {
    size_t vertex;
    float weight;
};

// Representa um subgrafo com seus vértices, o gap e outros detalhes importantes
struct Subgraph {
    std::vector<size_t> vertices;  // IDs dos vértices
    float max_weight;               // Peso máximo
    float min_weight;               // Peso mínimo
    float gap;                     // Gap

    // Construtor padrão
    Subgraph() : max_weight(0.0f), min_weight(std::numeric_limits<float>::max()), gap(0.0f) {}

    // Método para obter o tamanho do subgrafo
    size_t size() const {
        return vertices.size();  // Retorna o número de vértices no subgrafo
    }
};



// Representa a solução completa do particionamento em subgrafos
struct Solution {
    std::vector<Subgraph> subgraphs;  // Supondo que 'Subgraph' é uma estrutura que você definiu
    double total_gap;

    // Construtor padrão
    Solution() : total_gap(0.0) {}  // Inicializa o total_gap como 0.0

    // Construtor com número de subgrafos
    Solution(size_t num_subgraphs) : total_gap(0.0) {
        subgraphs.resize(num_subgraphs);  // Redimensiona o vetor de subgrafos
    }
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
    void printk() const;
    void printClusters(const Solution& solution) const;
    
    Solution partitionGreedy(double alfa); // Algoritmo guloso para particionamento
    Solution partitionGreedyRandomizedAdaptive(double alfa, int iterations);

    double calculateTotalCost(const Solution& solution);
    double calculateGap(const Subgraph& subgraph) const;
    std::vector<size_t>getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices);

    bool verifyAllNodesInSolution(const Solution& solution);
    bool isClusterConnected(const Subgraph& subgraph);
    bool verifyClustersConnectivity(const Solution& solution);
    void checkSolution(const Solution& solution);
    void printGapDetails(const Solution& solution);
    
private:
    size_t k; // Número de clusters
    std::unordered_map<size_t, std::unique_ptr<Node>> nodes; // Mapa de nós
    std::vector<float> weights;  // Pesos dos vértices
    size_t num_vertices;  // Número de vértices
    size_t num_subgraphs; // Número de subgrafos

};

#endif // GRAPH_HPP
