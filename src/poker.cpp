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
        jogador jogadores[1000];
        getline(file, str);
        istr.str(str);
        int j; // numero de jogadores
        int p; // valor do pingo
        istr >> j >> p;
        for (int k = 0; k < j; k++)
        {
            jogador kEsimo;
            getline(file, str);
            istr.str(str);
            istr >> kEsimo.nome >> kEsimo.valor_aposta;
            for (int l = 0; l < 5; l++)
            {
                std::string carta;
                istr >> carta;
                if (carta.size() == 2)
                {
                    kEsimo.sequencia_cartas[l].numero = carta[0] - '0';
                    kEsimo.sequencia_cartas[l].naipe = carta[1];
                }
                else
                {
                    if(carta[1] == '0') kEsimo.sequencia_cartas[l].numero = 10;
                    if(carta[1] == '1') kEsimo.sequencia_cartas[l].numero = 11;
                    if(carta[1] == '2') kEsimo.sequencia_cartas[l].numero = 12;
                    if(carta[1] == '3') kEsimo.sequencia_cartas[l].numero = 13;
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
    for(int i = 0; i < j; i++) ordenaMao(jogadores[i].sequencia_cartas); 
    for(int i = 0; i < j; i++)
    {
        std::cout << "Jogador " << i << ":\n"; 
        for(int k = 0; k < 5; k++)
        {
            std::cout << jogadores[i].sequencia_cartas[k].naipe << " " << jogadores[i].sequencia_cartas[k].numero << std::endl; 
        }
    }
}


void ordenaMao(carta sequencia_cartas[])
{
    ordenaPorNaipe(sequencia_cartas); 
    int c = 0 ,e = 0,o = 0,p = 0; // num de cartas de cada naipe
    for(int i = 0; i < 5; i++)
    {
        if(sequencia_cartas[i].naipe == 'C') c++; 
        if(sequencia_cartas[i].naipe == 'E') e++; 
        if(sequencia_cartas[i].naipe == 'O') o++; 
        if(sequencia_cartas[i].naipe == 'P') p++; 
    } 
    if(c > 1) ordenaPorNum(sequencia_cartas, c); 
    if(e > 1) ordenaPorNum(sequencia_cartas, e);
    if(o > 1) ordenaPorNum(sequencia_cartas, o);
    if(p > 1) ordenaPorNum(sequencia_cartas, p);
}

void ordenaPorNaipe(carta sequencia_cartas[])
{
    for(int i = 0; i < 5; i++)
    {
        int i_menor = i; 
        for(int j = i; j < 5; j++)
        {
            if(sequencia_cartas[j].naipe < sequencia_cartas[i_menor].naipe)
                i_menor = j; 
        }
        carta aux = sequencia_cartas[i]; 
        sequencia_cartas[i] = sequencia_cartas[i_menor]; 
        sequencia_cartas[i_menor] = aux; 
    }
}

void ordenaPorNum(carta sequencia_cartas[], int num)
{
    for(int i = 0; i < num; i++)
    {
        int i_menor = i; 
        for(int j = i; j < num; j++)
        {
            if(sequencia_cartas[j].numero < sequencia_cartas[i_menor].numero)
                i_menor = j; 
        }
        carta aux = sequencia_cartas[i]; 
        sequencia_cartas[i] = sequencia_cartas[i_menor]; 
        sequencia_cartas[i_menor] = aux; 
    }
}