#include <iostream>
#include <list>
using namespace std;

typedef unsigned int Vertex;

class GraphAL
{
private:
    unsigned int num_vertices;
    unsigned int num_edges;
    std::list<Vertex> *adj;

public:
    GraphAL(unsigned int);
    ~GraphAL();

    void add_edge(Vertex u, Vertex v);
    void remove_edge(Vertex u, Vertex v);

    unsigned int get_num_vertices() { return num_vertices; }
    unsigned int get_num_edges() { return num_edges; }
    list<Vertex> get_adj(Vertex v)
    {
        list<Vertex> values;
        return adj[v];
    }
};

/* Construtor ; Destrutor*/
GraphAL::GraphAL(unsigned int qtd_vertices) : num_vertices(qtd_vertices)
{
    adj = new std::list<Vertex>[qtd_vertices]();

    num_edges = 0;
}

GraphAL::~GraphAL()
{
    for (int u = 0; u < num_vertices; ++u)
        adj[u].clear();

    delete[] adj;
    adj = nullptr;

    // cout << "GraphAL obliterated" << endl;
}

/* Métodos */
void GraphAL::add_edge(Vertex u, Vertex v)
{
    for (const auto &vertex : adj[u])
    {
        if (vertex == v)
            return;
    }

    adj[u].push_back(v);
    adj[v].push_back(u);

    num_edges++;
}

void GraphAL::remove_edge(Vertex u, Vertex v)
{
    adj[u].remove(v);
    adj[v].remove(u);

    num_edges--;
}

void input_graph(GraphAL &g, unsigned int num_edges, const char *argv[], int argc)
{
    unsigned int num_vertices;

    for (int i = 3; i < argc; i = i + 2)
    {
        Vertex u, v;
        u = atoi(argv[i]);
        v = atoi(argv[i + 1]);
        g.add_edge(u, v);
    }
}

void plot_list(list<Vertex> &lst)
{
    for (auto v : lst)
        cout << v << ", ";

    cout << endl;
}

void plot_graph(GraphAL &g)
{
    for (int i = 0; i < g.get_num_vertices(); i++)
    {
        cout << "v[" << i << "]: ";
        list<Vertex> lst = g.get_adj(i);
        plot_list(lst);
    }
}

int main(int argc, char const *argv[])
{
    unsigned int num_vertices = atoi(argv[1]);
    unsigned int num_edges = 0;

    GraphAL g{num_vertices};
    input_graph(g, num_edges, argv, argc);

    cout << "num_vertices:" << g.get_num_vertices() << endl;
    cout << "num_edges:" << g.get_num_edges() << endl;
    plot_graph(g);

    return 0;
}
