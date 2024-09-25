# Minimum Gap Graph Partitioning Problem (MGGPP)

## Descrição
Este projeto resolve o **Minimum Gap Graph Partitioning Problem (MGGPP)**, um problema de particionamento de grafos ponderados por vértices, com o objetivo de minimizar a diferença entre os pesos dos vértices em cada subgrafo. O problema possui as seguintes regras e restrições:

### Regras e Restrições:
1. **Número predefinido de subgrafos**: 
   - O número de subgrafos `p` é fixo e definido previamente.
   - Restrição: `1 < p < n`, onde `n` é o número total de vértices do grafo.

2. **Subgrafos conectados**: 
   - Todos os subgrafos devem ser conectados, ou seja, deve existir um caminho entre qualquer par de vértices dentro de um mesmo subgrafo.

3. **Vértices desconectados**: 
   - Os subgrafos resultantes não podem ser desconexos ou compostos por apenas um vértice.

4. **Minimização do Gap Total**: 
   - O objetivo é minimizar a soma dos gaps de todos os subgrafos. O gap de um subgrafo é definido como a diferença entre o maior peso (`M_Vr`) e o menor peso (`m_Vr`) dos vértices dentro daquele subgrafo. A função objetivo busca minimizar a soma dos gaps em todos os subgrafos:
     \[
     \min \sum_{r=1}^{p} (M_{V_r} - m_{V_r})
     \]

## Compilação e Execução

### Compilação
Para compilar o código, utilize o seguinte comando:
```bash
g++ main.cpp src/Node.cpp src/Edge.cpp src/Graph.cpp -Iinclude -o meu_programa
```
Após compilar, você pode executar o programa com o seguinte comando:
```bash
./meu_programa
