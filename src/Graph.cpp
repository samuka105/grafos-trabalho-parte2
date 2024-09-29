#include "../include/Graph.hpp"


// --------- Construtor e Funções de Configuração do Grafo ---------


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


// --------- Funções de Impressão ---------


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


// --------- Algoritmos de Particionamento ---------


/**
 * @brief Algoritmo guloso para particionamento de vértices em subgrafos conectados.
 * 
 * O algoritmo distribui os vértices do grafo em `k` subgrafos de forma gulosa. 
 * Cada subgrafo inicialmente recebe um vértice, e os demais vértices são distribuídos
 * conforme critérios de peso e conectividade. O objetivo é minimizar o gap entre o 
 * maior e menor peso em cada subgrafo.
 * 
 * @param alfa Parâmetro de controle da aleatoriedade. Com alfa = 1, o algoritmo é puramente guloso. 
 *             Com alfa = 0, a escolha dos vértices é completamente aleatória.
 * @return Solution A solução gerada contendo os subgrafos e o gap total.
 */


Solution Graph::partitionGreedy(double alfa) {
    // Inicializa a solução com o número correto de subgrafos
    Solution solution(num_subgraphs);  
    solution.total_gap = 0.0;

    // Lista de vértices não atribuídos
    std::vector<size_t> unassigned_vertices;
    for (const auto& node_pair : nodes) {
        unassigned_vertices.push_back(node_pair.first);  // Insere o ID de cada vértice
    }

    // Atribui um vértice inicial a cada subgrafo
    for (size_t subgraph_idx = 0; subgraph_idx < num_subgraphs; ++subgraph_idx) {
        size_t initial_vertex = unassigned_vertices.back();
        unassigned_vertices.pop_back();  // Remove o vértice dos não atribuídos
        
        // Adiciona o vértice ao subgrafo
        solution.subgraphs[subgraph_idx].vertices.push_back(initial_vertex);
        
        // Inicializa os pesos máximo e mínimo com o peso do vértice inicial
        float initial_weight = nodes[initial_vertex]->getWeight();
        solution.subgraphs[subgraph_idx].max_weight = initial_weight;
        solution.subgraphs[subgraph_idx].min_weight = initial_weight;
        solution.subgraphs[subgraph_idx].gap = 0.0;
    }

    // Alterna entre os subgrafos para distribuir os vértices restantes
    size_t current_subgraph_idx = 0;
    while (!unassigned_vertices.empty()) {
        // Subgrafo atual
        Subgraph& current_subgraph = solution.subgraphs[current_subgraph_idx];

        // Lista de candidatos conectados ao subgrafo atual
        std::vector<size_t> candidate_list = getCandidates(current_subgraph.vertices, unassigned_vertices);

        // Se não houver candidatos, pule para o próximo subgrafo
        if (candidate_list.empty()) {
            if (current_subgraph.vertices.size() < 2) {
                std::cerr << "Erro: Subgrafo com menos de 2 vértices.\n";
            }
            current_subgraph_idx = (current_subgraph_idx + 1) % num_subgraphs;  
            continue;
        }

        // Ordena os candidatos pelo peso do vértice (vértices com pesos menores são preferidos)
        std::sort(candidate_list.begin(), candidate_list.end(), [&](size_t a, size_t b) {
            return nodes[a]->getWeight() < nodes[b]->getWeight();
        });

        // Cria a RCL (Restricted Candidate List) com base em alfa
        size_t rcl_size = std::max(static_cast<size_t>(alfa * candidate_list.size()), static_cast<size_t>(1));
        std::vector<size_t> rcl(candidate_list.begin(), candidate_list.begin() + rcl_size);

        // Escolhe aleatoriamente um vértice da RCL
        size_t chosen_vertex = rcl[rand() % rcl.size()];

        // Armazena o gap anterior para ajustar o gap total
        float previous_gap = current_subgraph.gap;

        // Adiciona o vértice ao subgrafo
        current_subgraph.vertices.push_back(chosen_vertex);
        unassigned_vertices.erase(std::remove(unassigned_vertices.begin(), unassigned_vertices.end(), chosen_vertex), unassigned_vertices.end());

        // Verifica se o subgrafo ainda é conexo
        if (!isClusterConnected(current_subgraph)) {
            std::cerr << "Erro: O subgrafo deixou de ser conexo após adicionar o vértice " << chosen_vertex << std::endl;
        }

        // Atualiza o peso máximo, mínimo e o gap do subgrafo
        float vertex_weight = nodes[chosen_vertex]->getWeight();
        current_subgraph.max_weight = std::max(current_subgraph.max_weight, vertex_weight);
        current_subgraph.min_weight = std::min(current_subgraph.min_weight, vertex_weight);
        current_subgraph.gap = current_subgraph.max_weight - current_subgraph.min_weight;

        // Ajusta o gap total da solução com base na mudança do gap do subgrafo
        solution.total_gap += (current_subgraph.gap - previous_gap);

        // Alterna para o próximo subgrafo
        current_subgraph_idx = (current_subgraph_idx + 1) % num_subgraphs;
    }

    return solution;
}


/**
 * @brief Algoritmo guloso randomizado adaptativo para particionamento de vértices.
 * 
 * Este algoritmo utiliza uma abordagem gulosa com adição de randomização controlada
 * pelo parâmetro alfa. A cada iteração, uma nova solução é gerada utilizando o valor
 * de alfa fornecido, que controla o nível de aleatoriedade nas escolhas dos vértices. 
 * O objetivo é encontrar a melhor solução após várias iterações.
 * 
 * @param alfa Valor de controle da aleatoriedade. Alfa = 1 corresponde ao algoritmo guloso puro,
 *             enquanto alfa = 0 representa escolhas completamente aleatórias.
 * @param iterations Número de iterações a serem realizadas.
 * @return Solution A melhor solução encontrada após as iterações.
 */
