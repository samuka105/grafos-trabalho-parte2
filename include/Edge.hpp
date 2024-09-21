#ifndef EDGE_HPP
#define EDGE_HPP

#include <cstddef> 

class Edge {
public:
    Edge();
    Edge(size_t from, size_t to);

    size_t getFrom() const;  // Retorna o nó de origem
    size_t getTo() const;    // Retorna o nó de destino

private:
    size_t from;  // Nó de origem
    size_t to;    // Nó de destino
};

#endif // EDGE_HPP