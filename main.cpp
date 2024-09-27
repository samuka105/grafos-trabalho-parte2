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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Erro: Caminho da instância não fornecido.\n";
        std::cerr << "Uso: " << argv[0] << " <caminho_para_instancia>\n";
        return 1;
    }

    // Recebe a instância como parâmetro da execução
    std::string filename = argv[1];

    // Cria uma instância do grafo
    Graph grafo;

    // Tenta ler a instância do arquivo
    if (grafo.readInstance(filename)) {
        std::cout << "Instância '" << filename << "' lida com sucesso!\n";

        // Exibe o número de clusters
        std::cout << "\n--- Cluster ---\n";
        grafo.printk();

        // Exibe os nós lidos
        std::cout << "\n--- Nós ---\n";
        grafo.printNodes();  // Função que imprime os nós e seus pesos

        // Exibe as arestas lidas
        std::cout << "\n--- Arestas ---\n";
        grafo.printEdges();  // Função que imprime as arestas do grafo

        // Exibir o menu para escolher o método
        int metodo = exibirMenu();
        switch (metodo) {
            case 1: {
                std::cout << "\n--- Executando Algoritmo Guloso ---\n";
                Solution sol = grafo.partitionGreedy(1.0);  // Alfa = 1.0 (totalmente guloso)
                grafo.printClusters(sol.subgraphs);
                grafo.checkSolution(sol);
                break;
            }
            case 2: {
                double alfa;
                int iteracoes;
                std::cout << "Digite o valor de alfa (0.0 a 1.0): ";
                std::cin >> alfa;
                std::cout << "Digite o número de iterações: ";
                std::cin >> iteracoes;

                std::cout << "\n--- Executando Algoritmo Guloso Randomizado Adaptativo ---\n";
                Solution sol = grafo.partitionGreedyRandomizedAdaptive(alfa, iteracoes);
                grafo.printClusters(sol.subgraphs);
                grafo.checkSolution(sol);
                break;
            }
            default:
                std::cerr << "Escolha inválida!\n";
        }
    } else {
        std::cerr << "Erro ao ler a instância do arquivo.\n";
        return 1;
    }

    return 0;
}
