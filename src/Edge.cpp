#include "../include/Edge.hpp"

Edge::Edge() : from(0), to(0) {}
Edge::Edge(size_t from, size_t to) : from(from), to(to) {}

size_t Edge::getFrom() const {
    return from;
}

size_t Edge::getTo() const {
    return to;
}

