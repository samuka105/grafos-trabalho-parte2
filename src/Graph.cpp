#include "../include/Graph.hpp"
#include "../include/Timer.hpp"
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
#include <limits>


Graph::Graph(const std::string& filename) {
    readInstance(filename);
    this->num_vertices = nodes.size(); 
    this->num_subgraphs = k; 
}

bool Graph::readInstance(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return false;
    }

    std::string line;
    
    // Lê o número de clusters
    while (std::getline(file, line)) {
        if (line.find("param p :=") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> temp >> temp;
            iss >> k;
            std::cout << "Número de clusters lido: " << k << std::endl;
            break;
        }
    }

    // Lê os vértices
    while (std::getline(file, line)) {
        if (line.find("set V :=") != std::string::npos) {
            std::getline(file, line);
            std::istringstream iss(line);
            size_t vertex;
            while (iss >> vertex) {
                addNode(vertex, 1.0f);  // Adiciona vértices com peso padrão 1.0f
            }
            break;
        }
    }

    // Lê os pesos dos vértices
    while (std::getline(file, line)) {
        if (line.find("param w :=") != std::string::npos) {
            while (std::getline(file, line) && line.find(";") == std::string::npos) {
                std::istringstream iss(line);
                size_t vertex;
                float weight;
                iss >> vertex >> weight;
                nodes[vertex]->setNodeWeight(weight);  // Atualiza o peso dos vértices
                std::cout << "Peso do vértice " << vertex << ": " << weight << std::endl;  // Para depuração
            }
            break;
        }
    }

    // Lê as arestas
    while (std::getline(file, line)) {
        if (line.find("set E :=") != std::string::npos) {
            while (std::getline(file, line) && line.find(";") == std::string::npos) {
                std::istringstream iss(line);
                std::string edge;
                while (iss >> edge) {
                    size_t from, to;
                    if (sscanf(edge.c_str(), "(%zu,%zu)", &from, &to) == 2) {  // Lê as arestas no formato (from,to)
                        addEdge(from, to);  // Adiciona aresta
                        std::cout << "Aresta lida: (" << from << ", " << to << ")" << std::endl;  // Para depuração
                    } else {
                        std::cerr << "Erro ao ler a aresta: " << edge << std::endl;  // Imprime erro
                    }
                }
            }
            break;
        }
    }

    file.close();
    std::cout << "Instância lida com sucesso!" << std::endl;
    return true;
}


void Graph::addNode(size_t id, float weight) {
    if (nodes.find(id) == nodes.end()) {
        nodes[id] = std::make_unique<Node>(id, weight);
    }
}

void Graph::addEdge(size_t from, size_t to) {
    if (nodes.find(from) != nodes.end() && nodes.find(to) != nodes.end()) {
        nodes[from]->addEdge(to);
        nodes[to]->addEdge(from);  // Se o grafo não for direcionado

        // Remova a verificação de adj_list aqui
    } else {
        std::cerr << "Erro: Um ou ambos os nós (" << from << ", " << to << ") não existem." << std::endl;
    }
}

void Graph::printNodes() const {
    if (nodes.empty()) {
        std::cout << "Nenhum nó foi encontrado no grafo." << std::endl;
    } else {
        std::cout << "Nós no grafo: " << std::endl;
        for (const auto& node_pair : nodes) {
            std::cout << "Nó " << node_pair.first << " com peso " << node_pair.second->getWeight() << std::endl;
        }
    }
}

void Graph::printEdges() const {
    if (nodes.empty()) {
        std::cout << "Nenhuma aresta foi encontrada no grafo." << std::endl;
    } else {
        for (const auto& node_pair : nodes) {
            size_t node_id = node_pair.first;
            const Node* node = node_pair.second.get();
            const auto& edges = node->getEdges();

            if (edges.empty()) {
                std::cout << "Nó " << node_id << " não possui arestas." << std::endl;
            } else {
                std::cout << "Arestas do nó " << node_id << ": ";
                for (const auto& edge_pair : edges) {
                    size_t target_id = edge_pair.first;
                    std::cout << "(" << node_id << " -> " << target_id << ") ";
                }
                std::cout << std::endl;
            }
        }
    }
}


void Graph:: printk() const {
    std::cout << "k = " << k << std::endl;
}

