#include "..\include\poker.hpp"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>

int n;  // numero de rodadas
int di; // dinheiro inicial dos participantes

void leEntrada()
{
    std::ifstream file("entrada.txt");
    std::string str;
    getline(file, str);
    std::istringstream istr(str);
    istr >> n >> di;
    for (int i = 0; i < n; i++)
    {
        int pote = 0;
        jogador jogadores[1000];
        getline(file, str);
        std::istringstream istr1(str);
        int j; // numero de jogadores
        int p; // valor do pingo
        istr1 >> j >> p;
        std::cout << j << " " << p << std::endl;
        for (int k = 0; k < j; k++)
        {
            jogador kEsimo;
            getline(file, str);
            std::istringstream istr2(str);
            istr2 >> kEsimo.nome >> kEsimo.valor_aposta;
            for (int l = 0; l < 5; l++)
            {
                std::string carta;
                istr2 >> carta;
                if (carta.size() == 2)
                {
                    kEsimo.sequencia_cartas[l].numero = carta[0] - '0';
                    kEsimo.sequencia_cartas[l].naipe = carta[1];
                }
                else
                {
                    if (carta[1] == '0')
                        kEsimo.sequencia_cartas[l].numero = 10;
                    if (carta[1] == '1')
                        kEsimo.sequencia_cartas[l].numero = 11;
                    if (carta[1] == '2')
                        kEsimo.sequencia_cartas[l].numero = 12;
                    if (carta[1] == '3')
                        kEsimo.sequencia_cartas[l].numero = 13;
                    kEsimo.sequencia_cartas[l].naipe = carta[2];
                }
            }

            jogadores[k] = kEsimo;
        }
        processaRodada(jogadores, j);
    }

    file.close();
}

void processaRodada(jogador jogadores[], int j)
{
    for (int i = 0; i < j; i++)
        ordenaMao(jogadores[i].sequencia_cartas);
    for (int i = 0; i < j; i++)
    {
        std::cout << "Jogador " << i << ":\n";
        for (int k = 0; k < 5; k++)
        {
            std::cout << jogadores[i].sequencia_cartas[k].naipe << " " << jogadores[i].sequencia_cartas[k].numero << std::endl;
        }
    }
}

// dado que as cartas já estão ordenadas, retorna uma string que corresponde àquela jogada: RSF, SF, FK, FH, F, S, TK, TP, OP, HC 
std::string processaSequenciaCartas(carta sequencia_cartas[])
{
    bool rsf = true; 
    bool sf = true; 
    bool fk = true; 
    bool fh = true; 
    bool f = true;  
    bool s = true; 
    bool tk = true; 
    bool tp = true; 
    bool op = true; 
    bool hc = true; 

    // RSF 
    for(int i = 1; i < 5; i++)
    {
        if(sequencia_cartas[i].naipe != sequencia_cartas[i-1].naipe)
        {
            rsf = false; 
            break; 
        } 
        if(sequencia_cartas[0].numero != 1){
            rsf = false; 
            break; 
        }
        if(i > 1)
        {
            if((sequencia_cartas[i-1].numero + 1) != sequencia_cartas[i].numero)
            {
                rsf = false; 
                break; 
            }
        }
    }
    if(rsf) return "RSF";

    // SF 
    for(int i = 1; i < 5; i++)
    {
        if(sequencia_cartas[i].naipe != sequencia_cartas[i-1].naipe)
        {
            sf = false; 
            break; 
        } 
    }
    // caso 1 2 11 12 13   
    if(sequencia_cartas[0].numero == 1 && sequencia_cartas[1].numero == 2 && sequencia_cartas[2].numero == 11 && sequencia_cartas[3].numero == 12 && sequencia_cartas[4].numero == 13)
    {
        if(sf) return "SF"; 
    } 
    // caso 1 2 3 12 13  
    if(sequencia_cartas[0].numero == 1 && sequencia_cartas[1].numero == 2 && sequencia_cartas[2].numero == 3 && sequencia_cartas[3].numero == 12 && sequencia_cartas[4].numero == 13)
    {
        if(sf) return "SF";
    }

    // caso 1 2 3 4 13  
    if(sequencia_cartas[0].numero == 1 && sequencia_cartas[1].numero == 2 && sequencia_cartas[2].numero == 3 && sequencia_cartas[3].numero == 4 && sequencia_cartas[4].numero == 13)
    {
        if(sf) return "SF";
    }

    // caso geral  
    for(int i = 1; i < 5; i++)
    {
        if((sequencia_cartas[i-1].numero+1) != sequencia_cartas[i].numero)
        {
            sf = false; 
            break; 
        } 
    }
    if(sf) return "SF"; 

    // FK 
    // caso dos 4 primeiros 
    for(int i = 1; i < 5; i++)
    {
        int aux = sequencia_cartas[0].numero; 
        if(aux != sequencia_cartas[i].numero)
        {
            fk = false; 
            break; 
        }
    }
    if(fk) return "FK"; 
    fk = true; 
    // caso dos 4 ultimos 
    for(int i = 0; i < 4; i++)
    {
        int aux = sequencia_cartas[4].numero; 
        if(aux != sequencia_cartas[i].numero)
        {
            fk = false; 
            break; 
        }
    }
    if(fk) return "FK"; 

    // FH 
    
}

