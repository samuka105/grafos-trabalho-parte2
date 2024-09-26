#include <iostream>
#include <string>
#include "./include/Graph.hpp"


int main() {
    // Cria uma instância do grafo
    Graph grafo;

    // Define o caminho para o arquivo de instância (ajuste o caminho se necessário)
    std::string filename = "./instances/n100d03p2i2.txt"; 

    // Tenta ler a instância do arquivo
    if (grafo.readInstance(filename)) {
        std::cout << "Instância lida com sucesso!\n";

        // Exibe os nós lidos
        std::cout << "\n--- Nós ---\n";
        grafo.printNodes();  // Função que imprime os nós e seus pesos

        // Exibe as arestas lidas
        std::cout << "\n--- Arestas ---\n";
        grafo.printEdges();  // Função que imprime as arestas do grafo

        // Particiona o grafo usando o algoritmo guloso
        grafo.partitionGreedy();

        // Verifica a solução sem precisar passar parâmetros
        grafo.checkSolution();
    } else {
        std::cerr << "Erro ao ler a instância do arquivo.\n";
    }

    return 0;
}
