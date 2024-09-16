#ifndef CANDIDATE_HPP
#define CANDIDATE_HPP

/*
Estrutura para representar um candidato a ser considerado durante o particionamento do grafo.
Inclui o cluster ao qual o candidato pertence, os IDs dos nós de origem e destino, e o aumento do gap que seria causado pela inclusão do candidato no cluster.
*/ 
struct Candidate
{
    int cluster;
    int source_id;
    int target_id;
    int increase_gap;

    friend bool operator<(Candidate c1, Candidate c2) 
    {
        return c1.increase_gap < c2.increase_gap;
    }

    friend bool operator>(Candidate c1, Candidate c2) 
    {
        return c1.increase_gap > c2.increase_gap;
    }

    friend bool operator==(Candidate c1, Candidate c2)
    {
        return c1.target_id == c2.target_id;
    }
};

#endif 
