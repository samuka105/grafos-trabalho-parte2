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
#include <queue>
#include <stack>
#include <limits>

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
    std::vector<size_t> partition_weights(k, 0);  // Pesos dos clusters
    std::vector<std::vector<size_t>> clusters(k);  // Clusters

    std::vector<std::pair<size_t, float>> sorted_nodes;

    // Ordena os nós por peso decrescente
    for (const auto& pair : nodes) {
        sorted_nodes.emplace_back(pair.first, pair.second->getWeight());
    }
    std::sort(sorted_nodes.begin(), sorted_nodes.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;  // Ordena do maior para o menor peso
              });

    // Define um limite máximo de nós por cluster (balanceamento)
    size_t max_nodes_per_cluster = sorted_nodes.size() / k + 1;  // Distribui aproximadamente igual

    // Atribui nós aos clusters, garantindo conectividade e balanceamento
    for (const auto& node : sorted_nodes) {
        bool node_assigned = false;

        // Tenta adicionar o nó em um cluster existente que tenha conectividade e espaço
        for (size_t i = 0; i < k; ++i) {
            if (clusters[i].size() < max_nodes_per_cluster) {
                size_t connected_node = findConnectedNode(node.first, clusters[i]);

                // Se encontrar um nó conectado no cluster, adiciona o nó
                if (connected_node != std::numeric_limits<size_t>::max()) {
                    clusters[i].push_back(node.first);
                    partition_weights[i] += node.second;
                    node_assigned = true;
                    std::cout << "Nó " << node.first << " atribuído ao cluster " << i 
                              << " (peso: " << node.second << ")" << std::endl;
                    break;  // Sai do loop, pois o nó foi atribuído
                }
            }
        }

        // Se o nó não puder ser adicionado a nenhum cluster existente, atribui ao menor cluster disponível com espaço
        if (!node_assigned) {
            size_t min_cluster = std::min_element(partition_weights.begin(), partition_weights.end()) - partition_weights.begin();
            if (clusters[min_cluster].size() < max_nodes_per_cluster) {
                clusters[min_cluster].push_back(node.first);
                partition_weights[min_cluster] += node.second;
                std::cout << "Nó " << node.first << " atribuído ao cluster " << min_cluster 
                          << " (peso: " << node.second << ") sem conectividade." << std::endl;
            }
        }
    }
    this->clusters = clusters;
    // Calcular o custo total (gap)
    double total_cost = calculateTotalCost(clusters);

    // Imprimir os clusters formatados
    printClusters(clusters);

    timer.stop();
    std::cout << "Custo total: " << total_cost << std::endl;
    std::cout << "Tempo de execução: " << timer.elapsed() << " segundos." << std::endl;

    return total_cost;
}



size_t Graph::findConnectedNode(size_t node_id, const std::vector<size_t>& cluster) {
    // Busca um nó do cluster que seja conectado ao node_id
    for (size_t cluster_node : cluster) {
        if (nodes[node_id]->hasEdge(cluster_node)) {
            return cluster_node;  // Retorna o primeiro nó conectado encontrado
        }
    }
    return std::numeric_limits<size_t>::max();  // Nenhum nó conectado encontrado
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
        double gap = calculateClusterGap(cluster);

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

/**
 * Calcula o gap de um cluster, que é a diferença entre o maior e o menor peso
 * dos nós do cluster.
 *
 * @param cluster Vetor com os IDs dos nós do cluster
 * @return O gap do cluster (diferença entre o maior e o menor peso dos nós)
 */
double Graph::calculateClusterGap(const std::vector<size_t>& cluster) const {
    if (cluster.empty()) return 0.0;

    double max_weight = std::numeric_limits<double>::lowest();
    double min_weight = std::numeric_limits<double>::max();

    // Percorre todos os nós do cluster
    for (size_t node_id : cluster) {
        // Verifica se o node_id existe no mapa
        if (nodes.find(node_id) == nodes.end()) {
            std::cerr << "Erro: O nó " << node_id << " não existe." << std::endl;
            continue;
        }

        double weight = nodes.at(node_id)->getWeight();

        // Atualiza o peso máximo e mínimo
        if (weight > max_weight) {
            max_weight = weight;
        }
        if (weight < min_weight) {
            min_weight = weight;
        }
    }

    return max_weight - min_weight;  // Retorna o gap
}


//verificação
bool Graph::verifyAllNodesInSolution(const std::vector<std::vector<size_t>>& clusters) {
    std::unordered_set<size_t> all_nodes_in_clusters;
    
    // Percorre todos os clusters e adiciona os nós a um conjunto
    for (const auto& cluster : clusters) {
        for (const auto& node : cluster) {
            all_nodes_in_clusters.insert(node);
        }
    }

    // Verifica se o número total de nós nos clusters é igual ao número total de nós no grafo
    return all_nodes_in_clusters.size() == nodes.size();
}

bool Graph::isClusterConnected(const std::vector<size_t>& cluster) {
    if (cluster.empty()) return true;

    std::unordered_set<size_t> visited;
    std::stack<size_t> to_visit;

    // Começa com o primeiro nó do cluster
    to_visit.push(cluster[0]);
    visited.insert(cluster[0]);

    // Faz uma busca em profundidade (DFS) para verificar conectividade
    while (!to_visit.empty()) {
        size_t current_node = to_visit.top();
        to_visit.pop();

        // Explora os vizinhos (arestas) do nó atual
        for (const auto& neighbor : nodes[current_node]->getEdges()) {
            size_t neighbor_node = neighbor.first;  // Acesse o primeiro elemento do par
            // Certifique-se de que o neighbor_node é do tipo size_t
            if (visited.find(neighbor_node) == visited.end() && 
                std::find(cluster.begin(), cluster.end(), neighbor_node) != cluster.end()) {
                visited.insert(neighbor_node);
                to_visit.push(neighbor_node);
            }
        }
    }

    // Verifica se todos os nós do cluster foram visitados (se são conectados)
    return visited.size() == cluster.size();
}




bool Graph::verifyClustersConnectivity(const std::vector<std::vector<size_t>>& clusters) {
    for (const auto& cluster : clusters) {
        if (!isClusterConnected(cluster)) {
            return false;  // Se qualquer cluster não for conectado, retorna falso
        }
    }
    return true;  // Se todos os clusters forem conectados, retorna verdadeiro
}

void Graph::checkSolution() {
    if (verifyAllNodesInSolution(clusters)) {
        std::cout << "Todos os nós estão na solução." << std::endl;
    } else {
        std::cout << "Alguns nós não foram incluídos na solução." << std::endl;
    }

    if (verifyClustersConnectivity(clusters)) {
        std::cout << "Todos os clusters são conectados." << std::endl;
    } else {
        std::cout << "Alguns clusters não são conectados." << std::endl;
    }
}


