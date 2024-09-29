#include <iostream>
#include <string>
#include <cstdlib>  
#include <chrono>
#include "./include/Graph.hpp"


int exibirMenu() {
    int escolha = 0;
    std::cout << "\nEscolha o método:\n";
    std::cout << "1) Algoritmo Guloso\n";
    std::cout << "2) Algoritmo Guloso Randomizado Adaptativo\n";
    std::cout << "3) Algoritmo Guloso Randomizado Adaptativo Reativo\n";
    std::cout << "0) Sair\n";  
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

    int escolha;
    do {
        escolha = exibirMenu();
        
        switch (escolha) {
            case 1: {
                // Algoritmo Guloso
                auto start = std::chrono::high_resolution_clock::now();
                double alpha = 1.0; // Alpha fixo em 1 para o algoritmo guloso
                Solution solution = grafo.partitionGreedy(alpha);
                grafo.printClusters(solution);
                std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
                auto end = std::chrono::high_resolution_clock::now();  
                std::chrono::duration<double> duration = end - start;  
                std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
                break;
            }
            case 2: {
                // Algoritmo Guloso Randomizado Adaptativo
                auto start = std::chrono::high_resolution_clock::now();
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
                grafo.printClusters(solution);
                std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
                auto end = std::chrono::high_resolution_clock::now();  
                std::chrono::duration<double> duration = end - start;  
                std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
                break;
            }
            case 3: {
                // Algoritmo Guloso Randomizado Adaptativo Reativo
                auto start = std::chrono::high_resolution_clock::now();
                int max_iterations;

                std::cout << "Digite o número de iterações: ";
                std::cin >> max_iterations;

                Solution solution = grafo.partitionGreedyRandomizedAdaptiveReactive(max_iterations);
                grafo.printClusters(solution);
                std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
                auto end = std::chrono::high_resolution_clock::now();  
                std::chrono::duration<double> duration = end - start;  
                std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
                break;
            }
            case 0:
                std::cout << "Saindo do programa." << std::endl;
                break;
            default:
                std::cout << "Escolha inválida. Tente novamente.\n";
                break;
        }
    } while (escolha != 0);  

    return 0;
}
