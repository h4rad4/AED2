#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <list>

class Grafo
{
public:
    Grafo(int numVertices);
    void adicionarAresta(int v1, int v2);
    void mostrarGrafo();

private:
    int numVertices;
    std::list<int> *listaAdjacencia;
};

#endif
