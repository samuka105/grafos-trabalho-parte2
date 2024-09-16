#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "AuxGraph.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstddef> 



class Graph {
private:
    size_t clusters; // Número de clusters para particionamento
    std::unordered_map<size_t, Node*> nodes; // Mapa de nós (ID -> Node)
    size_t number_edges; // Número total de arestas
    std::unordered_set<D0Pair> d0Pairs; // Armazena pares de nós relacionados
    std::unordered_set<Y0Triple> y0Triples; // Armazena trios de nós com valores adicionais

public:


    //funcoes auxiliares para construtor
    void readClusters(std::ifstream& file); //Leitura do número de clusters (param p) 
    void readVertices(std::ifstream& file); //Leitura dos vertices (set V)
    void readWeights(std::ifstream& file); //Leitura das arestas (param w)
    void readEdges(std::ifstream& file); //Leitura das arestas (set E)
    void readD0(std::ifstream& file) ; //Leitura das arestas (set D0)
    void readY0(std::ifstream& file) ; //Leitura das arestas (set Y0)
    // Construtor e Destrutor
    Graph(const std::string& filename);
    ~Graph();


    void printNodes() const ;
    void printEdges() const;
    
    // Métodos Getters e Setters
    size_t getClusters() const;
    void setClusters(size_t clusters);
    Node* getNode(size_t id);
    void setOutfileName(const std::string& outfile_name);

    // Métodos de Manipulação
    void addNode(size_t id, float weight = 1.0f);
    void addEdge(size_t source_id, size_t target_id, float weight);

    // Métodos de leitura e configuração
    bool readInstance(const std::string& filename);
    void readGraphFile(const std::string& filename);
    void addD0Pair(size_t node1, size_t node2);
    void addY0Triple(size_t node1, size_t node2, size_t node3, float value);

    // Algoritmos para MGGPP
    int greedyAlgorithm(float alfa = 0.0f);
    int greedyRandomizedAdaptive(float alfa, int iterations);
    int greedyRandomizedAdaptiveReactive(const std::vector<float>& alfas, int iterations, int stack);

    // Métodos Auxiliares
    void printGraph() const;
    void saveToFile(const std::string& filename) const;
};

#endif // GRAPH_HPP
