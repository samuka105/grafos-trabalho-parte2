#include "../include/Graph.hpp"
#include "../include/Node.hpp"
#include "../include/AuxGraph.hpp"
#include "../include/util.hpp"

#include <algorithm> // Adicione esta linha para usar std::replace
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>


#include <unordered_map>
#include <memory> 
// Auxiliares para o construtor
// Fase 1: Leitura do número de clusters (param p)
void Graph::readClusters(std::ifstream& file) {
    std::string line, token;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> token;
        if (token == "param") {
            std::string paramName;
            iss >> paramName;
            if (paramName == "p") {
                std::string equalSign;
                iss >> equalSign;
                if (equalSign == ":=") {
                    iss >> clusters;
                    std::cout << "Número de clusters lido: " << clusters << std::endl;
                    return;
                }
            }
        }
    }
}


// Fase 3: Leitura dos pesos dos vértices
void Graph::readWeights(std::ifstream& file) {
    std::string line;
    bool readingWeights = false;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.find("param w :=") != std::string::npos) {
            readingWeights = true;
            continue;
        }

        if (readingWeights) {
            std::istringstream iss(line);
            size_t nodeId;
            float weight;

            while (iss >> nodeId >> weight) {
                addNode(nodeId, weight); // Adiciona o nó com o peso lido
                std::cout << "Nó lido: " << nodeId << " com peso: " << weight << std::endl;
            }

            // Se encontrar o ponto-e-vírgula ';', termina a leitura dos pesos
            if (line.find(";") != std::string::npos) {
                break;
            }
        }
    }
}
// Fase 2: Leitura dos vértices (set V)
// Fase 2: Leitura dos vértices (set V)
void Graph::readVertices(std::ifstream& file) {
    std::string line, token;
    bool readingVertices = false;  // Adiciona uma flag para marcar o início da leitura dos vértices
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        iss >> token;
        
        // Verifica se estamos começando a ler os vértices
        if (!readingVertices) {
            if (token == "set" && line.find("V :=") != std::string::npos) {
                readingVertices = true;  // Inicia a leitura dos vértices
                continue;
            }
        }
        
        if (readingVertices) {
            size_t nodeId;
            while (iss >> nodeId) {
                addNode(nodeId, 1.0f);  // Adiciona o nó com peso padrão 1.0 (caso o peso não seja lido nesta fase)
                std::cout << "Nó lido: " << nodeId << std::endl;
            }

            // Se encontrar o ponto-e-vírgula ';', termina a leitura dos vértices
            if (line.find(";") != std::string::npos) {
                break;
            }
        }
    }
}



// pra testar








// pra testar
void Graph::readEdges(std::ifstream& file) {
    std::string line, token;
    bool readingEdges = false;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;  // Ignorar comentários e linhas vazias
        
        std::istringstream iss(line);
        
        // Verifica se estamos começando a ler as arestas
        if (!readingEdges) {
            iss >> token;
            if (token == "set" && line.find("E :=") != std::string::npos) {
                readingEdges = true;  // Começa a ler as arestas
                continue;  // Vai para a próxima linha
            }
        }

        if (readingEdges) {
            size_t node1, node2;
            while (iss >> token) {
                // Remover parênteses ao redor da aresta
                if (token.front() == '(' && token.back() == ')') {
                    token.erase(token.begin());  // Remove '('
                    token.pop_back();  // Remove ')'
                    std::replace(token.begin(), token.end(), ',', ' ');  // Substitui ',' por espaço
                
                    // Lê os dois números da aresta
                    std::istringstream edgeStream(token);
                    if (edgeStream >> node1 >> node2) {
                        addEdge(node1, node2, 1.0f);
                        std::cout << "Aresta lida: (" << node1 << ", " << node2 << ")" << std::endl;
                    }
                }
            }

            // Se encontrar o ponto-e-vírgula ';', termina a leitura das arestas
            if (line.find(";") != std::string::npos) {
                break;  // Saímos do loop ao encontrar o fim da seção de arestas
            }
        }
    }
}


// Fase 5: Leitura dos pares D0 (set D0)
void Graph::readD0(std::ifstream& file) {
    std::string line, token;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> token;
        if (token == "set" && iss.str().find("D0 :=") != std::string::npos) {
            size_t node1, node2;
            while (iss >> token) {
                if (token.front() == '(' && token.back() == ')') {
                    token.erase(token.begin());
                    token.pop_back();
                    std::replace(token.begin(), token.end(), ',', ' ');
                    std::istringstream pairStream(token);
                    if (pairStream >> node1 >> node2) {
                        addD0Pair(node1, node2);
                        std::cout << "Par D0 lido: (" << node1 << ", " << node2 << ")" << std::endl;
                    }
                }
            }
            return;
        }
    }
}
void Graph::readY0(std::ifstream& file) {
    std::string line, token;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> token;
        if (token == "set" && iss.str().find("Y0 :=") != std::string::npos) {
            size_t node1, node2, clusterId;
            float value;
            while (iss >> token) {
                if (token.front() == '(' && token.back() == ')') {
                    token.erase(token.begin());
                    token.pop_back();
                    std::replace(token.begin(), token.end(), ',', ' ');
                    std::istringstream tripleStream(token);
                    if (tripleStream >> node1 >> node2 >> clusterId >> value) {
                        addY0Triple(node1, node2, clusterId, value);
                        std::cout << "Tripla Y0 lida: (" << node1 << ", " << node2 << ", Cluster: " << clusterId << ", Valor: " << value << ")" << std::endl;
                    }
                }
            }
            // Se encontrar o "end;", termina a leitura das triplas Y0
            if (line.find("end;") != std::string::npos) {
                break;
            }
        }
    }
}





// Construtor que lê o arquivo de entrada e inicializa o grafo


