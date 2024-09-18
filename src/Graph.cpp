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
/*
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
*/


// pra testar


void Graph::readWeights(std::ifstream& file) {
    std::string line, token;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> token;
        std::cout << "Linha lida (readWeights): " << line << std::endl; // Debugging
        if (token == "param" && iss.str().find("w :=") != std::string::npos) {
            size_t nodeId;
            float weight;
            while (iss >> nodeId >> weight) {
                std::cout << "Adicionando peso: Nó " << nodeId << ", Peso " << weight << std::endl; // Debugging
                addNode(nodeId, weight);
                std::cout << "Peso lido para nó " << nodeId << ": " << weight << std::endl;
            }
            return;
        }
    }
}



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


/*
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
*/


// Fase 4: Leitura das arestas (set E)
/*void Graph::readEdges(std::ifstream& file) {
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
}*/

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


/*// Fase 6: Leitura das triplas Y0 (set Y0)
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
*/



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

/*bool Graph::readInstance(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo: " << filename << std::endl;
        return false;
    }

    // Leitura do número de clusters
    readClusters(file);

    // Leitura dos vértices
    readVertices(file);

    // Leitura das arestas
    readEdges(file);

    // Leitura dos conjuntos D0 e Y0
    readD0(file);
    readY0(file);

    file.close();
    return true;
}*/

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
    for (auto& node_pair : nodes) {
        delete node_pair.second; // Deleta cada nó
    }
    nodes.clear(); // Limpa o mapa de nós
}



void Graph::addNode(size_t id, float weight) {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
        // Cria um novo nó com o peso fornecido
        nodes[id] = new Node(id, weight);
        std::cout << "Nó " << id << " inserido com sucesso. Peso: " << weight << std::endl;
    } else {
        // Não altera o peso se o nó já existir
        std::cout << "Nó " << id << " já existe. Peso: " << it->second->getNodeWeight() << std::endl;
    }
}


/*
void Graph::addNode(size_t id, float weight) {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
        // Cria um novo nó com o peso fornecido
        nodes[id] = new Node(id, weight);
        std::cout << "Nó " << id << " inserido com sucesso." << std::endl;
    } else {
        // Não altera o peso se o nó já existir
        std::cout << "Nó " << id << " já existe. Peso não alterado." << std::endl;
    }
}
*/




/*void Graph::addEdge(size_t source_id, size_t target_id, float weight) {
    // Certifique-se de que os nós existem
    addNode(source_id, 0); // Adiciona o nó de origem com peso inicial (pode ser 0 ou qualquer valor apropriado)
    addNode(target_id, 0); // Adiciona o nó de destino com peso inicial (pode ser 0 ou qualquer valor apropriado)

    // Agora adiciona a aresta
    auto it_source = nodes.find(source_id);
    auto it_target = nodes.find(target_id);

    if (it_source != nodes.end() && it_target != nodes.end()) {
        it_source->second->addEdge(target_id, weight);
        std::cout << "Aresta adicionada entre " << source_id << " e " << target_id << " com peso " << weight << "." << std::endl;
    } else {
        std::cerr << "Nó(s) não encontrado(s) para adicionar aresta: " << source_id << " e/ou " << target_id << std::endl;
    }
}*/

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

    // Adiciona a aresta entre os dois nós
    nodes[from]->addEdge(to, weight);
    std::cout << "Aresta adicionada: (" << from << ", " << to << ") com peso " << weight << std::endl;
}




/*
se depuracao
void Graph::addEdge(size_t source_id, size_t target_id, float weight) { // ultimo que esta certo
    // Adiciona os nós se ainda não existirem
    addNode(source_id, 0); // Adiciona nó de origem
    addNode(target_id, 0); // Adiciona nó de destino

    // Adiciona a aresta do nó de origem ao nó de destino
    auto it_source = nodes.find(source_id);
    auto it_target = nodes.find(target_id);

    if (it_source != nodes.end() && it_target != nodes.end()) {
        it_source->second->addEdge(target_id, weight);
        std::cout << "Aresta adicionada entre " << source_id << " e " << target_id << " com peso " << weight << "." << std::endl;
    } else {
        std::cerr << "Nó(s) não encontrado(s) para adicionar aresta: " << source_id << " e/ou " << target_id << std::endl;
    }
}
*/





// Adiciona uma aresta entre dois nós


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

/*
void Graph::printNodes() const {
    for (const auto& nodePair : nodes) {
        const Node* node = nodePair.second;
        std::cout << "Nó ID: " << node->getId() << ", Peso: " << node->getNodeWeight() << std::endl;
    }
}
*/



void Graph::printEdges() const {
    // Verifica se existem nós no grafo
    if (nodes.empty()) {
        std::cout << "O grafo está vazio." << std::endl;
        return;
    }

    // Itera sobre cada nó do grafo
    for (const auto& node_pair : nodes) {
        size_t node_id = node_pair.first;
        const Node* node = node_pair.second;

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


/*void Graph::printEdges() const {
    std::cout << "Arestas do grafo:" << std::endl;

    bool has_edges = false;
    // Percorre o mapa de nós
    for (const auto& node_pair : nodes) {
        const Node* node = node_pair.second;
        // Percorre as arestas do nó
        for (const auto& edge_pair : node->getEdges()) {
            const Edge& edge = edge_pair.second;
            std::cout << "(" << node->getId() << ", " << edge.getTargetId()
                      << ") com peso " << edge.getWeight() << std::endl;
            has_edges = true;
        }
    }

    if (!has_edges) {
        std::cout << "Nenhuma aresta encontrada." << std::endl;
    }
}

*/








