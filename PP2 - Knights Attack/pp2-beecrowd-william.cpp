#include <iostream>
#include <list>
#include <vector>
#include <limits>
#include <sstream>

using namespace std;

typedef unsigned int Vertex;

template <typename T>
class Graph
{
private:
    unsigned int num_vertices;
    unsigned int num_edges;
    list<T> *adj;

public:
    Graph(unsigned int);
    ~Graph();

    void add_edge(T u, T v);
    void remove_edge(T u, T v);

    unsigned int get_num_vertices() const { return num_vertices; }
    unsigned int get_num_edges() const { return num_edges; }
    const list<T> &get_adj(T v) const { return adj[v]; }
    list<T> &get_adj_list(T v) { return adj[v]; }

    void sort_adj_lists();
    void plot_adj_lists();
};

template <typename T>
Graph<T>::Graph(unsigned int qtd_vertices) : num_vertices(qtd_vertices), num_edges(0)
{
    adj = new list<T>[qtd_vertices];
}

template <typename T>
Graph<T>::~Graph()
{
    delete[] adj;
}

template <typename T>
void Graph<T>::add_edge(T u, T v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);

    num_edges++;
}

template <typename T>
void Graph<T>::remove_edge(T u, T v)
{
    adj[u].remove(v);
    adj[v].remove(u);

    num_edges--;
}

template <typename T>
void Graph<T>::sort_adj_lists()
{
    for (unsigned int v = 0; v < num_vertices; ++v)
    {
        list<T> &adj_list = adj[v];

        bool swapped;
        do
        {
            swapped = false;
            auto it1 = adj_list.begin();
            auto it2 = std::next(it1);

            while (it2 != adj_list.end())
            {
                if (*it1 > *it2)
                {
                    std::swap(*it1, *it2);
                    swapped = true;
                }
                ++it1;
                ++it2;
            }
        } while (swapped);
    }
}

template <typename T>
void Graph<T>::plot_adj_lists()
{
    for (unsigned int v = 0; v < num_vertices; ++v)
    {
        cout << "v[" << v << "]:";
        for (const auto &neighbor : adj[v])
            cout << neighbor << " ";

        cout << endl;
    }
}

class ChessboardGraph
{
public:
    static const int SIZE = 8;

public:
    static Graph<Vertex> createGraph();
    static vector<pair<int, int>> getKnightMoves(int x, int y);
};

// cria o grafo do tabuleiro (os vértices são apenas as posições válidas para o cavalo)
Graph<Vertex> ChessboardGraph::createGraph()
{
    Graph<Vertex> g(SIZE * SIZE);

    for (int x = 0; x < SIZE; ++x)
    {
        for (int y = 0; y < SIZE; ++y)
        {
            int origin = x * SIZE + y;
            vector<pair<int, int>> moves = getKnightMoves(x, y); // possiveis movimentos do cavalo a partir de <x,y>

            for (const auto &move : moves)
            {
                int toX = move.first;
                int toY = move.second;
                int destiny = toX * SIZE + toY;
                g.add_edge(origin, destiny);
            }
        }
    }

    return g;
}

vector<pair<int, int>> ChessboardGraph::getKnightMoves(int x, int y)
{
    vector<pair<int, int>> moves = {{-1, -2}, {-2, -1}, {1, -2}, {-2, 1}, {1, 2}, {2, 1}, {-1, 2}, {2, -1}};
    vector<pair<int, int>> validMoves;

    for (const auto &move : moves)
    {
        int newX = x + move.first;
        int newY = y + move.second;

        if (newX >= 0 && newY >= 0 && newX < SIZE && newY < SIZE)
            validMoves.push_back({newX, newY});
    }

    return validMoves;
}

vector<Vertex> bfs(const Graph<Vertex> &g, Vertex start, Vertex end)
{
    vector<bool> visited(g.get_num_vertices(), false);
    vector<Vertex> predecessors(g.get_num_vertices(), -1);
    vector<Vertex> path;

    visited[start] = true;
    vector<Vertex> queue;
    queue.push_back(start);

    while (!queue.empty())
    {
        Vertex current = queue.front();
        queue.erase(queue.begin());

        if (current == end)
        {
            while (current != -1)
            {
                path.push_back(current);
                current = predecessors[current];
            }
            break;
        }

        for (Vertex neighbor : g.get_adj(current))
        {
            if (!visited[neighbor])
            {
                queue.push_back(neighbor);
                visited[neighbor] = true;
                predecessors[neighbor] = current;
            }
        }
    }

    return path;
}

pair<int, int> convertCoordinate(string coordenada)
{
    int x = coordenada[0] - 'a';
    int y = coordenada[1] - '0' - 1;
    return make_pair(x, y);
}

int findMinor(const vector<unsigned int> &aux)
{
    if (aux.empty())
        return -1;

    int menor = aux[0];

    for (size_t i = 1; i < aux.size(); ++i)
    {
        if (aux[i] < menor)
            menor = aux[i];
    }

    return menor;
}

int main()
{
    Graph<Vertex> chessboardGraph = ChessboardGraph::createGraph();
    chessboardGraph.sort_adj_lists();
    // chessboardGraph.plot_adj_lists();

    int numTests;
    cin >> numTests;
    cin.ignore();

    vector<vector<Vertex>> allTestPositions;

    for (int test = 1; test <= numTests; ++test)
    {
        string entrada;
        getline(cin, entrada);
        stringstream ss(entrada);
        string posicao;
        vector<Vertex> positions;

        while (ss >> posicao)
        {
            pair<int, int> pos = convertCoordinate(posicao);
            Vertex vertex = pos.first * ChessboardGraph::SIZE + pos.second;
            positions.push_back(vertex);
        }

        allTestPositions.push_back(positions);
    }

    // processamento
    for (int test = 0; test < numTests; ++test)
    {
        const vector<Vertex> &positions = allTestPositions[test];
        unsigned int end = positions[4];

        vector<unsigned int> moves;

        for (int i = 0; i < 4; ++i)
        {
            unsigned int start = positions[i];
            vector<Vertex> shortest = bfs(chessboardGraph, start, end);

            if (shortest.empty())
            {
                moves.push_back(numeric_limits<unsigned int>::max());
            }
            else
            {
                moves.push_back(shortest.size() - 2);
            }
        }

        int minMoves = findMinor(moves);

        for (unsigned int i : moves)
        {
            if (i == minMoves)
            {
                cout << i;
            }
        }
    }

    return 0;
}