void ordenaMao(carta sequencia_cartas[])
{
    ordenaPorNaipe(sequencia_cartas);
    int i = 0;
    int c_ini = -1, c_fim = -1;
    if (sequencia_cartas[i].naipe == 'C')
    {
        c_ini = i;
        c_fim = i;
        while (i < 5 && sequencia_cartas[++i].naipe == 'C')
        {
            c_fim++;
        }
        if (c_ini != c_fim)
            ordenaPorNum(sequencia_cartas, c_ini, c_fim);
    }
    int e_ini = -1, e_fim = -1;
    if (sequencia_cartas[i].naipe == 'E')
    {
        e_ini = i;
        e_fim = i;
        while (i < 5 && sequencia_cartas[++i].naipe == 'E')
        {
            e_fim++;
        }
        if (e_ini != e_fim)
            ordenaPorNum(sequencia_cartas, e_ini, e_fim);
    }
    int o_ini = -1, o_fim = -1;
    if (sequencia_cartas[i].naipe == 'O')
    {
        o_ini = i;
        o_fim = i;
        while (sequencia_cartas[++i].naipe == 'O')
        {
            o_fim++;
        }
        if (o_ini != o_fim)
            ordenaPorNum(sequencia_cartas, o_ini, o_fim);
    }

    int p_ini = -1, p_fim = -1;
    if (sequencia_cartas[i].naipe == 'P')
    {
        p_ini = i;
        p_fim = i;
        while (sequencia_cartas[++i].naipe == 'P')
        {
            p_fim++;
        }
        if (p_ini != p_fim)
            ordenaPorNum(sequencia_cartas, p_ini, p_fim);
    }
}

void ordenaPorNaipe(carta sequencia_cartas[])
{
    for (int i = 0; i < 5; i++)
    {
        int i_menor = i;
        for (int j = i + 1; j < 5; j++)
        {
            if (sequencia_cartas[j].naipe < sequencia_cartas[i_menor].naipe)
                i_menor = j;
        }
        carta aux = sequencia_cartas[i];
        sequencia_cartas[i] = sequencia_cartas[i_menor];
        sequencia_cartas[i_menor] = aux;
    }
}

void ordenaPorNum(carta sequencia_cartas[], int ini, int fim)
{
    for (int i = ini; i <= fim; i++)
    {
        int i_menor = i;
        for (int j = i + 1; j <= fim; j++)
        {
            if (sequencia_cartas[j].numero < sequencia_cartas[i_menor].numero)
                i_menor = j;
        }
        carta aux = sequencia_cartas[i];
        sequencia_cartas[i] = sequencia_cartas[i_menor];
        sequencia_cartas[i_menor] = aux;
    }
}