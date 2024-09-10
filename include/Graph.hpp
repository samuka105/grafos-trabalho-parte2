#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "Edge.hpp"
#include <string>

#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

#include <set>
#include <stack>


class Graph
{
public:
    /*Assinatura dos métodos básicos para o funcionamento da classe*/

    // contrutor e destrutor
    
    Graph();
    ~Graph();

    // funcoes de arestas
    void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0);
    


    // funcoes de no
    void add_node(size_t node_id, float weight = 0);
   
    int conected(size_t node_id_1, size_t node_id_2);


private:
    size_t numberNodes;
    size_t numberEdges;
    bool   directed; 
    bool   weightedEdges; 
    bool   weightedNodes;
    Node  *first;
    Node  *last;
    std::vector<Node> nodes; // Define the nodes member variable
};

#endif  //GRAPH_HPP