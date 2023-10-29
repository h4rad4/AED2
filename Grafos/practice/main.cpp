#include "main.hpp"

int main(int argc, char const *argv[])
{
    Grafo grafo(3);

    grafo.adicionarAresta(0, 1);
    grafo.adicionarAresta(0, 2);
    grafo.adicionarAresta(1, 2);

    grafo.mostrarGrafo();

    return 0;
}