Solution Graph::partitionGreedy(double alfa) {
    Solution solution;
    solution.subgraphs.resize(num_subgraphs); // Iniciar a solução com o número de subgrafos
    solution.total_gap = 0.0;

    // Lista de vértices não atribuídos
    std::vector<size_t> unassigned_vertices;
    for (const auto& node_pair : nodes) {
        unassigned_vertices.push_back(node_pair.first); // Insere o ID de cada nó
    }

    // Para cada subgrafo, atribuir os vértices de forma gulosa
    for (size_t subgraph_idx = 0; subgraph_idx < num_subgraphs; ++subgraph_idx) {
        while (!unassigned_vertices.empty()) {
            std::cout << "Subgrafo " << (subgraph_idx + 1) << ": " << unassigned_vertices.size() << " vértices não atribuídos." << std::endl;

            // Gerar a lista de candidatos conectados ao subgrafo
            std::vector<size_t> candidate_list = getCandidates(solution.subgraphs[subgraph_idx], unassigned_vertices);
            std::cout << "Tamanho da lista de candidatos: " << candidate_list.size() << std::endl;

            if (candidate_list.empty()) {
                if (solution.subgraphs[subgraph_idx].size() < 2) {
                    std::cerr << "Erro: Subgrafo com menos de 2 vértices.\n";
                }
                break; // Se não houver candidatos disponíveis
            }

            // Criar a RCL (Restricted Candidate List) com base em alfa
            std::sort(candidate_list.begin(), candidate_list.end(), [&](size_t a, size_t b) {
                return nodes[a]->getWeight() < nodes[b]->getWeight();  // Ordenar os candidatos pelo peso
            });

            // Definindo o tamanho da RCL
            size_t rcl_size = std::max(static_cast<size_t>(alfa * candidate_list.size()), static_cast<size_t>(1));
            std::vector<size_t> rcl(candidate_list.begin(), candidate_list.begin() + rcl_size);

            // Escolher aleatoriamente um vértice da RCL
            size_t chosen_vertex = rcl[rand() % rcl.size()];

            // Adicionar o vértice ao subgrafo
            solution.subgraphs[subgraph_idx].push_back(chosen_vertex);
            unassigned_vertices.erase(std::remove(unassigned_vertices.begin(), unassigned_vertices.end(), chosen_vertex), unassigned_vertices.end());
        }

        // Recalcular o gap total ao final de cada subgrafo
        solution.total_gap += calculateGap(solution.subgraphs[subgraph_idx]);
    }

    return solution;
}




Solution Graph::partitionGreedyRandomizedAdaptive(double alfa, int iterations) {
    Solution best_solution;
    double best_gap = std::numeric_limits<double>::max(); // Melhor gap inicializado como infinito

    for (int i = 0; i < iterations; ++i) {
        // Executa o algoritmo guloso
        Solution current_solution = partitionGreedy(alfa);

        // Verifica se a solução atual é melhor
        if (current_solution.total_gap < best_gap) {
            best_gap = current_solution.total_gap;
            best_solution = current_solution; // Atualiza a melhor solução
        }
    }

    return best_solution;
}



double Graph::calculateTotalCost(const std::vector<std::vector<size_t>>& clusters) {
    double total_gap = 0.0;

    for (const auto& cluster : clusters) {
        if (cluster.empty()) continue;  // Ignora clusters vazios

        double max_weight = std::numeric_limits<double>::min();
        double min_weight = std::numeric_limits<double>::max();

        // Verifica se o cluster tem pelo menos um nó
        if (cluster.size() == 0) {
            std::cerr << "Erro: cluster vazio." << std::endl;
            return 0.0;  // Retorna um valor inválido
        }

        // Calcular pesos máximo e mínimo
        for (size_t node_id : cluster) {
            if (nodes.find(node_id) == nodes.end()) {
                std::cerr << "Erro: nó " << node_id << " não encontrado no grafo." << std::endl;
                return 0.0;  // Retorna um valor inválido
            }

            double weight = nodes[node_id]->getWeight();  // Obtenha o peso do nó
            if (weight < 0.0) {
                std::cerr << "Erro: peso do nó " << node_id << " é negativo." << std::endl;
                return 0.0;  // Retorna um valor inválido
            }

            max_weight = std::max(max_weight, weight);
            min_weight = std::min(min_weight, weight);
        }

        double gap = max_weight - min_weight;
        total_gap += gap;

        // Mostra o cálculo do gap
        
        std::cout << "Cluster: ";
        for (size_t node_id : cluster) {
            std::cout << node_id << " (peso: " << nodes[node_id]->getWeight() << ") ";
        }
        std::cout << " -> Gap: " << gap << " (Max: " << max_weight << ", Min: " << min_weight << ")" << std::endl;
        
    }

    return total_gap;  // Retorna a soma dos gaps
}


