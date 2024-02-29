#include <iostream>
#include <list>
#include <utility>
#include <iomanip>
#include <limits>

#define INFINITY_WEIGHT std::numeric_limits<Weight>::infinity();
using namespace std;

typedef unsigned int Vertex;
typedef double Weight;

class WeightedGraphAM
{
private:
    unsigned int num_vertices;
    unsigned int num_edges;
    Weight **adj;

public:
    WeightedGraphAM(unsigned int num_vertices);
    ~WeightedGraphAM();
    void add_edge(Vertex u, Vertex v, Weight w);
    void remove_edge(Vertex u, Vertex v);
    unsigned int get_num_vertices() { return num_vertices; }
    unsigned int get_num_edges() { return num_edges; }
    Weight get_weight_edge(Vertex u, Vertex v) { return adj[u][v]; };
    list<Vertex> get_adj(Vertex u)
    {
        list<Vertex> values;

        for (int i = 0; i < num_vertices; i++)

            if (adj[u][i] != 0)
                values.push_back(i);

        return values;
    };
};

WeightedGraphAM::WeightedGraphAM(unsigned int qtd_vertices) : num_vertices(qtd_vertices), num_edges(0)
{
    const unsigned int SIZE = qtd_vertices;
    adj = new Weight *[SIZE];

    for (unsigned int i = 0; i < qtd_vertices; i++)
    {
        adj[i] = new Weight[SIZE]();

        // Inicializa a linha com infinito
        for (unsigned int j = 0; j < SIZE; j++)
            adj[i][j] = INFINITY_WEIGHT;
    }

    num_edges = 0;
}

WeightedGraphAM::~WeightedGraphAM()
{
    for (int i = 0; i < num_vertices; i++)
        delete[] adj[i];

    adj = nullptr;
    // cout << "WeightedGraphAM obliterated" << endl;
}

void WeightedGraphAM::add_edge(Vertex u, Vertex v, Weight w)
{
    if (adj[u][v] != std::numeric_limits<Weight>::infinity())
        return;

    adj[u][v] = w;
    adj[v][u] = w;

    num_edges++;
}

void WeightedGraphAM::remove_edge(Vertex u, Vertex v)
{
    adj[u][v] = 0;
    adj[v][u] = 0;
    num_edges--;
}

void input_graph(WeightedGraphAM &g, unsigned int num_edges, const char *argv[], int argc)
{
    unsigned int num_vertices;

    for (int i = 3; i < argc; i = i + 3)
    {
        Vertex u, v;
        Weight w;
        u = atoi(argv[i]);
        v = atoi(argv[i + 1]);
        w = std::stof(argv[i + 2]);
        g.add_edge(u, v, w);
    }
}

void display_list(list<Vertex> values)
{
    for (auto it = values.begin(); it != values.end(); ++it)
        cout << *it << " ";
}

void display_matadj_graph(WeightedGraphAM &g)
{
    int k = 3;

    // Constrói cada linha da matriz
    for (unsigned int i = 0; i < g.get_num_vertices(); i++)
    {
        // Valores da linha
        for (unsigned int j = 0; j < g.get_num_vertices(); j++)
            cout << std::setw(k) << g.get_weight_edge(i, j) << " ";
        cout << endl;
    }
}

int main(int argc, char const *argv[])
{
    unsigned int num_vertices = atoi(argv[1]);
    unsigned int num_edges = 0;

    WeightedGraphAM g{num_vertices};
    input_graph(g, num_edges, argv, argc);

    // g.remove_edge(0, 1);

    cout << "num_vertices:" << g.get_num_vertices() << endl;
    cout << "num_edges:" << g.get_num_edges() << endl;
    display_matadj_graph(g);

    return 0;
}
