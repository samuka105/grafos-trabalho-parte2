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
    std::cout << "3) Algoritmo Guloso Randomizado Adaptativo Reativo (em desenvolvimento)\n";
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
            std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
            grafo.printClusters(solution); // Imprime os clusters
            grafo.checkSolution(solution); // Verifica se a solução é consistente
            grafo.printGapDetails(solution); // Imprime os detalhes dos gaps
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
            std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
            grafo.printClusters(solution); // Imprime os clusters
            grafo.checkSolution(solution); // Verifica se a solução é consistente
            grafo.printGapDetails(solution); // Imprime os detalhes dos gaps
            break;
        }
        
        case 3: {
        // Algoritmo Guloso Randomizado Adaptativo Reativo
        double initial_alfa;
        int max_iterations;
        double max_gap;
        double min_gap;

        std::cout << "Digite o valor inicial de alfa (0 a 1): ";
        std::cin >> initial_alfa;

        // Validação do valor de alfa
        while (initial_alfa < 0.0 || initial_alfa > 1.0) {
            std::cout << "Valor inválido. Digite o valor inicial de alfa (0 a 1): ";
            std::cin >> initial_alfa;
        }

        std::cout << "Digite o número máximo de iterações: ";
        std::cin >> max_iterations;

        std::cout << "Digite o gap máximo permitido: ";
        std::cin >> max_gap;

        std::cout << "Digite o gap mínimo permitido: ";
        std::cin >> min_gap;

        Solution solution = grafo.partitionGreedyRandomizedAdaptiveReactive(initial_alfa, max_iterations, max_gap, min_gap);
        std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
        grafo.printClusters(solution); // Imprime os clusters
        grafo.checkSolution(solution); // Verifica se a solução é consistente
        grafo.printGapDetails(solution); // Imprime os detalhes dos gaps
        break;
}
        default:
            std::cout << "Escolha inválida.\n";
            break;
    }

    return 0;
}
