#include <iostream>
#include <list>
#include <vector>
#include <limits>

using namespace std;

typedef unsigned int Vertex;
typedef double Weight;

class WeightedDigraphAL
{
private:
    unsigned int num_vertices;
    vector<list<pair<Vertex, Weight>>> adj;

public:
    WeightedDigraphAL(unsigned int num_vertices);
    WeightedDigraphAL();
    ~WeightedDigraphAL();
    void add_edge(Vertex u, Vertex v, Weight w);
    void remove_edge(Vertex u, Vertex v);
    unsigned int get_num_vertices() const { return num_vertices; }
    list<pair<Vertex, Weight>> get_adj(Vertex u) const;
};

WeightedDigraphAL::WeightedDigraphAL(unsigned int qtd_vertices) : num_vertices(qtd_vertices), adj(qtd_vertices)
{
    adj.resize(qtd_vertices);
}

WeightedDigraphAL::WeightedDigraphAL() : num_vertices(0) {}

WeightedDigraphAL::~WeightedDigraphAL() {}

void WeightedDigraphAL::add_edge(Vertex u, Vertex v, Weight w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

void WeightedDigraphAL::remove_edge(Vertex u, Vertex v)
{
    for (auto it = adj[u].begin(); it != adj[u].end(); ++it)
    {
        if (it->first == v)
        {
            adj[u].erase(it);
            break;
        }
    }
}

list<pair<Vertex, Weight>> WeightedDigraphAL::get_adj(Vertex u) const
{
    return adj[u];
}

class Brain
{
private:
    unsigned int num_vertices;
    vector<list<pair<Vertex, Weight>>> adj;

public:
    vector<WeightedDigraphAL> subgraphs;

    Brain(unsigned int num_vertices);
    ~Brain();
    void add_edge(Vertex u, Vertex v, Weight w);
    void remove_edge(Vertex u, Vertex v);
    unsigned int get_num_vertices() const { return num_vertices; }
    list<pair<Vertex, Weight>> get_adj(Vertex u) const;

    void add_internal_graph(unsigned int external_vertex, unsigned int vertices);
    void add_edge_internal(unsigned int external_vertex, Vertex u, Vertex v, Weight w);
    void remove_edge_internal(unsigned int external_vertex, Vertex u, Vertex v);
    list<pair<Vertex, Weight>> get_adj_internal(unsigned int external_vertex, Vertex u) const;
};

Brain::Brain(unsigned int qtd_vertices) : num_vertices(qtd_vertices), adj(qtd_vertices), subgraphs(qtd_vertices)
{
    adj.resize(qtd_vertices);
}

Brain::~Brain() {}

void Brain::add_edge(Vertex u, Vertex v, Weight w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

void Brain::remove_edge(Vertex u, Vertex v)
{
    for (auto it = adj[u].begin(); it != adj[u].end(); ++it)
    {
        if (it->first == v)
        {
            adj[u].erase(it);
            break;
        }
    }
}

list<pair<Vertex, Weight>> Brain::get_adj(Vertex u) const
{
    return adj[u];
}

void Brain::add_internal_graph(unsigned int external_vertex, unsigned int vertices)
{
    if (external_vertex < num_vertices)
    {
        subgraphs[external_vertex] = WeightedDigraphAL(vertices);
    }
}

void Brain::add_edge_internal(unsigned int external_vertex, Vertex u, Vertex v, Weight w)
{
    subgraphs[external_vertex].add_edge(u, v, w);
}

void Brain::remove_edge_internal(unsigned int external_vertex, Vertex u, Vertex v)
{
    subgraphs[external_vertex].remove_edge(u, v);
}

list<pair<Vertex, Weight>> Brain::get_adj_internal(unsigned int external_vertex, Vertex u) const
{
    return subgraphs[external_vertex].get_adj(u);
}

struct Node
{
    Vertex vertex;
    Weight distance;

    Node(Vertex v, Weight d) : vertex(v), distance(d) {}

    bool operator<(const Node &other) const
    {
        return distance < other.distance;
    }
};

class MinPriorityQueue
{
private:
    vector<Node> heap;
    unsigned int heap_size;

    void heapifyDown(int index)
    {
        int leftChild = 2 * index;
        int rightChild = 2 * index + 1;
        int smallest = index;

        if (leftChild < (int)(heap.size()) && heap[leftChild] < heap[smallest])
        {
            smallest = leftChild;
        }

        if (rightChild < static_cast<int>(heap.size()) && heap[rightChild] < heap[smallest])
        {
            smallest = rightChild;
        }

        if (smallest != index)
        {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    MinPriorityQueue() : heap_size(0)
    {
        heap.push_back(Node{(Vertex)-1, -1});
    }
    void build_heap()
    {
        for (int index = heap_size / 2; index >= 1; index--)
        {
            heapifyDown(index);
        }
    }
    bool isEmpty() const
    {
        return heap_size == 0;
    }

    void insert(Node element)
    {
        heap.push_back(element);
        heap_size++;
        build_heap();
    }

    Node extract_min()
    {
        if (isEmpty())
        {
            cerr << "fila vazia" << endl;
            exit(1);
        }

        Node minElement = heap[1];
        heap[1] = heap.back();
        heap.pop_back();
        build_heap();
        heap_size--;
        return minElement;
    }
    void updateHeap(Vertex v, float d)
    {
        for (unsigned int i = 1; i <= heap_size; i++)
        {
            if (heap[i].vertex == v)
            {
                heap[i].distance = d;
                build_heap();
                return;
            }
        }
    }
};

vector<Vertex> dijkstra(const Brain &graph, Vertex source)
{
    vector<Weight> distance(graph.get_num_vertices(), numeric_limits<Weight>::infinity());
    vector<Vertex> predecessors;
    MinPriorityQueue pq;
    distance[source] = 0;
    for (Vertex i = 0; i < graph.get_num_vertices(); i++)
    {
        pq.insert(Node{i, distance[i]});
        predecessors.push_back(-1);
    }
    while (!pq.isEmpty())
    {
        Node current = pq.extract_min();
        Vertex u = current.vertex;
        for (const auto &neighbor : graph.get_adj(u))
        {
            Vertex v = neighbor.first;
            Weight w = neighbor.second;
            if (distance[u] + w < distance[v])
            {
                distance[v] = distance[u] + w;
                pq.updateHeap(v, distance[v]);
                predecessors[v] = u;
            }
        }
    }

    return predecessors;
}

struct union_find
{
    unsigned int size;
    vector<Vertex> key;

    union_find(unsigned int n)
    {
        size = n;
        for (Vertex i = 0; i < n; ++i)
        {
            key.push_back(i);
        }
    }

    Vertex find(Vertex x)
    {
        if (x >= size)
            return -1;
        return key[x];
    }

    void unite(Vertex x, Vertex y)
    {
        Vertex rootX = find(x);
        Vertex rootY = find(y);

        if (rootX != rootY)
        {
            for (unsigned int i = 0; i < size; i++)
            {
                if (key[i] == rootX)
                    key[i] = rootY;
            }
        }
    }
};

struct Aresta
{
    Vertex origem, destino;
    Weight peso;
};

Weight kruskal(const WeightedDigraphAL &subgraph)
{
    unsigned int numVertices = subgraph.get_num_vertices();

    union_find uf(numVertices);

    vector<Aresta> arestas;
    for (Vertex u = 0; u < numVertices; ++u)
    {
        for (const auto &vizinho : subgraph.get_adj(u))
        {
            Vertex v = vizinho.first;
            Weight peso = vizinho.second;
            arestas.push_back({u, v, peso});
        }
    }

    for (unsigned int i = 0; i < arestas.size() - 1; ++i)
    {
        for (unsigned int j = 0; j < arestas.size() - i - 1; ++j)
        {
            if (arestas[j].peso > arestas[j + 1].peso)
            {
                swap(arestas[j], arestas[j + 1]);
            }
        }
    }

    Weight mst_wt = 0.0;

    for (const Aresta &aresta : arestas)
    {
        Vertex origem = aresta.origem;
        Vertex destino = aresta.destino;

        if (uf.find(origem) != uf.find(destino))
        {
            mst_wt += aresta.peso;
            uf.unite(origem, destino);
        }
    }

    return mst_wt;
}

void entrada_dijkstra(Brain &b, unsigned int num_edges)
{
    for (unsigned int i = 0; i < num_edges; ++i)
    {
        Vertex u, v;
        Weight w;
        cin >> u >> v >> w;

        if (w != 0)
        {
            b.add_edge(u - 1, v - 1, w);
        }
    }
}

void entrada_grafos(Brain &b, vector<Vertex> &sick_vector)
{
    unsigned int n = b.get_num_vertices();

    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int num_vertices_block, num_edges_block;
        cin >> num_vertices_block >> num_edges_block;

        Vertex num_sick, sick;

        cin >> num_sick;
        sick_vector.push_back(num_sick);
        for (unsigned int i = 0; i < num_sick; ++i)
        {
            cin >> sick;
        }

        b.add_internal_graph(i, num_vertices_block);

        for (unsigned int j = 0; j < num_edges_block; ++j)
        {
            Vertex u, v;
            Weight w;
            cin >> u >> v >> w;
            b.add_edge_internal(i, u - 1, v - 1, w);
        }
    }
}

int main(int argc, char const *argv[])
{
    vector<Vertex> sick_vector;
    unsigned int num_vertices_brain, num_edges_brain;

    cin >> num_vertices_brain >> num_edges_brain;

    Brain brain{num_vertices_brain};
    entrada_dijkstra(brain, num_edges_brain);

    Vertex source_brain, exit_brain;
    cin >> source_brain >> exit_brain;

    entrada_grafos(brain, sick_vector);

    vector<Vertex> route_brain = dijkstra(brain, source_brain - 1);
    Vertex i = exit_brain - 1;
    Weight distance = 0;

    while (i != (Vertex)-1)
    {
        if (sick_vector[i])
            distance += kruskal(brain.subgraphs[i]);
        i = route_brain[i];
    }

    cout << distance << endl;

    return 0;
}
