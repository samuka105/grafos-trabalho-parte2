#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Node.hpp"
#include <unordered_map>
#include <memory>
#include <limits>
#include <vector>


struct Candidate {
    size_t vertex;
    float weight;
};


struct Subgraph {
    std::vector<size_t> vertices;  
    float max_weight;               
    float min_weight;               
    float gap;                     

    Subgraph() : max_weight(0.0f), min_weight(std::numeric_limits<float>::max()), gap(0.0f) {}

    size_t size() const {
        return vertices.size();  
    }
};


struct Solution {
    std::vector<Subgraph> subgraphs;  
    double total_gap;

    Solution() : total_gap(0.0) {}  

    Solution(size_t num_subgraphs) : total_gap(0.0) {
        subgraphs.resize(num_subgraphs);  
    }
};



class Graph {
public:
    Graph() = default;
    Graph(const std::string& filename);  
    bool readInstance(const std::string& filename); 
    

    void addNode(size_t id, float weight); 
    void addEdge(size_t from, size_t to);  


    void printNodes() const;  
    void printEdges() const;  
    void printk() const;
    void printClusters(const Solution& solution) const;
    
    Solution partitionGreedy(double alfa); 
    Solution partitionGreedyRandomizedAdaptive(double alfa, int iterations);
    Solution partitionGreedyRandomizedAdaptiveReactive(int iterations);

    double calculateTotalCost(const Solution& solution);
    double calculateGap(const Subgraph& subgraph) const;
    std::vector<size_t>getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices);

    bool verifyAllNodesInSolution(const Solution& solution);
    bool isClusterConnected(const Subgraph& subgraph);
    bool verifyClustersConnectivity(const Solution& solution);
    void checkSolution(const Solution& solution);
    void printGapDetails(const Solution& solution);
    
private:
    size_t k; 
    std::unordered_map<size_t, std::unique_ptr<Node>> nodes; 
    std::vector<float> weights;  
    size_t num_vertices;  
    size_t num_subgraphs; 

};

#endif 
