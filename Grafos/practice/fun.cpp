#include "main.hpp"

Grafo::Grafo(int numVertices)
{
    this->numVertices = numVertices;
    listaAdjacencia = new std::list<int>[numVertices];
}

void Grafo::adicionarAresta(int v1, int v2)
{
    listaAdjacencia[v1].push_back(v2);
    listaAdjacencia[v2].push_back(v1); // Grafo não direcionado
}

void Grafo::mostrarGrafo()
{
    for (int i = 0; i < numVertices; i++)
    {
        std::cout << "Vertice " << i << ": ";
        
        for (auto it = listaAdjacencia[i].begin(); it != listaAdjacencia[i].end(); ++it)
            std::cout << *it << " ";

        std::cout << std::endl;
    }
}
