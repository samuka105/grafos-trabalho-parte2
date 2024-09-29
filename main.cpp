#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <filesystem>
#include <fstream>
#include "./include/Graph.hpp"

// Função para exibir o menu de escolha do método
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

// Função para salvar resultados em arquivo
void salvarResultados(const std::string& metodo, const std::string& instancia, const Solution& solution, double tempo) {
    std::string folder_name = "output" + metodo; // Nome da pasta para o método
    std::string file_name = folder_name + "/" + instancia + ".txt"; // Nome do arquivo

    // Verifica se a pasta existe, se não, cria
    std::filesystem::create_directories(folder_name);

    // Inicializa o contador de execuções
    int execucao = 1;

    // Tenta abrir o arquivo para verificar o número de execuções
    std::ifstream infile(file_name);
    if (infile.good()) {
        std::string line;
        while (std::getline(infile, line)) {
            // Verifica se a linha contém "Execução"
            if (line.find("Execução") != std::string::npos) {
                // Extraí o número da execução
                int numero_execucao;
                std::sscanf(line.c_str(), "Resultados para a instância: %*s - Execução %d", &numero_execucao);
                execucao = numero_execucao + 1; // Incrementa o número de execuções
            }
        }
        infile.close(); // Fecha o arquivo de leitura
    }

    // Abre o arquivo para anexar os resultados
    std::ofstream file(file_name, std::ios::app);
    if (file.is_open()) {
        file << "Resultados para a instância: " << instancia << " - Execução " << execucao << "\n"; // Adiciona o número da execução
        file << "Gap total da solução: " << solution.total_gap << "\n";
        for (size_t i = 0; i < solution.subgraphs.size(); ++i) {
            file << "Cluster " << (i + 1) << " (Vértices: ";
            for (size_t j = 0; j < solution.subgraphs[i].vertices.size(); ++j) {
                file << solution.subgraphs[i].vertices[j];
                if (j < solution.subgraphs[i].vertices.size() - 1) {
                    file << " ";
                }
            }
            file << ") - Gap: " << solution.subgraphs[i].gap << "\n";
        }
        file << "Tempo de execução: " << tempo << " segundos\n";
        file.close();
        std::cout << "Resultados salvos em: " << file_name << std::endl;
    } else {
        std::cerr << "Erro ao criar o arquivo: " << file_name << std::endl;
    }
}

// Função para executar o algoritmo e salvar resultados
void executarMetodo(Graph& grafo, int escolha, const std::string& instancia) {
    Solution solution;

    switch (escolha) {
        case 1: {
            auto start = std::chrono::high_resolution_clock::now();
            double alpha = 1.0; 
            solution = grafo.partitionGreedy(alpha);
            grafo.printClusters(solution);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            salvarResultados("greedy", instancia, solution, duration.count());
            break;
        }
        case 2: {
            auto start = std::chrono::high_resolution_clock::now();
            double alpha;
            int iterations;

            std::cout << "Digite o valor de alpha (0 a 1): ";
            std::cin >> alpha;

            while (alpha < 0.0 || alpha > 1.0) {
                std::cout << "Valor inválido. Digite o valor de alpha (0 a 1): ";
                std::cin >> alpha;
            }

            std::cout << "Digite o número de iterações: ";
            std::cin >> iterations;

            solution = grafo.partitionGreedyRandomizedAdaptive(alpha, iterations);
            grafo.printClusters(solution);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            salvarResultados("grasp", instancia, solution, duration.count());
            break;
        }
        case 3: {
            auto start = std::chrono::high_resolution_clock::now();
            int max_iterations;

            std::cout << "Digite o número de iterações: ";
            std::cin >> max_iterations;

            solution = grafo.partitionGreedyRandomizedAdaptiveReactive(max_iterations);
            grafo.printClusters(solution);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            salvarResultados("grasp_reactive", instancia, solution, duration.count());
            break;
        }
        default:
            std::cout << "Escolha inválida.\n";
            return; // Saia se a escolha for inválida
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Erro: Caminho da instância não fornecido." << std::endl;
        return 1;
    }

    Graph grafo(argv[1]);

    int escolha = exibirMenu();
    if (escolha == 0) return 0; // Sai se o usuário escolher 0

    // Obtém o nome da instância para o arquivo
    std::string instancia = argv[1];

    // Remove o caminho e a extensão do nome da instância
    size_t last_slash = instancia.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        instancia = instancia.substr(last_slash + 1);
    }
    size_t dot = instancia.find_last_of(".");
    if (dot != std::string::npos) {
        instancia = instancia.substr(0, dot);
    }

    executarMetodo(grafo, escolha, instancia); // Executa o método escolhido

    return 0;
}
