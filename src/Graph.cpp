#include "../include/Graph.hpp"
#include "../include/Node.hpp"
#include "../include/AuxGraph.hpp"
#include "../include/util.hpp"

#include <algorithm> // Adicione esta linha para usar std::replace
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Construtor que lê o arquivo de entrada e inicializa o grafo
Graph::Graph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo " << filename << std::endl;
        return;
    }

    std::string line;
    bool readingVertices = false; // Flag para indicar a leitura de vértices
    bool readingWeights = false;  // Flag para indicar a leitura de pesos
    bool readingEdges = false;    // Flag para indicar a leitura de arestas
    bool readingD0 = false;       // Flag para indicar a leitura de pares D0
    bool readingY0 = false;       // Flag para indicar a leitura de triplas Y0

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            // Ignora linhas vazias ou comentários
            continue;
        }

        std::istringstream iss(line);
        std::string token;
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
                }
            }
        } else if (token == "set") {
            std::string setName;
            iss >> setName;
            if (setName == "V") {
                readingVertices = true;
                readingWeights = false;
                readingEdges = false;
                readingD0 = false;
                readingY0 = false;
                std::cout << "Início da leitura de vértices" << std::endl;
            } else if (setName == "w") {
                readingVertices = false;
                readingWeights = true;
                readingEdges = false;
                readingD0 = false;
                readingY0 = false;
                std::cout << "Início da leitura de pesos" << std::endl;
            } else if (setName == "E") {
                readingVertices = false;
                readingWeights = false;
                readingEdges = true;
                readingD0 = false;
                readingY0 = false;
                std::cout << "Início da leitura de arestas" << std::endl;
            } else if (setName == "D0") {
                readingVertices = false;
                readingWeights = false;
                readingEdges = false;
                readingD0 = true;
                readingY0 = false;
                std::cout << "Início da leitura de pares D0" << std::endl;
            } else if (setName == "Y0") {
                readingVertices = false;
                readingWeights = false;
                readingEdges = false;
                readingD0 = false;
                readingY0 = true;
                std::cout << "Início da leitura de triplas Y0" << std::endl;
            }
        } else if (readingVertices) {
            size_t nodeId;
            while (iss >> nodeId) {
                if (nodes.find(nodeId) == nodes.end()) {
                    addNode(nodeId, 1.0f);
                    std::cout << "Lendo nó (vertices): " << nodeId << std::endl;
                } else {
                    std::cout << "Nó já existe: " << nodeId << std::endl;
                }
            }
            readingVertices = false;  // Reseta a flag após processar
        } else if (readingWeights) {
            size_t nodeId;
            float weight;
            while (iss >> nodeId >> weight) {
                if (nodes.find(nodeId) != nodes.end()) {
                    nodes[nodeId]->setNodeWeight(weight);
                    std::cout << "Lendo nó e peso (weights): " << nodeId << " com peso " << weight << std::endl;
                } else {
                    addNode(nodeId, weight);
                    std::cout << "Nó lido e adicionado: " << nodeId << std::endl;
                }
            }
            readingWeights = false;  // Reseta a flag após processar
        } else if (readingEdges) {
            size_t node1, node2;
            while (iss >> token) {
                // Verifica se o token está no formato (n1,n2)
                if (token.front() == '(' && token.back() == ')') {
                    token.erase(token.begin()); // Remove '('
                    token.pop_back(); // Remove ')'
                    std::replace(token.begin(), token.end(), ',', ' '); // Substitui ',' por espaço
                    std::istringstream edgeStream(token);
                    if (edgeStream >> node1 >> node2) {
                        addEdge(node1, node2, 1.0f);
                        std::cout << "Lendo aresta: (" << node1 << ", " << node2 << ")" << std::endl;
                    }
                }
            }
            readingEdges = false;  // Reseta a flag após processar
        } else if (readingD0) {
            size_t node1, node2;
            while (iss >> token) {
                // Verifica se o token está no formato (n1,n2)
                if (token.front() == '(' && token.back() == ')') {
                    token.erase(token.begin()); // Remove '('
                    token.pop_back(); // Remove ')'
                    std::replace(token.begin(), token.end(), ',', ' '); // Substitui ',' por espaço
                    std::istringstream pairStream(token);
                    if (pairStream >> node1 >> node2) {
                        addD0Pair(node1, node2);
                        std::cout << "Lendo par D0: (" << node1 << ", " << node2 << ")" << std::endl;
                    }
                }
            }
            readingD0 = false;  // Reseta a flag após processar
        } else if (readingY0) {
            size_t node1, node2, node3;
            float value;
            while (iss >> token) {
                // Verifica se o token está no formato (n1,n2,n3,v)
                if (token.front() == '(' && token.back() == ')') {
                    token.erase(token.begin()); // Remove '('
                    token.pop_back(); // Remove ')'
                    std::replace(token.begin(), token.end(), ',', ' '); // Substitui ',' por espaço
                    std::istringstream tripleStream(token);
                    if (tripleStream >> node1 >> node2 >> node3 >> value) {
                        addY0Triple(node1, node2, node3, value);
                        std::cout << "Lendo tripla Y0: (" << node1 << ", " << node2 << ", " << node3 << ", " << value << ")" << std::endl;
                    }
                }
            }
            readingY0 = false;  // Reseta a flag após processar
        }
    }

    file.close();  // Fecha o arquivo após a leitura
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

