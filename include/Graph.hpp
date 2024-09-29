#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Node.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <memory>
#include <limits>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>

// Estrutura que representa um candidato, contendo o vértice e o peso
struct Candidate {
    size_t vertex;  // ID do vértice
    float weight;   // Peso associado ao vértice
};

// Estrutura que representa um subgrafo com vértices e atributos como pesos máximos, mínimos e gap
struct Subgraph {
    std::vector<size_t> vertices;  // Vértices que pertencem ao subgrafo
    float max_weight;              // Peso máximo no subgrafo
    float min_weight;              // Peso mínimo no subgrafo
    float gap;                     // Diferença entre o peso máximo e mínimo (gap)

    // Construtor padrão que inicializa os valores de max_weight e min_weight
    Subgraph() : max_weight(0.0f), min_weight(std::numeric_limits<float>::max()), gap(0.0f) {}

    // Retorna o número de vértices no subgrafo
    size_t size() const {
        return vertices.size();  
    }
};

// Estrutura que representa a solução do particionamento, contendo os subgrafos e o gap total
struct Solution {
    std::vector<Subgraph> subgraphs;  // Lista de subgrafos que formam a solução
    double total_gap;                 // Gap total da solução

    // Construtor padrão que inicializa o gap total
    Solution() : total_gap(0.0) {}

    // Construtor que inicializa a solução com um número específico de subgrafos
    Solution(size_t num_subgraphs) : total_gap(0.0) {
        subgraphs.resize(num_subgraphs);  // Cria os subgrafos
    }
};

// Classe que representa um grafo
class Graph {
public:
    // --------- Construtor e Funções de Configuração do Grafo ---------

    // Construtor que lê o arquivo da instância e inicializa o grafo
    Graph(const std::string& filename);  

    // Lê a instância de um arquivo e preenche os vértices e arestas do grafo
    bool readInstance(const std::string& filename); 

    // Adiciona um vértice ao grafo, especificando o ID e o peso do vértice
    void addNode(size_t id, float weight); 

    // Adiciona uma aresta não-direcionada entre dois vértices
    void addEdge(size_t from, size_t to);  

    // --------- Funções de Impressão ---------

    // Imprime todos os nós e seus pesos
    void printNodes() const;  

    // Imprime todas as arestas do grafo
    void printEdges() const;  

    // Imprime o valor de k (número de subgrafos)
    void printk() const;

    // Imprime os clusters (subgrafos) e os vértices que pertencem a cada cluster
    void printClusters(const Solution& solution) const;

    // Imprime os detalhes de forma simplificada do gap de cada cluster e o gap total da solução
    void printGapDetails(const Solution& solution);

    // --------- Algoritmos de Particionamento ---------

    // Algoritmo guloso que realiza o particionamento de vértices em subgrafos, utilizando um alfa fixo
    Solution partitionGreedy(double alfa); 

    // Algoritmo guloso randomizado adaptativo que realiza várias iterações e escolhe a melhor solução
    Solution partitionGreedyRandomizedAdaptive(double alfa, int iterations);

    // Algoritmo guloso randomizado adaptativo reativo que ajusta as probabilidades de escolha de alfa
    Solution partitionGreedyRandomizedAdaptiveReactive(int iterations);

    // Gera a lista de candidatos (vértices conectados) para um subgrafo atual
    std::vector<size_t> getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices);

    // --------- Funções de Verificação ---------

    // Verifica se todos os nós do grafo estão presentes na solução
    bool verifyAllNodesInSolution(const Solution& solution);

    // Verifica se os vértices de um subgrafo estão conectados
    bool isClusterConnected(const Subgraph& subgraph);

    // Verifica se todos os subgrafos da solução são conectados
    bool verifyClustersConnectivity(const Solution& solution);

    // Verifica a solução inteira, garantindo que todos os vértices estejam incluídos e os subgrafos estejam conectados
    void checkSolution(const Solution& solution);

private:
    size_t k;  // Número de clusters/subgrafos
    std::unordered_map<size_t, std::unique_ptr<Node>> nodes;  // Mapeia os vértices para os objetos Node
    std::vector<float> weights;  // Vetor para armazenar os pesos dos vértices
    size_t num_vertices;  // Número total de vértices no grafo
    size_t num_subgraphs;  // Número de subgrafos (clusters) em que o grafo será particionado
};

#endif
