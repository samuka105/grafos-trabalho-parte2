#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>

/*
Estrutura para representar uma solução de particionamento do grafo.
Inclui os IDs dos vértices no cluster, o valor mais alto e o valor mais baixo encontrado na solução.
*/ 
struct Solution
{
    std::vector<int> vertices_ids;
    int higher;
    int lower;
};

#endif 
