#include <iostream>
#include <vector>
#include <string>

using namespace std;

void ComputaTabelaSaltos(const string &padrao, vector<int> &R)
{
    int m = padrao.size();
    for (int i = 0; i < 255; ++i)
    {
        R[i] = m;
    }
    for (int j = 0; j < m - 1; ++j)
    {
        R[padrao[j]] = j;
    }
}

int BoyerMoore(const string &texto, const string &padrao)
{
    int n = texto.size();
    int m = padrao.size();

    vector<int> R(256, 0);

    ComputaTabelaSaltos(padrao, R);

    int salto = 0;

    for (int i = 0; i <= n - m; i += salto)
    {
        salto = 0;

        for (int j = m - 1; j >= 0; --j)
        {
            if (padrao[j] != texto[i + j])
            {
                salto = j - R[texto[i + j]];

                if (salto < 1)
                    salto = 1;

                break;
            }
        }

        if (salto == 0)
        {
            cout << "(" << i << ")" << endl;
            salto = 1;
        }
    }

    return -1;
}

int main()
{
    string texto = "o rato roeu a roupa do rei de roma";
    string padrao = "rei";

    BoyerMoore(texto, padrao);

    return 0;
}
