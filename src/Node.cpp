#include "../include/Node.hpp"
#include "../include/Edge.hpp"
#include <cstddef>
#include <iostream>

#include <iostream>
#include <unordered_map> // Se você estiver usando unordered_map

Node::Node(size_t id, float weight)
    : node_id(id), node_weight(weight) {}  // Usando lista de inicialização

size_t Node::getId() const {
    return node_id;
}

float Node::getNodeWeight() const {
    return node_weight;
}

const std::unordered_map<size_t, Edge>& Node::getEdges() const {
    return edges;
}

void Node::setNodeWeight(float weight) {
    node_weight = weight;
}

void Node::addEdge(size_t target_id, float weight) {
    // Verifica se a aresta já existe
    if (edges.find(target_id) == edges.end()) {
        edges[target_id] = Edge(target_id, weight); // Adiciona a nova aresta
    } else {
        std::cout << "Aresta já existe: " << getId() << " -> " << target_id << std::endl;
    }
}

void Node::removeEdge(size_t target_id) {
    edges.erase(target_id);
}
void Node::updateEdgeWeight(size_t to, float weight) {
    auto it = edges.find(to);
    if (it != edges.end()) {
        it->second.setWeight(weight);
        std::cout << "Peso da aresta atualizada: " << getId() << " -> " << to << " para " << weight << std::endl;
    } else {
        std::cerr << "Erro: Aresta de " << getId() << " para " << to << " não existe." << std::endl;
    }
}

bool Node::edgeExists(size_t to) const {
    return edges.find(to) != edges.end();
}

