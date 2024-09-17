#include "./include/Graph.hpp"
#include <iostream>
#include <iostream>
#include "Graph.hpp"  // Inclua o arquivo de cabeçalho da sua classe Graph

#include <iostream>
#include "graph.hpp"  // Supondo que a classe Graph esteja definida aqui
#include <iostream>
#include "Graph.hpp"
#include <iostream>
#include <stdexcept> // Para std::runtime_error
#include "Graph.hpp"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <caminho do arquivo>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    Graph graph(filename);

    try {
        if (graph.readInstance(filename)) {
            std::cout << "Instância lida com sucesso." << std::endl;
            std::cout << "Nós do grafo:" << std::endl;
            graph.printNodes();

            std::cout << "Arestas do grafo:" << std::endl;
             graph.printEdges();

            std::cout << "Número de clusters: " << graph.getClusters() << std::endl;
        } else {
            std::cerr << "Erro ao ler a instância." << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro ao construir o grafo: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
/*
int main() {
    // Caminho para o arquivo de entrada
    std::string filename = "ipt1.txt";
   //Graph graph("./instances/ipt1.txt");

    // Cria uma instância do grafo
    Graph graph(filename);

    // Tenta carregar a instância do arquivo
    if (!graph.readInstance(filename)) {
        std::cerr << "Erro ao carregar o grafo do arquivo " << filename << std::endl;
        return 1;
    }

    std::cout << "Grafo carregado com sucesso!" << std::endl;

    // Teste: imprime nós e arestas para verificar se foram carregados corretamente
    std::cout << "Nós do grafo:" << std::endl;
    graph.printNodes();

    std::cout << "Arestas do grafo:" << std::endl;
    graph.printEdges();

    // Opcional: exiba outras informações ou atributos do grafo, se necessário
     std::cout << "Número de clusters: " << graph.getClusters() << std::endl;

    return 0;
}


int main() {
    
    Graph graph("./instances/ipt1.txt");
    std::cout << "Número de clusters: " << graph.getClusters() << std::endl;
    
    graph.printNodes(); // imprime os nós no grafo
    //graph.printEdges();  // Verifica as arestas
    
    return 0;
}*/
