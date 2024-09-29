#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <filesystem>
#include <fstream>
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


void salvarResultados(const std::string& metodo, const std::string& instancia, const Solution& solution, double tempo) {
    std::string folder_name = "output" + metodo; 
    std::string file_name = folder_name + "/" + instancia + ".txt"; 

    
    std::filesystem::create_directories(folder_name);

    
    int execucao = 1;

    
    std::ifstream infile(file_name);
    if (infile.good()) {
        std::string line;
        while (std::getline(infile, line)) {
            
            if (line.find("Execução") != std::string::npos) {
                
                int numero_execucao;
                std::sscanf(line.c_str(), "Resultados para a instância: %*s - Execução %d", &numero_execucao);
                execucao = numero_execucao + 1; 
            }
        }
        infile.close(); 
    }

    std::ofstream file(file_name, std::ios::app);
    if (file.is_open()) {
        file << "Resultados para a instância: " << instancia << " - Execução " << execucao << "\n"; 
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


void executarMetodo(Graph& grafo, int escolha, const std::string& instancia) {
    Solution solution;

    switch (escolha) {
        case 1: {
            auto start = std::chrono::high_resolution_clock::now();
            solution = grafo.partitionGreedy(1.0);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            grafo.printClusters(solution);
            std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
            std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
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
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            grafo.printClusters(solution);
            std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
            std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
            salvarResultados("grasp", instancia, solution, duration.count());
            break;
        }
        case 3: {
            auto start = std::chrono::high_resolution_clock::now();
            int max_iterations;

            std::cout << "Digite o número de iterações: ";
            std::cin >> max_iterations;

            solution = grafo.partitionGreedyRandomizedAdaptiveReactive(max_iterations);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            grafo.printClusters(solution);
            std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
            std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
            salvarResultados("grasp_reactive", instancia, solution, duration.count());
            break;
        }
        default:
            std::cout << "Escolha inválida.\n";
            return; 
    }
}


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Erro: Caminho da instância não fornecido." << std::endl;
        return 1;
    }

    Graph grafo(argv[1]);

    int escolha = exibirMenu();
    if (escolha == 0) return 0; 

    
    std::string instancia = argv[1];

    
    size_t last_slash = instancia.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        instancia = instancia.substr(last_slash + 1);
    }
    size_t dot = instancia.find_last_of(".");
    if (dot != std::string::npos) {
        instancia = instancia.substr(0, dot);
    }

    executarMetodo(grafo, escolha, instancia); 

    return 0;
}


/*
void testarInstancias() {
    std::string instancias[] = {
        "n100d03p1i2.txt",
        "n100d03p2i2.txt",
        "n100d03p3i2.txt",
        "n100d06p1i3.txt",
        "n100d06p2i3.txt",
        "n100d06p3i3.txt",
        "n100plap1i1.txt",
        "n100plap2i1.txt",
        "n100plap3i1.txt",
        "n200d03p1i5.txt",
        "n200d03p2i5.txt",
        "n200d03p3i5.txt",
        "n200plap1i4.txt",
        "n200plap2i4.txt",
        "n200plap3i4.txt",
        "n300d06p2i2.txt",
        "n300plap1i1.txt",
        "n300plap2i4.txt",
        "n300plap3i5.txt"
    };

    double alpha;
    std::cout << "Digite o valor de alpha (0 a 1) para o método Guloso Randomizado Adaptativo: ";
    std::cin >> alpha;

    while (alpha < 0.0 || alpha > 1.0) {
        std::cout << "Valor inválido. Digite o valor de alpha (0 a 1): ";
        std::cin >> alpha;
    }

    for (const auto& instancia : instancias) {
        std::string caminho = "./instances/" + instancia; // Ajuste o caminho do arquivo
        std::cout << "Testando instância: " << caminho << std::endl;

        // Tente abrir o arquivo
        std::ifstream infile(caminho);
        if (!infile) {
            std::cerr << "Erro ao abrir o arquivo: " << caminho << std::endl;
            continue; // Passe para a próxima instância se houver erro
        }

        // Cria o grafo para cada instância
        Graph grafo(caminho);

        // Método 1: Guloso
        auto start = std::chrono::high_resolution_clock::now();
        Solution solution1 = grafo.partitionGreedy(1.0);
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration1 = end1 - start;
        salvarResultados("greedy", instancia, solution1, duration1.count());

        // Método 2: Guloso Randomizado Adaptativo
        auto start2 = std::chrono::high_resolution_clock::now();
        Solution solution2 = grafo.partitionGreedyRandomizedAdaptive(alpha, 20);
        auto end2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration2 = end2 - start2;
        salvarResultados("grasp", instancia, solution2, duration2.count());

        // Método 3: Guloso Randomizado Adaptativo Reativo
        auto start3 = std::chrono::high_resolution_clock::now();
        Solution solution3 = grafo.partitionGreedyRandomizedAdaptiveReactive(20);
        auto end3 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration3 = end3 - start3;
        salvarResultados("grasp_reactive", instancia, solution3, duration3.count());
    }
}


int main() {
    testarInstancias();
    return 0;
}
*/