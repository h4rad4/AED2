#include <iostream>
#include <cmath>
#include <string>
#include <list>
#include <vector>

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
private:
    list<Item> tabela[M];

public:
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
        }
    }
};

string inputSymbols()
{
    string text;
    string finalText;

    while (true)
    {
        cin >> text;
        finalText += text;
        if (text.find("~~~") != string::npos)
            break;
    }

    return finalText;
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

void computaTabelaSaltos(const string &padrao, vector<int> &R)
{
    int m = padrao.size();
    for (int i = 0; i < 256; ++i)
    {
        R[i] = -1;
    }
    for (int j = 0; j < m; ++j)
    {
        R[padrao[j]] = j;
    }
}

int boyerMoore(const string &texto, const string &padrao)
{
    int n = texto.size();
    int m = padrao.size();

    vector<int> R(256, -1);

    computaTabelaSaltos(padrao, R);

    int salto = 0;

    for (int i = 0; i <= n - m; i += salto)
    {
        salto = 0;

        for (int j = m - 1; j >= 0; --j)
        {

            if (padrao[j] != texto[i + j])
            {
                salto = j - R[(int)(texto[i + j])];

                if (salto < 1)
                    salto = 1;

                cout << salto << " ";
                break;
            }
        }

        if (salto == 0)
        {
            cout << "0 (" << i << ") ";
            salto = 1;
        }
    }

    cout << endl;

    return -1;
}

vector<string> decifra(string text, int k)
{
    vector<string> sample;
    for (int i = 0; i < k; ++i)
    {
        sample.push_back("");
        for (const auto &carac : text)
        {
            char new_carac = carac + i;

            if (carac == ' ' || carac == '.')
            {
                sample[i] += carac;
            }

            else
            {
                if (carac >= 'A' && carac <= 'Z')
                {
                    if (new_carac <= 'Z')
                    {
                        sample[i] = sample[i] + new_carac;
                    }
                    else
                    {
                        new_carac = new_carac - 26;
                        sample[i] = sample[i] + new_carac;
                    }
                }

                else if (carac >= 'a' && carac <= 'z')
                {
                    if (new_carac <= 'z')
                    {
                        sample[i] = sample[i] + new_carac;
                    }
                    else
                    {
                        new_carac = new_carac - 26;
                        sample[i] = sample[i] + new_carac;
                    }
                }
            }
        }
    }
    return sample;
}

int findMessage(const vector<string> &v)
{
    int i = 0;

    for (auto &str : v)
    {
        i++;
        if (str[0] == 'M')
            return i - 1;
    }
    cout << endl;

    return -1;
}

vector<string> inputPattern()
{
    vector<string> patterns;
    string text;

    while (true)
    {
        cin >> text;
        if (text == "fim" || text == "FIM")
            return patterns;

        patterns.push_back(text);
    }
}

void toUpperCase(string &str)
{
    for (auto &c : str)
        c = toupper(c);
}

int main()
{
    ArtefatoLatim dicionario;

    string alien = inputSymbols();
    string decodificado = decodifica(alien, dicionario);

    vector<string> resultado = decifra(decodificado, 26);

    int index = findMessage(resultado);

    vector<string> patterns = inputPattern();

    cout << resultado[index] << endl;

    for (auto &pattern : patterns)
    {
        toUpperCase(pattern);
        cout << pattern << ": ";
        boyerMoore(resultado[index], pattern);
    }

    return 0;
}
