# Minimum Gap Graph Partitioning Problem (MGGPP)

## Compilação e Execução

### Compilação
Para compilar o código, utilize o seguinte comando:
```bash
g++ main.cpp src/Node.cpp src/Edge.cpp src/Graph.cpp -Iinclude -o execGrupoX
```
Onde “GrupoX” indica a qual grupo o trabalho se refere;
Após compilar, você pode executar o programa com o seguinte comando:
```bash
./meu_programa ./instances/(coloque o nome da instancia aqui)
```
Por exemplo:
```bash
./meu_programa ./instances/n100d03p1i2.txt
```
## Para executar o teste em todos os arquivos:
1) Utilizar a segunda main criada em baixo de testarInstancias(). A primeira main deve estar em um comentario bloco.
2) Executar comando
```bash
g++ main.cpp src/Node.cpp src/Edge.cpp src/Graph.cpp -Iinclude -o execGrupoX
```
Onde “GrupoX” indica a qual grupo o trabalho se refere;
Após compilar, você pode executar o programa com o seguinte comando:
```bash
./execGrupoX
```
### Debug tip
Para compilar o código, utilize o seguinte comando:
```bash
g++ -g main.cpp src/Node.cpp src/Edge.cpp src/Graph.cpp -Iinclude -o execGrupoX
```
Utilize o gdb para executar
```bash
gdb ./execGrupoX
set args ./instances/(coloque o nome da instancia aqui)
run
backtrace
```
