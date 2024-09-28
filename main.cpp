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
    std::cout << "3) Algoritmo Guloso Randomizado Adaptativo Reativo\n";
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

    // Exibir o menu
    int escolha = exibirMenu();
    
    switch (escolha) {
        case 1: {
            // Algoritmo Guloso
            double alpha = 1.0; // Alpha fixo em 1 para o algoritmo guloso
            Solution solution = grafo.partitionGreedy(alpha);
            grafo.printClusters(solution); // Imprime os clusters
            std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
            break;
        }
        case 2: {
            // Algoritmo Guloso Randomizado Adaptativo
            double alpha;
            int iterations;

            std::cout << "Digite o valor de alpha (0 a 1): ";
            std::cin >> alpha;

            // Validação do valor de alpha
            while (alpha < 0.0 || alpha > 1.0) {
                std::cout << "Valor inválido. Digite o valor de alpha (0 a 1): ";
                std::cin >> alpha;
            }

            std::cout << "Digite o número de iterações: ";
            std::cin >> iterations;

            Solution solution = grafo.partitionGreedyRandomizedAdaptive(alpha, iterations);
            grafo.printClusters(solution); // Imprime os clusters
            std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
            break;
        }
        
        case 3: {
        // Algoritmo Guloso Randomizado Adaptativo Reativo
        int max_iterations;

        std::cout << "Digite o número de iterações: ";
        std::cin >> max_iterations;

        Solution solution = grafo.partitionGreedyRandomizedAdaptiveReactive(max_iterations);
        grafo.printClusters(solution); // Imprime os clusters
        std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
        break;
}
        default:
            std::cout << "Escolha inválida.\n";
            break;
    }

    return 0;
}
