#include <iostream>
#include <cmath>
#include <string>
#include <list>

using namespace std;

const int TAM_ASCII = 256;
const int M = 7;

struct Item
{
    string symbol;
    char latin;
};

int h(const string &key)
{
    int hash = 0;
    for (unsigned int i = 0; i < key.length(); i++)
        hash += static_cast<int>(key[i]) * (int)pow(TAM_ASCII, key.length() - i - 1) % M;

    return hash % M;
}

class ArtefatoLatim
{
public:
    list<Item> tabela[M];

    ArtefatoLatim()
    {
        addItem(":::", 'A');
        addItem(".::", 'B');
        addItem(":.:", 'C');
        addItem("::.", 'D');
        addItem(":..", 'E');
        addItem(".:.", 'F');
        addItem("..:", 'G');
        addItem("...", 'H');
        addItem("|::", 'I');
        addItem(":|:", 'J');
        addItem("::|", 'K');
        addItem("|.:", 'L');
        addItem(".|:", 'M');
        addItem(".:|", 'N');
        addItem("|:.", 'O');
        addItem(":|.", 'P');
        addItem(":.|", 'Q');
        addItem("|..", 'R');
        addItem(".|.", 'S');
        addItem("..|", 'T');
        addItem(".||", 'U');
        addItem("|.|", 'V');
        addItem("||.", 'W');
        addItem("-.-", 'X');
        addItem(".--", 'Y');
        addItem("--.", 'Z');
        addItem("---", ' ');
        addItem("~~~", '.');
    }

    void addItem(const string &symbol, char latin)
    {
        int index = h(symbol);
        tabela[index].push_back({symbol, latin});
    }

    char decodifica(const string &symbol) const
    {
        int index = h(symbol);

        for (const auto &item : tabela[index])
        {
            if (item.symbol == symbol)
                return item.latin;
        }

        return '\0';
    }

    void printaHash()
    {
        for (int i = 0; i < M; ++i)
        {
            cout << i << ": ";
            for (auto &item : tabela[i])
                cout << item.latin << " ";

            cout << endl;
        }
    }
};

string input_symbols()
{
    string text;
    cin >> text;

    while (true)
    {
        if (text.find("~~~"))
            return text;
    }
}

string decodifica(string alien, ArtefatoLatim dicionario)
{
    string texto_decodificado;

    for (unsigned int i = 0; i < alien.length(); i += 3)
    {
        string bloco = alien.substr(i, 3);
        texto_decodificado += dicionario.decodifica(bloco);
    }

    return texto_decodificado;
}

int main()
{
    ArtefatoLatim dicionario;
    //dicionario.printaHash();

    string alien = input_symbols();
    string decodificado = decodifica(alien, dicionario);

    cout << decodificado << endl;

    return 0;
}
