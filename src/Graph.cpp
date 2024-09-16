#include "../include/Graph.hpp"
#include "../include/Node.hpp"
#include "../include/AuxGraph.hpp"
#include "../include/util.hpp"

#include <algorithm> // Adicione esta linha para usar std::replace
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
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

// Fase 2: Leitura dos vértices (set V)
void Graph::readVertices(std::ifstream& file) {
    std::string line, token;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> token;
        if (token == "set" && iss.str().find("V :=") != std::string::npos) {
            size_t nodeId;
            while (iss >> nodeId) {
                addNode(nodeId, 1.0f);
                std::cout << "Nó lido: " << nodeId << std::endl;
            }
            return;
        }
    }
}

// Fase 3: Leitura dos pesos (param w)
void Graph::readWeights(std::ifstream& file) {
    std::string line, token;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> token;
        if (token == "param" && iss.str().find("w :=") != std::string::npos) {
            size_t nodeId;
            float weight;
            while (iss >> nodeId >> weight) {
                addNode(nodeId, weight);
                std::cout << "Peso lido para nó " << nodeId << ": " << weight << std::endl;
            }
            return;
        }
    }
}

// Fase 4: Leitura das arestas (set E)
void Graph::readEdges(std::ifstream& file) {
    std::string line, token;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> token;
        if (token == "set" && iss.str().find("E :=") != std::string::npos) {
            size_t node1, node2;
            while (iss >> token) {
                if (token.front() == '(' && token.back() == ')') {
                    token.erase(token.begin());
                    token.pop_back();
                    std::replace(token.begin(), token.end(), ',', ' ');
                    std::istringstream edgeStream(token);
                    if (edgeStream >> node1 >> node2) {
                        addEdge(node1, node2, 1.0f);
                        std::cout << "Aresta lida: (" << node1 << ", " << node2 << ")" << std::endl;
                    }
                }
            }
            return;
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

// Fase 6: Leitura das triplas Y0 (set Y0)
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
            return;
        }
    }
}

// Construtor que lê o arquivo de entrada e inicializa o grafo
Graph::Graph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo " << filename << std::endl;
        return;
    }

    // Funçoes auxiliares para construir o grafo
    readClusters(file);
    readVertices(file);
    readWeights(file);
    readEdges(file);
    readD0(file);
    readY0(file);

    file.close();
}


// Destrutor que limpa a memória dos nós
Graph::~Graph() {
    for (auto& node_pair : nodes) {
        delete node_pair.second; // Deleta cada nó
    }
    nodes.clear(); // Limpa o mapa de nós
}

// Adiciona um nó ao grafo
void Graph::addNode(size_t id, float weight) {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
        nodes[id] = new Node(id, weight); // Cria um novo nó
        std::cout << "Nó " << id << " inserido com sucesso." << std::endl;
    } else {
        it->second->setNodeWeight(weight); // Atualiza o peso do nó existente
        std::cout << "Nó " << id << " já existe. Peso atualizado para " << weight << "." << std::endl;
    }
}

// Adiciona uma aresta entre dois nós
void Graph::addEdge(size_t source_id, size_t target_id, float weight) {
    auto it_source = nodes.find(source_id);
    auto it_target = nodes.find(target_id);

    if (it_source != nodes.end() && it_target != nodes.end()) {
        it_source->second->addEdge(target_id, weight);
        std::cout << "Aresta adicionada entre " << source_id << " e " << target_id << " com peso " << weight << "." << std::endl;
    } else {
        std::cerr << "Nó(s) não encontrado(s) para adicionar aresta: " << source_id << " e/ou " << target_id << std::endl;
    }
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
    for (const auto& nodePair : nodes) {
        const Node* node = nodePair.second;
        std::cout << "Nó ID: " << node->getId() << ", Peso: " << node->getNodeWeight() << std::endl;
    }
}


