#include <iostream>
#include <string>
#include <cstdlib>  
#include "./include/Graph.hpp"

// Função para exibir o menu de escolha do método
int exibirMenu() {
    int escolha = 0;
    std::cout << "\nEscolha o método:\n";
    std::cout << "1) Algoritmo Guloso\n";
    std::cout << "2) Algoritmo Guloso Randomizado Adaptativo\n";
    std::cout << "Escolha: ";
    std::cin >> escolha;
    return escolha;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Erro: Caminho da instância não fornecido." << std::endl;
        return 1;
    }

    Graph grafo(argv[1]);
    
    // Execute o algoritmo guloso
    double alpha = 1.0; // Ou um valor que você queira testar
    Solution solution = grafo.partitionGreedy(alpha);

    // Verifique a solução
    std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
    grafo.printClusters(solution); // Imprime os clusters
    grafo.checkSolution(solution); // Verifica se a solução é consistente

    return 0;
}