void Graph::printClusters(const std::vector<std::vector<size_t>>& clusters) const {
    for (size_t i = 0; i < clusters.size(); ++i) {
        const auto& cluster = clusters[i];

        // Calcule o gap para o cluster atual
        double gap = calculateGap(cluster);

        // Prepare a string para os vértices do cluster
        std::ostringstream vertices;
        vertices << "Cluster " << (i + 1) << " (Vértices: ";  // Inicializa a string com o nome do cluster

        for (size_t j = 0; j < cluster.size(); ++j) {
            vertices << cluster[j];
            if (j < cluster.size() - 1) {
                vertices << " ";  // Adiciona espaço entre os vértices
            }
        }

        // Adiciona a informação do gap
        vertices << ") - Gap: " << gap;  // Adiciona o gap ao final da string

        // Imprime o cluster formatado
        std::cout << vertices.str() << std::endl;
    }
}


double Graph::calculateGap(const std::vector<size_t>& subgraph) const {
    if (subgraph.size() < 2) return 0.0;
    float max_weight = -std::numeric_limits<float>::infinity();
    float min_weight = std::numeric_limits<float>::infinity();

    for (size_t v : subgraph) {
        float weight = nodes.at(v)->getWeight();  // Pegue o peso diretamente de nodes
        max_weight = std::max(max_weight, weight);
        min_weight = std::min(min_weight, weight);
    }

    return max_weight - min_weight;
}


std::vector<size_t> Graph::getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices) {
    std::vector<size_t> candidates;
    for (size_t v : unassigned_vertices) {
        for (size_t u : subgraph) {
            // Verifique se o vértice u tem uma aresta para v
            if (nodes[u]->hasEdge(v)) {
                candidates.push_back(v);
                break; // Não precisa continuar verificando outros vértices do subgrafo
            }
        }
    }
    return candidates;
}





//verificação
bool Graph::verifyAllNodesInSolution(const Solution& solution) {
    std::unordered_set<size_t> all_nodes_in_subgraphs;

    // Percorre todos os subgrafos e adiciona os nós a um conjunto
    for (const auto& subgraph : solution.subgraphs) {
        for (const auto& node : subgraph) {
            all_nodes_in_subgraphs.insert(node);
        }
    }

    // Verifica se o número total de nós nos subgrafos é igual ao número total de nós no grafo
    return all_nodes_in_subgraphs.size() == nodes.size();
}


bool Graph::isClusterConnected(const std::vector<size_t>& subgraph) {
    if (subgraph.empty()) return true;

    std::unordered_set<size_t> visited;
    std::stack<size_t> to_visit;

    // Começa com o primeiro nó do subgrafo
    to_visit.push(subgraph[0]);
    visited.insert(subgraph[0]);

    // Faz uma busca em profundidade (DFS) para verificar conectividade
    while (!to_visit.empty()) {
        size_t current_node = to_visit.top();
        to_visit.pop();

        // Explora os vizinhos (arestas) do nó atual
        for (const auto& neighbor : nodes[current_node]->getEdges()) {
            size_t neighbor_node = neighbor.first;  // Acesse o primeiro elemento do par
            // Certifique-se de que o neighbor_node é do tipo size_t
            if (visited.find(neighbor_node) == visited.end() &&
                std::find(subgraph.begin(), subgraph.end(), neighbor_node) != subgraph.end()) {
                visited.insert(neighbor_node);
                to_visit.push(neighbor_node);
            }
        }
    }

    // Verifica se todos os nós do subgrafo foram visitados (se são conectados)
    return visited.size() == subgraph.size();
}


bool Graph::verifyClustersConnectivity(const Solution& solution) {
    for (const auto& subgraph : solution.subgraphs) {
        if (!isClusterConnected(subgraph)) {
            return false;  // Se qualquer subgrafo não for conectado, retorna falso
        }
    }
    return true;  // Se todos os subgrafos forem conectados, retorna verdadeiro
}

void Graph::checkSolution(const Solution& solution) {
    // Verifica se todos os nós estão na solução
    if (verifyAllNodesInSolution(solution)) {
        std::cout << "Todos os nós estão na solução." << std::endl;
    } else {
        std::cout << "Alguns nós não foram incluídos na solução." << std::endl;
    }

    // Verifica a conectividade dos subgrafos
    if (verifyClustersConnectivity(solution)) {
        std::cout << "Todos os subgrafos são conectados." << std::endl;
    } else {
        std::cout << "Alguns subgrafos não são conectados." << std::endl;
    }
}