Solution Graph::partitionGreedyRandomizedAdaptive(double alfa, int iterations) {
    Solution best_solution(num_subgraphs);  // Inicializa a melhor solução
    double best_gap = std::numeric_limits<double>::max();  // Inicializa o melhor gap com valor máximo
    Solution current_solution = partitionGreedy(1);  // Executa o algoritmo guloso puro inicialmente

    // Loop para executar o algoritmo em várias iterações
    for (int i = 0; i < iterations; ++i) {
        
        // Gera uma nova solução com o valor de alfa fornecido
        current_solution = partitionGreedy(alfa);

        // Verifica se a nova solução é melhor que a melhor solução encontrada até o momento
        if (current_solution.total_gap < best_gap) {
            best_gap = current_solution.total_gap;
            best_solution = current_solution;  // Atualiza a melhor solução
        }
    }

    return best_solution; 
}


/**
 * @brief Algoritmo guloso randomizado adaptativo reativo para particionamento de vértices.
 * 
 * O algoritmo utiliza uma estratégia reativa, adaptando as probabilidades de escolha de diferentes valores de alfa
 * com base na performance das soluções encontradas em iterações anteriores. A cada iteração, um valor de alfa é escolhido
 * de forma probabilística, com base nos scores acumulados, que refletem a qualidade das soluções obtidas com cada alfa.
 * 
 * @param iterations Número de iterações do algoritmo.
 * @return Solution A melhor solução encontrada após as iterações.
 */


Solution Graph::partitionGreedyRandomizedAdaptiveReactive(int iterations) {
    // Lista de valores possíveis de alfa
    std::vector<double> alphas = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};  
    
    // Inicializa as probabilidades uniformemente
    std::vector<double> probabilities(alphas.size(), 1.0 / alphas.size());  
    
    // Inicializa os scores de cada valor de alfa como zero
    std::vector<double> scores(alphas.size(), 0.0);  

    // Inicializa a solução corrente e a melhor solução com o algoritmo guloso puro
    Solution current_solution = partitionGreedy(1);
    Solution best_solution(num_subgraphs);  
    double best_gap = std::numeric_limits<double>::max();  // Inicializa o melhor gap com um valor máximo

    for (int i = 0; i < iterations; ++i) {
        // Gera um valor aleatório para selecionar o alfa com base nas probabilidades
        double random_value = (double)rand() / RAND_MAX;
        double cumulative_prob = 0.0;
        size_t chosen_alpha_idx = 0;

        // Seleciona o índice de alfa acumulando as probabilidades
        for (size_t j = 0; j < probabilities.size(); ++j) {
            cumulative_prob += probabilities[j];
            if (random_value <= cumulative_prob) {
                chosen_alpha_idx = j;
                break;
            }
        }

        // Obtém o valor de alfa escolhido
        double chosen_alpha = alphas[chosen_alpha_idx];
        
        // Executa o algoritmo guloso com o alfa escolhido
        current_solution = partitionGreedy(chosen_alpha);

        // Verifica se a solução atual é melhor que a melhor solução encontrada até agora
        if (current_solution.total_gap < best_gap) {
            best_gap = current_solution.total_gap;
            best_solution = current_solution;  
        }

        // Atualiza o score do alfa escolhido com base na melhoria de gap
        scores[chosen_alpha_idx] += (best_gap - current_solution.total_gap);  

        // Atualiza as probabilidades com base nos scores acumulados
        double total_score = std::accumulate(scores.begin(), scores.end(), 0.0);
        if (total_score > 0) {
            for (size_t j = 0; j < probabilities.size(); ++j) {
                probabilities[j] = scores[j] / total_score;  // Normaliza as probabilidades
            }
        } else {
            // Se todos os scores forem zero, mantém probabilidades uniformes
            std::fill(probabilities.begin(), probabilities.end(), 1.0 / probabilities.size());
        }

        // Comentário para visualizar o progresso das iterações 
        // std::cout << "Iteração " << i + 1 << " - Alfa escolhido: " << chosen_alpha << " - Gap da solução: " << current_solution.total_gap << std::endl;
    }

    return best_solution;
}


/**
 * @brief Obtém a lista de candidatos conectados a um subgrafo a partir dos vértices não atribuídos.
 * 
 * Esta função verifica, para cada vértice no subgrafo atual, quais vértices não atribuídos estão 
 * conectados a ele através de uma aresta. A lista de candidatos consiste nesses vértices conectados,
 * sendo que cada vértice não atribuído é considerado apenas uma vez.
 * 
 * @param subgraph Vetor de IDs dos vértices que compõem o subgrafo atual.
 * @param unassigned_vertices Vetor de IDs dos vértices que ainda não foram atribuídos a nenhum subgrafo.
 * @return std::vector<size_t> A lista de candidatos conectados ao subgrafo.
 */
std::vector<size_t> Graph::getCandidates(const std::vector<size_t>& subgraph, const std::vector<size_t>& unassigned_vertices) {
    std::vector<size_t> candidates;

    // Para cada vértice no subgrafo atual
    for (size_t u : subgraph) {
        // Verifica se existe conexão entre o vértice u do subgrafo e algum vértice não atribuído
        for (size_t v : unassigned_vertices) {
            if (nodes[u]->hasEdge(v)) {  // Se u está conectado a v
                candidates.push_back(v);  // Adiciona v à lista de candidatos
                break;  // Após encontrar uma conexão para u, interrompe o loop interno
            }
        }
    }

    return candidates;  // Retorna a lista de candidatos conectados
}


// --------- Funções de Verificação ---------


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









