#include "..\include\poker.hpp"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdio>

int n;  // numero de rodadas
int di; // dinheiro inicial dos participantes
int pote;
int nv; // numero de vencedores
int j; // numero de jogadores
int p; // valor do pingo
jogador vencedores[12]; 

void leEntrada()
{
    // std::ifstream file("entrada.txt");
    // std::string str;
    // getline(file, str);
    // std::istringstream istr(str);
    // istr >> n >> di;
    FILE *arq = fopen("entrada.txt", "r");
    fscanf(arq, "%d %d", &n, &di); 
    std::cout << "n: " << n << " di: " << di << std::endl;
    for (int i = 0; i < n; i++)
    {
        pote = 0;
        nv = 1; 
        jogador jogadores[12];
        fscanf(arq, "%d %d\n", &j, &p); 
        std::cout << j << " " << p << std::endl;
        for (int k = 0; k < j; k++)
        {
            jogador kEsimo;
            char nome[100]; 
            fscanf(arq, "%[^0-9] %d ", nome, &kEsimo.valor_aposta);
            kEsimo.nome = nome; 
            pote += kEsimo.valor_aposta;
            kEsimo.saldo = di - kEsimo.valor_aposta;
            for (int l = 0; l < 5; l++)
            {
                char c[5];
                fscanf(arq, "%s ", c); 
                std::string carta = c; 
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

    fclose(arq); 
}

void processaRodada(jogador jogadores[], int j)
{
    for (int i = 0; i < j; i++)
        ordenaMao(jogadores[i].sequencia_cartas);
    for (int i = 0; i < j; i++)
        jogadores[i].classificacaoJogada = processaSequenciaCartas(jogadores[i].sequencia_cartas);
    ordenaJogadorPorClassificacao(jogadores, j);
    vencedores[0] = jogadores[0]; 
    achaMaiorCarta(jogadores); 
    trataEmpateJogada(jogadores); 
    escreveSaida(jogadores); 
}

void escreveSaida(jogador jogadores[])
{
    std::ofstream file("saida.txt");     
    file << nv << " " << pote/nv << " " << jogadaParaString(jogadores[0].classificacaoJogada) << "\n"; 
    for(int i = 0; i < nv; i++)
    {
        file << vencedores[i].nome << '\n'; 
        vencedores[i].saldo += pote/nv; 
    }
    file << "####\n"; 
    for(int i = 0; i < nv; i++)
    {
        file << vencedores[i].nome << " " << vencedores[i].saldo << '\n'; 
    }
    for(int i = 0; i < j; i++)
    {   
        bool e_vencedor = false; 
        // se ele for um dos vencedores ja foi impresso anteriormente
        for(int k = 0; k < nv; k++) 
        {
            if(jogadores[i].nome == vencedores[k].nome)
                e_vencedor = true;  
        }
        if(!e_vencedor)
            file << jogadores[i].nome << " " << jogadores[i].saldo << '\n';  
    }
    file.close(); 
}

std::string jogadaParaString(int classificacaoJogada)
{
    switch (classificacaoJogada)
    {
    case RSF:
        return "RSF";
    case SF:
        return "SF";
    case FK:
        return "FK";
    case FH:
        return "FH";
    case F:
        return "F";
    case S:
        return "S";
    case TK:
        return "TK";
    case TP:
        return "TP";
    case OP:
        return "OP";
    case HC:
        return "HC";
    }
}

void trataEmpateJogada(jogador jogadores[])
{
    int rsf = 0;
    int sf = 0;
    int fk = 0;
    int fh = 0;
    int f = 0;
    int s = 0;
    int tk = 0;
    int tp = 0;
    int op = 0;
    int hc = 0;
    int p_sf; // posiçao do primeiro jogador com sf se houver
    for (int i = 0; i < j; i++)
    {
        switch (jogadores[i].classificacaoJogada)
        {
        case RSF:
            rsf++;
            break;
        case SF:
            sf++;
            if(sf == 1) p_sf = i; 
            break;
        case FK:
            fk++;
            break;
        case FH:
            fh++;
            break;
        case F:
            f++;
            break;
        case S:
            s++;
            break;
        case TK:
            tk++;
            break;
        case TP:
            tp++;
            break;
        case OP:
            op++;
            break;
        case HC:
            hc++;
            break;
        }
    }
    if (rsf > 1)
    {
        for(int i = 0; i < rsf; i++)
        {
            vencedores[i] = jogadores[i]; 
            nv++; 
        }
        nv--; 
    }
    if (sf > 1)
    {
        int maior = jogadores[p_sf].maior_carta.numero; 
        // vendo a maior carta de quem tem SF
        while(jogadores[++p_sf].classificacaoJogada == SF)
        {
            if(jogadores[p_sf].maior_carta.numero < maior)
                maior = jogadores[p_sf].maior_carta.numero; 
        }
        // vendo quantos jogadores estão empatados
        for(int i = 0; i < j; i++)
        {
            if(jogadores[i].classificacaoJogada == SF && jogadores[i].maior_carta.numero == maior)
            {
                vencedores[nv-1] = jogadores[i]; 
                nv++; 
            }    
        }
        nv--; 
    }
}

void achaMaiorCarta(jogador jogadores[])
{
    for(int i = 0; i < j; i++)
    {
        carta maior = jogadores[i].sequencia_cartas[0];
        for (int k = 1; k < 5; k++)
        {
            // a maior carta no poker é a de menor valor
            // ex: o ás é a maior carta
            if (jogadores[i].sequencia_cartas[k].numero < maior.numero)
                maior = jogadores[i].sequencia_cartas[k];
        }
        jogadores[i].maior_carta = maior; 
    }
    
}

void ordenaJogadorPorClassificacao(jogador jogadores[], int j)
{
    /*
    o jogador com a maior mão será o primeiro
    */
    for (int i = 1; i < j; i++)
    {
        jogador aux = jogadores[i];
        int j = i - 1;
        while (j >= 0 && jogadores[j].classificacaoJogada > aux.classificacaoJogada)
        {
            jogadores[j + 1] = jogadores[j];
            j--;
        }
        jogadores[j + 1] = aux;
    }
}

int processaSequenciaCartas(carta sequencia_cartas[])
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
    for (int i = 1; i < 5; i++)
    {
        if (sequencia_cartas[i].naipe != sequencia_cartas[i - 1].naipe)
        {
            rsf = false;
            break;
        }
    }
    if(sequencia_cartas[0].numero != 1) rsf = false; 
    if(sequencia_cartas[1].numero != 10) rsf = false; 
    if(sequencia_cartas[2].numero != 11) rsf = false; 
    if(sequencia_cartas[3].numero != 12) rsf = false; 
    if(sequencia_cartas[4].numero != 13) rsf = false; 
    if (rsf)
        return RSF;

    // SF
    for (int i = 1; i < 5; i++)
    {
        if (sequencia_cartas[i].naipe != sequencia_cartas[i - 1].naipe)
        {
            sf = false;
            break;
        }
    }
    // caso 1 2 11 12 13
    if (sequencia_cartas[0].numero == 1 && sequencia_cartas[1].numero == 2 && sequencia_cartas[2].numero == 11 && sequencia_cartas[3].numero == 12 && sequencia_cartas[4].numero == 13)
    {
        if (sf)
            return SF;
    }
    // caso 1 2 3 12 13
    if (sequencia_cartas[0].numero == 1 && sequencia_cartas[1].numero == 2 && sequencia_cartas[2].numero == 3 && sequencia_cartas[3].numero == 12 && sequencia_cartas[4].numero == 13)
    {
        if (sf)
            return SF;
    }

    // caso 1 2 3 4 13
    if (sequencia_cartas[0].numero == 1 && sequencia_cartas[1].numero == 2 && sequencia_cartas[2].numero == 3 && sequencia_cartas[3].numero == 4 && sequencia_cartas[4].numero == 13)
    {
        if (sf)
            return SF;
    }

    // caso geral
    for (int i = 1; i < 5; i++)
    {
        if ((sequencia_cartas[i - 1].numero + 1) != sequencia_cartas[i].numero)
        {
            sf = false;
            break;
        }
    }
    if (sf)
        return SF;

    // FK
    // caso dos 4 primeiros
    for (int i = 1; i < 5; i++)
    {
        int aux = sequencia_cartas[0].numero;
        if (aux != sequencia_cartas[i].numero)
        {
            fk = false;
            break;
        }
    }
    if (fk)
        return FK;
    fk = true;
    // caso dos 4 ultimos
    for (int i = 0; i < 4; i++)
    {
        int aux = sequencia_cartas[4].numero;
        if (aux != sequencia_cartas[i].numero)
        {
            fk = false;
            break;
        }
    }
    if (fk)
        return FK;

    // FH
    for (int i = 1; i < 3; i++)
    {
        if (sequencia_cartas[i - 1].numero != sequencia_cartas[i].numero)
            fh = false;
    }
    if (fh)
    {
        if (sequencia_cartas[3].numero == sequencia_cartas[4].numero)
            return FH;
    }
    else
    {
        for (int i = 3; i < 5; i++)
        {
            if (sequencia_cartas[i - 1].numero != sequencia_cartas[i].numero)
                fh = false;
        }
        if (fh)
        {
            if (sequencia_cartas[0].numero == sequencia_cartas[1].numero)
                return FH;
        }
    }

    // FLUSH
    for (int i = 1; i < 5; i++)
    {
        if (sequencia_cartas[i - 1].naipe != sequencia_cartas[i].naipe)
            f = false;
    }
    if (f)
        return F;

    // STRAIGHT
    for (int i = 1; i < 5; i++)
    {
        if ((sequencia_cartas[i - 1].numero + 1) != sequencia_cartas[i].numero)
            s = false;
    }
    if (s)
        return S;

    // THREE OF A KIND
    int seq_3 = 0;
    for (int i = 1; i < 5; i++)
    {
        if (sequencia_cartas[i - 1].numero == sequencia_cartas[i].numero)
            seq_3++;
        else
            seq_3 = 0;
    }
    if (seq_3 == 3)
        return TK;

    // TWO PAIRS
    int seq2 = 0;
    int num_seq2 = 0;
    for (int i = 1; i < 5; i++)
    {
        if (seq2 == 2)
        {
            num_seq2++;
            seq2 = 0;
        }
        if (sequencia_cartas[i - 1].numero != sequencia_cartas[i].numero)
        {
            seq2 = 0;
        }
        else
            seq2++;
    }

    // ONE PAIR
    seq2 = 0;
    for (int i = 1; i < 5; i++)
    {
        if (sequencia_cartas[i - 1].numero != sequencia_cartas[i].numero)
        {
            seq2 = 0;
        }
        else
            seq2++;
    }
    if (seq2 == 1)
        return OP;

    // HIGH CARD
    // não passou nos testes anteriores, então só pode ser high card
    return HC;
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