
#include "Graph.hpp"
#include <iostream> 
#include <string>
#include <fstream>

#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

#include <set>
#include <stack>

// Construtor da classe Graph que cria um grafo vazio
Graph::Graph()
{
    // Inicializa todos os membros do grafo
    this->numberNodes = 0;
    this->numberEdges = 0;
    this->directed = false;
    this->weightedEdges = false;
    this->weightedEdges = false;
    this->first = nullptr;
    this->last = nullptr;
}

// Destrutor da classe Graph que libera a memória alocada
Graph::~Graph()
{
// Libera a memória alocada para os nós
    while (first != nullptr) {
        Node* next_node = first->get_next_node();
        delete first;
        first = next_node;
    }
}

void Graph::add_node(size_t node_id, float weight) {
    // Verifica se o nó já existe
    for (const Node& node : nodes) {
        if (node.get_id() == node_id) {  // Verifica o ID usando o getter
            std::cout << "Nó " << node_id << " já existe." << std::endl;
            return;
        }
    }

    // Cria um novo nó e define seus atributos usando os setters
    Node new_node;
    new_node.set_id(node_id);  // Define o ID
    new_node.set_weight(weight);  // Define o peso
    nodes.push_back(new_node);  // Adiciona o nó ao vetor de nós

    std::cout << "Adicionado nó: " << node_id << " com peso " << weight << std::endl;
}


void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight) {
    Node* node1 = nullptr;
    Node* node2 = nullptr;

    // Procura os nós pelos IDs usando os getters
    for (Node& node : nodes) {
        if (node.get_id() == node_id_1) {
            node1 = &node;
        }
        if (node.get_id() == node_id_2) {
            node2 = &node;
        }
    }

    // Verifica se ambos os nós foram encontrados
    if (node1 == nullptr || node2 == nullptr) {
        std::cout << "Não foi possível adicionar a aresta: um ou ambos os nós não foram encontrados." << std::endl;
        return;
    }

    // Cria a nova aresta e define os valores usando setters
    Edge new_edge;
    new_edge.set_target_id(node_id_2); // Define o nó de destino
    new_edge.set_weight(weight);       // Define o peso da aresta
    node1->get_edges().push_back(new_edge);  // Adiciona a aresta ao nó 1

    std::cout << "Adicionada aresta de " << node_id_1 << " para " << node_id_2 << " com peso " << weight << std::endl;

    // Se o grafo não for direcionado, adiciona a aresta inversa
    if (!directed) {
        Edge reverse_edge;
        reverse_edge.set_target_id(node_id_1);  // Define o nó de destino inverso
        reverse_edge.set_weight(weight);        // Define o peso da aresta inversa
        node2->get_edges().push_back(reverse_edge);  // Adiciona a aresta ao nó 2

        std::cout << "Adicionada aresta de " << node_id_2 << " para " << node_id_1 << " com peso " << weight << std::endl;
    }
}

int Graph::conected(size_t node_id_1, size_t node_id_2)
{
    Node* node1 = nullptr;
    Node* node2 = nullptr;

    // Encontra os nós node_id_1 e node_id_2 no grafo
    for (Node& node : nodes) {
        if (node.get_id() == node_id_1) {
            node1 = &node;
        }
        if (node.get_id() == node_id_2) {
            node2 = &node;
        }
        if (node1 != nullptr && node2 != nullptr) {
            break;
        }
    }

    // Verifica se ambos os nós foram encontrados
    if (node1 == nullptr || node2 == nullptr) {
        // Um ou ambos os nós não foram encontrados
        return 0;
    }

    // Verifica se há uma aresta entre node1 e node2
    for (const Edge& edge : node1->get_edges()) {
        if (edge.get_target_id() == node_id_2) {
            // Aresta encontrada, os nós estão conectados
            return 1;
        }
    }

    // Se o grafo não for direcionado, verifica a aresta inversa
    if (!directed) {
        for (const Edge& edge : node2->get_edges()) {
            if (edge.get_target_id() == node_id_1) {
                // Aresta inversa encontrada, os nós estão conectados
                return 1;
            }
        }
    }

    // Nenhuma aresta foi encontrada, os nós não estão conectados
    return 0;
}

// Calcula o "gap" do subgrafo, que é a diferença entre o maior e o menor peso
float Graph::calculate_gap(const Graph& G) const {
    if (nodes.empty()) return 0.0f;

    float min_weight = std::numeric_limits<float>::max();
    float max_weight = std::numeric_limits<float>::min();

    for (const Node& node : nodes) {
        float weight = node.get_weight();  // Usa o getter para obter o peso do nó
        if (weight < min_weight) min_weight = weight;
        if (weight > max_weight) max_weight = weight;
    }

    return max_weight - min_weight;
}


