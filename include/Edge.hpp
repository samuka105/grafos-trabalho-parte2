#ifndef EDGE_HPP
#define EDGE_HPP

#include <cstddef> 

class Edge {
public:
    Edge();
    Edge(size_t from, size_t to);

    size_t getFrom() const;  
    size_t getTo() const;    

private:
    size_t from;  
    size_t to;    
};

#endif 