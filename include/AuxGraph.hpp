#ifndef AUXGRAPH_HPP
#define AUXGRAPH_HPP

#include <cstddef>      // Para definir size_t
#include <unordered_map> // Para std::unordered_map
#include <unordered_set> // Para std::unordered_set
#include <tuple>        // Para std::tuple

// Estrutura para representar um par de nós (D0Pair)
struct D0Pair {
    size_t node1; // Identificador do primeiro nó
    size_t node2; // Identificador do segundo nó
    
    // Operador de igualdade para comparar pares de nós
    bool operator==(const D0Pair& other) const {
        // Considera o par igual se os nós são os mesmos, independentemente da ordem
        return (node1 == other.node1 && node2 == other.node2) ||
               (node1 == other.node2 && node2 == other.node1);
    }
};

// Estrutura para representar um triplo de nós com um valor adicional (Y0Triple)
struct Y0Triple {
    size_t node1;
    size_t node2;
    size_t node3;
    float value;

    // Construtor
    Y0Triple(size_t n1, size_t n2, size_t n3, float val)
        : node1(n1), node2(n2), node3(n3), value(val) {}

    // Operador de igualdade
    bool operator==(const Y0Triple& other) const {
        return std::tie(node1, node2, node3, value) == std::tie(other.node1, other.node2, other.node3, other.value);
    }
};

// Namespace std para especializar o template hash para as estruturas D0Pair e Y0Triple
namespace std {
    // Especialização do template hash para a estrutura D0Pair
    template <>
    struct hash<D0Pair> {
        size_t operator()(const D0Pair& pair) const {
            // Garante que o hash seja o mesmo para (node1, node2) e (node2, node1)
            return hash<size_t>()(std::min(pair.node1, pair.node2)) ^ hash<size_t>()(std::max(pair.node1, pair.node2));
        }
    };
    
    // Especialização do template hash para a estrutura Y0Triple
    template <>
    struct hash<Y0Triple> {
        size_t operator()(const Y0Triple& triple) const {
            size_t seed = 0;
            auto hashCombine = [&seed](size_t val) {
                seed ^= val + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            };
            hashCombine(hash<size_t>()(triple.node1));
            hashCombine(hash<size_t>()(triple.node2));
            hashCombine(hash<size_t>()(triple.node3));
            hashCombine(hash<float>()(triple.value));
            return seed;
        }
    };
}

#endif // AUXGRAPH_HPP
