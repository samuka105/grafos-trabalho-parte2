#include "../include/Graph.hpp"


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

    
    while (std::getline(file, line)) {
        if (line.find("param p :=") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> temp >> temp;  
            if (!(iss >> k) || k <= 0) {
                std::cerr << "Erro: Número de clusters deve ser maior que zero." << std::endl;
                return false;  
            }
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
                addNode(vertex, 1.0f);  
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
                if (iss >> vertex >> weight) {
                    if (nodes.find(vertex) != nodes.end()) {  
                        nodes[vertex]->setNodeWeight(weight);  
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
                while (iss >> edge) {
                    size_t from, to;
                    if (sscanf(edge.c_str(), "(%zu,%zu)", &from, &to) == 2) {  
                        addEdge(from, to);  
                    } else {
                        std::cerr << "Erro ao ler a aresta: " << edge << std::endl;  
                    }
                }
            }
            break;
        }
    }

    file.close();
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
        nodes[to]->addEdge(from);  
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

void Graph::printClusters(const Solution& solution) const {
    for (size_t i = 0; i < solution.subgraphs.size(); ++i) {
        const Subgraph& subgraph = solution.subgraphs[i];  

        
        std::ostringstream vertices;
        vertices << "Cluster " << (i + 1) << " (Vértices: ";  

        for (size_t j = 0; j < subgraph.vertices.size(); ++j) {
            vertices << subgraph.vertices[j];
            if (j < subgraph.vertices.size() - 1) {
                vertices << " ";  
            }
        }

        vertices << ") - Gap: " << subgraph.gap; 

        std::cout << vertices.str() << std::endl;
    }
}

void Graph::printGapDetails(const Solution& solution) {
    std::cout << "Detalhes dos Gaps:" << std::endl;
    for (size_t i = 0; i < solution.subgraphs.size(); ++i) {
        std::cout << "Cluster " << (i + 1) << " - Gap: " << solution.subgraphs[i].gap << std::endl;
    }
    std::cout << "Gap total da solução: " << solution.total_gap << std::endl;
}



Solution Graph::partitionGreedy(double alfa) {
    Solution solution(num_subgraphs);  
    solution.total_gap = 0.0;

    std::vector<size_t> unassigned_vertices;
    for (const auto& node_pair : nodes) {
        unassigned_vertices.push_back(node_pair.first);  
    }

    
    for (size_t subgraph_idx = 0; subgraph_idx < num_subgraphs; ++subgraph_idx) {
        size_t initial_vertex = unassigned_vertices.back();
        unassigned_vertices.pop_back();  
        
        
        solution.subgraphs[subgraph_idx].vertices.push_back(initial_vertex);
        
        
        float initial_weight = nodes[initial_vertex]->getWeight();
        solution.subgraphs[subgraph_idx].max_weight = initial_weight;
        solution.subgraphs[subgraph_idx].min_weight = initial_weight;
        solution.subgraphs[subgraph_idx].gap = 0.0;

    }

    
    size_t current_subgraph_idx = 0;
    while (!unassigned_vertices.empty()) {
        
        Subgraph& current_subgraph = solution.subgraphs[current_subgraph_idx];

        
        std::vector<size_t> candidate_list = getCandidates(current_subgraph.vertices, unassigned_vertices);

        if (candidate_list.empty()) {
            if (current_subgraph.vertices.size() < 2) {
                std::cerr << "Erro: Subgrafo com menos de 2 vértices.\n";
            }
            current_subgraph_idx = (current_subgraph_idx + 1) % num_subgraphs;  
            continue;
        }

        
        std::sort(candidate_list.begin(), candidate_list.end(), [&](size_t a, size_t b) {
            return nodes[a]->getWeight() < nodes[b]->getWeight();
        });

        
        size_t rcl_size = std::max(static_cast<size_t>(alfa * candidate_list.size()), static_cast<size_t>(1));
        std::vector<size_t> rcl(candidate_list.begin(), candidate_list.begin() + rcl_size);

        
        size_t chosen_vertex = rcl[rand() % rcl.size()];

        
        float previous_gap = current_subgraph.gap;

        
        current_subgraph.vertices.push_back(chosen_vertex);
        unassigned_vertices.erase(std::remove(unassigned_vertices.begin(), unassigned_vertices.end(), chosen_vertex), unassigned_vertices.end());

        
        float vertex_weight = nodes[chosen_vertex]->getWeight();
        current_subgraph.max_weight = std::max(current_subgraph.max_weight, vertex_weight);
        current_subgraph.min_weight = std::min(current_subgraph.min_weight, vertex_weight);
        current_subgraph.gap = current_subgraph.max_weight - current_subgraph.min_weight;

        
        solution.total_gap += (current_subgraph.gap - previous_gap);

        current_subgraph_idx = (current_subgraph_idx + 1) % num_subgraphs;
    }

    return solution;
}

Solution Graph::partitionGreedyRandomizedAdaptive(double alfa, int iterations) {
    Solution best_solution(num_subgraphs);  
    double best_gap = std::numeric_limits<double>::max(); 
    Solution current_solution = partitionGreedy(1);

    for (int i = 0; i < iterations; ++i) {
        
        current_solution = partitionGreedy(alfa);

        if (current_solution.total_gap < best_gap) {
            best_gap = current_solution.total_gap;
            best_solution = current_solution; 
        }
    }

    return best_solution;
}

Solution Graph::partitionGreedyRandomizedAdaptiveReactive(int iterations) {
    std::vector<double> alphas = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};  
    std::vector<double> probabilities(alphas.size(), 1.0 / alphas.size());  
    std::vector<double> scores(alphas.size(), 0.0);  

    Solution current_solution = partitionGreedy(1);
    Solution best_solution(num_subgraphs);  
    double best_gap = std::numeric_limits<double>::max(); 

    for (int i = 0; i < iterations; ++i) {
        double random_value = (double)rand() / RAND_MAX;
        double cumulative_prob = 0.0;
        size_t chosen_alpha_idx = 0;

        for (size_t j = 0; j < probabilities.size(); ++j) {
            cumulative_prob += probabilities[j];
            if (random_value <= cumulative_prob) {
                chosen_alpha_idx = j;
                break;
            }
        }

        double chosen_alpha = alphas[chosen_alpha_idx];
        current_solution = partitionGreedy(chosen_alpha);

       
        if (current_solution.total_gap < best_gap) {
            best_gap = current_solution.total_gap;
            best_solution = current_solution;  
        }

        
        scores[chosen_alpha_idx] += (best_gap - current_solution.total_gap);  

        
        double total_score = std::accumulate(scores.begin(), scores.end(), 0.0);
        if (total_score > 0) {
            for (size_t j = 0; j < probabilities.size(); ++j) {
                probabilities[j] = scores[j] / total_score;  
            }
        } else {
            std::fill(probabilities.begin(), probabilities.end(), 1.0 / probabilities.size());
        }

        //std::cout << "Iteração " << i + 1 << " - Alfa escolhido: " << chosen_alpha << " - Gap da solução: " << current_solution.total_gap << std::endl;
    }

    return best_solution;
}

