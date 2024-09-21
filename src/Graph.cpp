#include "../include/Graph.hpp"
#include "../include/Timer.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <cmath>

Graph::Graph(const std::string& filename) {
    readInstance(filename);
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
        std::cout << "Linha lida: \"" << line << "\"" << std::endl;  // Mostra o que foi lido

        // Verifica se a linha contém a substring
        if (line.find("param p :=") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            
            // Lê "param", "p", ":="
            iss >> temp >> temp >> temp;

            // Tenta ler o valor de k
            if (iss >> k) {
                std::cout << "Número de clusters lido: " << k << std::endl;
            } else {
                std::cerr << "Erro ao ler o número de clusters." << std::endl;
                k = 0;  // Define um valor padrão ou trate o erro
            }
            break;  // Sai do loop após encontrar e ler
        }
    }



    // Lê os vértices
    while (std::getline(file, line)) {
        if (line.find("set V :=") != std::string::npos) {
            std::getline(file, line);  // Próxima linha contém os vértices
            std::istringstream iss(line);
            size_t vertex;
            while (iss >> vertex) {
                addNode(vertex, 1.0f);  // Adiciona vértices com peso padrão 1.0f (será atualizado depois)
                std::cout << "Vértice lido: " << vertex << std::endl;
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
                std::cout << "Peso do vértice " << vertex << ": " << weight << std::endl;
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
                    sscanf(edge.c_str(), "(%zu,%zu)", &from, &to);  // Lê as arestas no formato (from,to)
                    addEdge(from, to);  // Adiciona aresta
                    std::cout << "Aresta lida: (" << from << ", " << to << ")" << std::endl;
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
        std::cout << "Nó " << id << " adicionado com peso " << weight << std::endl;
    }
}

void Graph::addEdge(size_t from, size_t to) {
    if (nodes.find(from) != nodes.end() && nodes.find(to) != nodes.end()) {
        nodes[from]->addEdge(to);
        nodes[to]->addEdge(from);  // Se o grafo não for direcionado
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

double Graph::partitionGreedy() {
    Timer timer;
    timer.start();

    // Inicialização
    std::vector<size_t> partition_weights(k, 0);
    std::vector<std::vector<size_t>> clusters(k);
    std::vector<std::pair<size_t, float>> sorted_nodes;

    // Ordena os nós por peso decrescente
    for (const auto& pair : nodes) {
        sorted_nodes.emplace_back(pair.first, pair.second->getWeight());
    }
    std::sort(sorted_nodes.begin(), sorted_nodes.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    // Atribui nós aos clusters
    for (const auto& node : sorted_nodes) {
        size_t min_cluster = std::min_element(partition_weights.begin(), partition_weights.end()) - partition_weights.begin();
        clusters[min_cluster].push_back(node.first);
        partition_weights[min_cluster] += node.second;

        // Mostra a atribuição do nó ao cluster
        std::cout << "Nó " << node.first << " atribuído ao cluster " << min_cluster << " (peso: " << node.second << ")" << std::endl;
    }

    // Calcular o custo 
    double total_cost = calculateTotalCost(clusters);  

    timer.stop();
    std::cout << "Custo total: " << total_cost << std::endl;
    std::cout << "Tempo de execução: " << timer.elapsed() << " segundos." << std::endl;

    return total_cost;
}


double Graph::calculateTotalCost(const std::vector<std::vector<size_t>>& clusters) {
    double total_gap = 0.0;

    for (const auto& cluster : clusters) {
        if (cluster.empty()) continue;  // Ignora clusters vazios

        double max_weight = std::numeric_limits<double>::min();
        double min_weight = std::numeric_limits<double>::max();

        // Calcular pesos máximo e mínimo
        for (size_t node_id : cluster) {
            double weight = nodes[node_id]->getWeight();  // Obtenha o peso do nó
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


bool Graph::isGraphConnected() const {
    if (nodes.empty()) return true;

    std::unordered_set<size_t> visited;
    std::vector<size_t> to_visit = { nodes.begin()->first };

    while (!to_visit.empty()) {
        size_t current = to_visit.back();
        to_visit.pop_back();
        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            for (const auto& edge_pair : nodes.at(current)->getEdges()) {
                if (visited.find(edge_pair.first) == visited.end()) {
                    to_visit.push_back(edge_pair.first);
                }
            }
        }
    }

    return visited.size() == nodes.size();
}
