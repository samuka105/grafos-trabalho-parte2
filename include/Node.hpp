



#ifndef NODE_HPP
#define NODE_HPP

#include <unordered_map>
#include <cstddef> 
#include "Edge.hpp"

class Node {
private:
    size_t node_id; 
    float node_weight; 
    std::unordered_map<size_t, Edge> edges ; // Mapa de arestas (target_id -> Edge)
    

public:
    // Construtor / Cria um novo nó com um ID e um peso (opcional, default é 1.0f)
    Node(size_t id, float weight = 1.0f); 

    // Getters
    size_t getId()const;        //Retorna ID do nó 
   
    float getNodeWeight()const; //Retorno peso do nó
    const std::unordered_map<size_t, Edge>& getEdges()const; //Retorno do mapa de arestas do nó

    // Setters
    void setNodeWeight(float weight);  //Define o peso do nó
    void addEdge(size_t target_id, float weight); //Adiciona uma aresta ao nó com o ID de destino e peso especificados
    void removeEdge(size_t target_id); //Remove a aresta com o ID de destino especificado
};

#endif 



