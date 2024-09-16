#ifndef AUXEDGE_HPP
#define AUXEDGE_HPP


/*
Estrutura para representar uma aresta auxiliar usada no particionamento do grafo.
Inclui os IDs dos nós de origem e destino, e o gap associado à aresta.
*/ 
struct AuxEdge
{
    int source;
    int target;
    int gap;

    friend bool operator<(AuxEdge e1, AuxEdge e2) 
    {
        return e1.gap < e2.gap;
    }

    friend bool operator>(AuxEdge e1, AuxEdge e2) 
    {
        return e1.gap > e2.gap;
    }

    /*
    bool operator<(const AuxEdge& e) const 
    {
        return gap < e.gap;
    }

    bool operator>(const AuxEdge& e) const 
    {
        return gap > e.gap;
    }
    */
};

#endif 
