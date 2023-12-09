#include <iostream>
#include <list>
#include <utility>
#include <iomanip>

using namespace std;

typedef unsigned int Vertex;
typedef float Weight;

class VertexWeightPair
{
public:
    Vertex v;
    Weight w;
    VertexWeightPair(Vertex v, Weight w)
    {
        this->v = v;
        this->w = w;
    }
};

class WeightedDigraphAL
{
private:
    unsigned int num_vertices;
    unsigned int num_edges;
    list<VertexWeightPair> *adj;

public:
    WeightedDigraphAL(unsigned int num_vertices);
    ~WeightedDigraphAL();
    void add_edge(Vertex u, Vertex v, Weight w);
    void remove_edge(Vertex u, Vertex v);
    unsigned int get_num_vertices() { return num_vertices; }
    unsigned int get_num_edges() { return num_edges; }
    list<VertexWeightPair> get_adj(Vertex u) { return adj[u]; }
};

/* Construtor ; Destrutor*/
WeightedDigraphAL::WeightedDigraphAL(unsigned int num_vertices) : num_vertices(num_vertices), num_edges(0)
{
    adj = new list<VertexWeightPair>[num_vertices]();
    num_edges = 0;
}

WeightedDigraphAL::~WeightedDigraphAL()
{
    delete[] adj;
}

/* Métodos */
void WeightedDigraphAL::add_edge(Vertex u, Vertex v, Weight w)
{
    for (const auto &pair : adj[u])
    {
        if (pair.v == v)
            return;
    }

    adj[u].push_back(VertexWeightPair(v, w));
    num_edges++;
}

void WeightedDigraphAL::remove_edge(Vertex u, Vertex v)
{
    adj[u].remove_if([&](const VertexWeightPair &pair)
                     { return pair.v == v; });
    num_edges--;
}

void input_graph(WeightedDigraphAL &g, unsigned int num_edges, char const *argv[], int argc)
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

void display_list(list<VertexWeightPair> &lst)
{
    for (const auto &pair : lst)
        cout << "(" << pair.v << ", " << pair.w << "), ";
    cout << "\n";
}

void display_lstadj_graph(WeightedDigraphAL &g)
{
    for (Vertex v = 0; v < g.get_num_vertices(); v++)
    {
        list<VertexWeightPair> lst_adj = g.get_adj(v);
        cout << v << ": ";
        display_list(lst_adj);
    }
}

int main(int argc, char const *argv[])
{
    unsigned int num_vertices = atoi(argv[1]);
    unsigned int num_edges = 0;

    WeightedDigraphAL g{num_vertices};
    input_graph(g, num_edges, argv, argc);

    cout << "num_vertices:" << g.get_num_vertices() << endl;
    cout << "num_edges:" << g.get_num_edges() << endl;
    display_lstadj_graph(g);

    return 0;
}