std::vector<size_t> Graph::getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices) {
    std::vector<size_t> candidates;
    
    for (size_t u : subgraph) {
        for (size_t v : unassigned_vertices) {
            if (nodes[u]->hasEdge(v)) {
                candidates.push_back(v);
                break; 
            }
        }
    }

    return candidates;
}


double Graph::calculateTotalCost(const Solution& solution) {
    double total_gap = 0.0;

    for (const auto& subgraph : solution.subgraphs) {
        total_gap += subgraph.gap;  
    }

    return total_gap;
}




//verificação
bool Graph::verifyAllNodesInSolution(const Solution& solution) {
    std::unordered_set<size_t> all_nodes_in_subgraphs;

    for (const auto& subgraph : solution.subgraphs) {
        for (const auto& vertex : subgraph.vertices) {
            all_nodes_in_subgraphs.insert(vertex);
        }
    }

    return all_nodes_in_subgraphs.size() == nodes.size();
}

bool Graph::isClusterConnected(const Subgraph& subgraph) {
    if (subgraph.vertices.empty()) return true;

    std::unordered_set<size_t> visited;
    std::stack<size_t> to_visit;

    
    to_visit.push(subgraph.vertices[0]);
    visited.insert(subgraph.vertices[0]);

    
    while (!to_visit.empty()) {
        size_t current_node = to_visit.top();
        to_visit.pop();

        for (const auto& neighbor : nodes[current_node]->getEdges()) {
            size_t neighbor_node = neighbor.first;
            if (visited.find(neighbor_node) == visited.end() &&
                std::find(subgraph.vertices.begin(), subgraph.vertices.end(), neighbor_node) != subgraph.vertices.end()) {
                visited.insert(neighbor_node);
                to_visit.push(neighbor_node);
            }
        }
    }

    return visited.size() == subgraph.vertices.size();
}

bool Graph::verifyClustersConnectivity(const Solution& solution) {
    for (const auto& subgraph : solution.subgraphs) {
        if (!isClusterConnected(subgraph)) {
            return false;  
        }
    }
    return true;  
}

void Graph::checkSolution(const Solution& solution) {
    
    if (verifyAllNodesInSolution(solution)) {
        std::cout << "Todos os nós estão na solução." << std::endl;
    } else {
        std::cout << "Alguns nós não foram incluídos na solução." << std::endl;
    }

    if (verifyClustersConnectivity(solution)) {
        std::cout << "Todos os subgrafos são conectados." << std::endl;
    } else {
        std::cout << "Alguns subgrafos não são conectados." << std::endl;
    }
}









