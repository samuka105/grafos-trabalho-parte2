#include "./include/Graph.hpp"
#include <iostream>

int main() {
    
    Graph graph("./instances/ipt1.txt");
    std::cout << "Número de clusters: " << graph.getClusters() << std::endl;
    
    graph.printNodes(); // imprime os nós no grafo
    //graph.printEdges();  // Verifica as arestas
    
    return 0;
}
