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
            iss >> temp >> temp >> temp;  // Ignora "param p :="
            if (!(iss >> k) || k <= 0) {
                std::cerr << "Erro: Número de clusters deve ser maior que zero." << std::endl;
                return false;  // Retorna falso se k for inválido
            }
            std::cout << "Número de clusters lido: " << k << std::endl;
            break;
        }
    }

    // Lê os vértices
    while (std::getline(file, line)) {
        if (line.find("set V :=") != std::string::npos) {
            std::getline(file, line);  // Próxima linha contém os vértices
            std::istringstream iss(line);
            size_t vertex;
            std::cout << "Vértices lidos: ";
            while (iss >> vertex) {
                addNode(vertex, 1.0f);  // Adiciona vértices com peso padrão 1.0f
                std::cout << vertex << " ";  // Para depuração
            }
            std::cout << std::endl;
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
                if (iss >> vertex >> weight) {
                    if (nodes.find(vertex) != nodes.end()) {  // Verifica se o nó foi adicionado
                        nodes[vertex]->setNodeWeight(weight);  // Atualiza o peso dos vértices
                        std::cout << "(" << vertex << ": " << weight << " )";  // Para depuração
                    } else {
                        std::cerr << "Erro: Vértice " << vertex << " não encontrado." << std::endl;
                    }
                }
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
                std::cout << "Aresta lida: ";
                while (iss >> edge) {
                    size_t from, to;
                    if (sscanf(edge.c_str(), "(%zu,%zu)", &from, &to) == 2) {  // Lê as arestas no formato (from,to)
                        addEdge(from, to);  // Adiciona aresta
                        std::cout << " (" << from << ", " << to << ") ";  // Para depuração
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
    Solution solution(num_subgraphs);  // Inicializa a solução com o número de subgrafos
    solution.total_gap = 0.0;

    // Inicializa a lista de vértices não atribuídos
    std::vector<size_t> unassigned_vertices;
    for (const auto& node_pair : nodes) {
        unassigned_vertices.push_back(node_pair.first);  // Insere o ID de cada nó
    }

    // Atribui um vértice inicial a cada subgrafo para garantir que todos os subgrafos tenham pelo menos um vértice
    for (size_t subgraph_idx = 0; subgraph_idx < num_subgraphs; ++subgraph_idx) {
        size_t initial_vertex = unassigned_vertices.back();
        unassigned_vertices.pop_back();  // Remove o vértice dos não atribuídos
        
        // Adiciona o vértice ao subgrafo
        solution.subgraphs[subgraph_idx].vertices.push_back(initial_vertex);
        
        // Atualiza o peso do subgrafo
        float initial_weight = nodes[initial_vertex]->getWeight();
        solution.subgraphs[subgraph_idx].max_weight = initial_weight;
        solution.subgraphs[subgraph_idx].min_weight = initial_weight;
        solution.subgraphs[subgraph_idx].gap = 0.0;

        std::cout << "Subgrafo " << subgraph_idx + 1 << " iniciado com vértice " << initial_vertex << std::endl;
    }

    // Alterna entre os subgrafos para distribuir os vértices
    size_t current_subgraph_idx = 0;
    while (!unassigned_vertices.empty()) {
        // Seleciona o subgrafo atual
        Subgraph& current_subgraph = solution.subgraphs[current_subgraph_idx];

        // Gerar a lista de candidatos conectados ao subgrafo atual
        std::vector<size_t> candidate_list = getCandidates(current_subgraph.vertices, unassigned_vertices);
        std::cout << "Subgrafo " << (current_subgraph_idx + 1) << ": " << unassigned_vertices.size() << " vértices não atribuídos." << std::endl;
        std::cout << "Tamanho da lista de candidatos: " << candidate_list.size() << std::endl;

        if (candidate_list.empty()) {
            if (current_subgraph.vertices.size() < 2) {
                std::cerr << "Erro: Subgrafo com menos de 2 vértices.\n";
            }
            current_subgraph_idx = (current_subgraph_idx + 1) % num_subgraphs;  // Alterna para o próximo subgrafo
            continue;
        }

        // Ordenar os candidatos pelo peso do vértice (vértices com pesos menores são preferidos)
        std::sort(candidate_list.begin(), candidate_list.end(), [&](size_t a, size_t b) {
            return nodes[a]->getWeight() < nodes[b]->getWeight();
        });

        // Criar a RCL (Restricted Candidate List) com base em alfa
        size_t rcl_size = std::max(static_cast<size_t>(alfa * candidate_list.size()), static_cast<size_t>(1));
        std::vector<size_t> rcl(candidate_list.begin(), candidate_list.begin() + rcl_size);

        // Escolher aleatoriamente um vértice da RCL
        size_t chosen_vertex = rcl[rand() % rcl.size()];

        // Adicionar o vértice ao subgrafo
        current_subgraph.vertices.push_back(chosen_vertex);
        unassigned_vertices.erase(std::remove(unassigned_vertices.begin(), unassigned_vertices.end(), chosen_vertex), unassigned_vertices.end());

        // Atualizar o gap do subgrafo
        float vertex_weight = nodes[chosen_vertex]->getWeight();
        current_subgraph.max_weight = std::max(current_subgraph.max_weight, vertex_weight);
        current_subgraph.min_weight = std::min(current_subgraph.min_weight, vertex_weight);
        current_subgraph.gap = current_subgraph.max_weight - current_subgraph.min_weight;

        // Atualizar o gap total da solução
        solution.total_gap += current_subgraph.gap;

        // Alterna para o próximo subgrafo para balancear a distribuição
        current_subgraph_idx = (current_subgraph_idx + 1) % num_subgraphs;
    }

    return solution;
}



Solution Graph::partitionGreedyRandomizedAdaptive(double alfa, int iterations) {
    Solution best_solution(num_subgraphs);  // Inicializa com o número correto de subgrafos
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


double Graph::calculateTotalCost(const Solution& solution) {
    double total_gap = 0.0;

    for (const auto& subgraph : solution.subgraphs) {
        total_gap += subgraph.gap;  // O gap de cada subgrafo já foi calculado
    }

    return total_gap;
}



void Graph::printClusters(const Solution& solution) const {
    for (size_t i = 0; i < solution.subgraphs.size(); ++i) {
        const Subgraph& subgraph = solution.subgraphs[i];  // Acessa o subgrafo atual

        // Prepare a string para os vértices do subgrafo
        std::ostringstream vertices;
        vertices << "Cluster " << (i + 1) << " (Vértices: ";  // Inicializa a string com o nome do cluster

        for (size_t j = 0; j < subgraph.vertices.size(); ++j) {
            vertices << subgraph.vertices[j];
            if (j < subgraph.vertices.size() - 1) {
                vertices << " ";  // Adiciona espaço entre os vértices
            }
        }

        // Adiciona a informação do gap
        vertices << ") - Gap: " << subgraph.gap;  // Adiciona o gap ao final da string

        // Imprime o subgrafo formatado
        std::cout << vertices.str() << std::endl;
    }
}

std::vector<size_t> Graph::getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices) {
    std::vector<size_t> candidates;
    
    for (size_t u : subgraph) {
        for (size_t v : unassigned_vertices) {
            // Verifique se u tem uma aresta para v
            if (nodes[u]->hasEdge(v)) {
                candidates.push_back(v);
                break; // Se já encontrou um candidato para u, não precisa verificar mais
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
        for (const auto& vertex : subgraph.vertices) {
            all_nodes_in_subgraphs.insert(vertex);
        }
    }

    // Verifica se o número total de nós nos subgrafos é igual ao número total de nós no grafo
    return all_nodes_in_subgraphs.size() == nodes.size();
}



bool Graph::isClusterConnected(const Subgraph& subgraph) {
    if (subgraph.vertices.empty()) return true;

    std::unordered_set<size_t> visited;
    std::stack<size_t> to_visit;

    // Começa com o primeiro nó do subgrafo
    to_visit.push(subgraph.vertices[0]);
    visited.insert(subgraph.vertices[0]);

    // Faz uma busca em profundidade (DFS) para verificar conectividade
    while (!to_visit.empty()) {
        size_t current_node = to_visit.top();
        to_visit.pop();

        // Explora os vizinhos (arestas) do nó atual
        for (const auto& neighbor : nodes[current_node]->getEdges()) {
            size_t neighbor_node = neighbor.first;
            if (visited.find(neighbor_node) == visited.end() &&
                std::find(subgraph.vertices.begin(), subgraph.vertices.end(), neighbor_node) != subgraph.vertices.end()) {
                visited.insert(neighbor_node);
                to_visit.push(neighbor_node);
            }
        }
    }

    // Verifica se todos os nós do subgrafo foram visitados
    return visited.size() == subgraph.vertices.size();
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