bool Graph::readInstance(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo " + filename);
    }

    try {
        readClusters(file);
        readVertices(file);
        readEdges(file);
        readD0(file);
        readY0(file);
    } catch (const std::exception& e) {
        std::cerr << "Erro ao ler o arquivo: " << e.what() << std::endl;
        return false;
    }

    file.close();
    return true;
}



Graph::Graph(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir o arquivo " + filename);
        }

        // Funçoes auxiliares para construir o grafo
        readClusters(file);
        readVertices(file);
        readWeights(file);
        readEdges(file);
        readD0(file);
        readY0(file);

        file.close();
    } catch (const std::exception& e) {
        std::cerr << "Erro ao construir o grafo: " << e.what() << std::endl;
        throw; // Re-lança a exceção para que possa ser tratada pelo chamador
    }
}
// Destrutor que limpa a memória dos nós
Graph::~Graph() {
    // Não é necessário deletar manualmente, o std::unique_ptr cuida disso
    nodes.clear(); // Limpa o mapa de nós
}




void Graph::addNode(size_t id, float weight) {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
        // Cria um novo nó com o peso fornecido
        nodes[id] = std::make_unique<Node>(id, weight);
        std::cout << "Nó " << id << " inserido com sucesso. Peso: " << weight << std::endl;
    } else {
        // Atualiza o peso se o nó já existir
        it->second->setNodeWeight(weight); // Supondo que setWeight esteja definido na classe Node
        std::cout << "Nó " << id << " já existe. Peso atualizado para: " << weight << std::endl;
    }
}





void Graph::addEdge(size_t from, size_t to, float weight) {
    // Verifica se ambos os nós já existem no grafo
    if (nodes.find(from) == nodes.end()) {
        std::cerr << "Erro: Nó " << from << " não foi encontrado." << std::endl;
        return;
    }
    if (nodes.find(to) == nodes.end()) {
        std::cerr << "Erro: Nó " << to << " não foi encontrado." << std::endl;
        return;
    }

    // Verifica se a aresta já existe
    if (!nodes[from]->edgeExists(to)) {
        nodes[from]->addEdge(to, weight);
        std::cout << "Aresta adicionada: (" << from << ", " << to << ") com peso " << weight << std::endl;
    } else {
        std::cout << "Aresta já existe: (" << from << ", " << to << ") com peso " << weight << std::endl;
    }
}






bool Graph::edgeExists(size_t from, size_t to) {
    auto it = nodes.find(from);
    if (it != nodes.end()) {
        const auto& edges = it->second->getEdges(); // Supondo que você tenha um método getEdges() na classe Node
        for (const auto& edgePair : edges) { // `edgePair` é um std::pair
            if (edgePair.second.getTargetId() == to) { // Acessa o nó de destino da aresta
                return true;
            }
        }
    }
    return false; // A aresta não existe
}






size_t Graph::getClusters() const {
    return clusters; // Retorna o número de clusters
}


// Adiciona um par D0
void Graph::addD0Pair(size_t node1, size_t node2) {
    d0Pairs.insert({node1, node2});
    std::cout << "Par D0 adicionado: (" << node1 << ", " << node2 << ")" << std::endl;
}

// Adiciona uma tripla Y0
void Graph::addY0Triple(size_t node1, size_t node2, size_t node3, float value) {
    y0Triples.emplace(node1, node2, node3, value);
    std::cout << "Tripla Y0 adicionada: (" << node1 << ", " << node2 << ", " << node3 << ", " << value << ")" << std::endl;
}



void Graph::printNodes() const {
    if (nodes.empty()) {
        std::cout << "Nenhum nó foi encontrado no grafo." << std::endl;
    } else {
        std::cout << "Nós no grafo: ";
        for (const auto& node_pair : nodes) {
            std::cout << node_pair.first << " ";
        }
        std::cout << std::endl;
    }
}





void Graph::printEdges() const {
    // Verifica se existem nós no grafo
    if (nodes.empty()) {
        std::cout << "O grafo está vazio." << std::endl;
        return;
    }

    // Itera sobre cada nó do grafo
    for (const auto& node_pair : nodes) {
        size_t node_id = node_pair.first;
        const Node* node = node_pair.second.get();


        std::cout << "Arestas do nó " << node_id << ":" << std::endl;

        // Verifica se o nó possui arestas
        const auto& edges = node->getEdges();
        if (edges.empty()) {
            std::cout << "  Este nó não possui arestas." << std::endl;
        } else {
            // Imprime cada aresta associada ao nó
            for (const auto& edge_pair : edges) {
                size_t target_id = edge_pair.first;
                const Edge& edge = edge_pair.second;
                std::cout << "  " << node_id << " -> " << target_id << " [Peso: " << edge.getWeight() << "]" << std::endl;
            }
        }
    }
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
void Graph::removeNode(size_t node_id) {
    // Remove todas as arestas conectadas ao nó
    nodes.erase(node_id);
    // Remover as arestas de outros nós que apontam para o nó removido
    for (auto& pair : nodes) {
        pair.second->removeEdge(node_id);
    }
}
bool Graph::validateGraph() const {
    for (const auto& pair : nodes) {
        const Node* node = pair.second.get();
        if (!node) {
            std::cerr << "Erro: Nó " << pair.first << " é nulo!" << std::endl;
            return false;
        }
        // Verifica se o nó tem as arestas corretas
        for (const auto& edge_pair : node->getEdges()) {
            size_t target_id = edge_pair.first;
            const Edge& edge = edge_pair.second;
            if (nodes.find(target_id) == nodes.end()) {
                std::cerr << "Erro: Aresta com destino " << target_id << " aponta para um nó inexistente!" << std::endl;
                return false;
            }
        }
    }
    return true;
}









