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
int j_total; 
int p; // valor do pingo
bool rodada_valida; 

jogador vencedores[12]; 
jogador jogadores_total[12]; 
void leEntrada()
{
    FILE *arq = fopen("entrada.txt", "r");
    std::ofstream file("saida.txt", std::ios::app);
    fscanf(arq, "%d %d", &n, &di);
    for (int i = 0; i < n; i++)
    {
        pote = 0;
        nv = 1;
        rodada_valida = true; 
        jogador jogadores[12];
        fscanf(arq, "%d %d\n", &j, &p);
        if(i == 0) j_total = j; 
        if(p < 50)
            rodada_valida = false;
        if(rodada_valida) pote += (p * j_total); 
        for (int k = 0; k < j; k++)
        {
            jogador kEsimo;
            char nome[100];
            fscanf(arq, "%[^0-9] %d ", nome, &kEsimo.valor_aposta);
            kEsimo.nome = nome;
            if(rodada_valida)
            {
                pote += kEsimo.valor_aposta;
                if(i == 0) 
                    kEsimo.saldo = di - kEsimo.valor_aposta ;
                else
                {
                    int indice = achaIndiceJogador(kEsimo.nome); 
                    jogadores_total[indice].saldo -= kEsimo.valor_aposta;
                }
            }
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
            if(i == 0)
                jogadores_total[k] = kEsimo; 
            jogadores[k] = kEsimo;
        }
        if(rodada_valida)
        {
            for(int x = 0; x < j_total; x++)
                jogadores_total[x].saldo -= p; 
        }
        processaRodada(jogadores, j);
    }
    ordenaJogadorPorMontante(jogadores_total); 
    file << "####\n";
    for(int i = 0; i < j_total; i++)
        file << jogadores_total[i].nome << " " << jogadores_total[i].saldo << '\n'; 
    file.close(); 
    fclose(arq);
}

void processaRodada(jogador jogadores[], int j)
{
    for (int i = 0; i < j; i++)
        ordenaMao(jogadores[i].sequencia_cartas);
    for (int i = 0; i < j; i++)
    {
        jogadores[i].classificacaoJogada = processaSequenciaCartas(jogadores[i].sequencia_cartas);
    }

    ordenaJogadorPorClassificacao(jogadores, j);
    vencedores[0] = jogadores[0];
    achaMaiorCarta(jogadores);
    trataEmpateJogada(jogadores);
    escreveSaida(jogadores);
}

void escreveSaida(jogador jogadores[])
{
    std::ofstream file("saida.txt", std::ios::app);
    if(rodada_valida)
    {
        file << nv << " " << pote / nv << " " << jogadaParaString(jogadores[0].classificacaoJogada) << "\n";
        for (int i = 0; i < nv; i++)
        {
            file << vencedores[i].nome << '\n';
            vencedores[i].saldo += pote / nv;
            int indice = achaIndiceJogador(vencedores[i].nome); 
            jogadores_total[indice].saldo += pote/nv; 
        }
    }else  
        file << "0 0 I\n"; 
    file.close();
}

void ordenaJogadorPorMontante(jogador jogadores[])
{
    for (int i = 0; i < 5; i++)
    {
        int i_menor = i;
        for (int j = i + 1; j < 5; j++)
        {
            if (jogadores[j].saldo < jogadores[i_menor].saldo)
                i_menor = j;
        }
        jogador aux = jogadores[i];
        jogadores[i] = jogadores[i_menor];
        jogadores[i_menor] = aux;
    }
}

int achaIndiceJogador(std::string nome)
{
    for(int i = 0; i < j_total; i++)
    {
        if(nome == jogadores_total[i].nome)
            return i; 
    }
    return -1; 
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
    if(jogadores[0].classificacaoJogada != jogadores[1].classificacaoJogada) 
        return; 
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
            if (sf == 1)
                p_sf = i;
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
        for (int i = 0; i < rsf; i++)
        {
            vencedores[i] = jogadores[i];
            nv++;
        }
        nv--;

    }
    else if (sf > 1)
    {
        int maior = jogadores[p_sf].maior_carta.numero;
        // vendo a maior carta de quem tem SF
        while (jogadores[++p_sf].classificacaoJogada == SF)
        {
            if (jogadores[p_sf].maior_carta.numero < maior)
                maior = jogadores[p_sf].maior_carta.numero;
        }
        // vendo quantos jogadores estão empatados
        for (int i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == SF && jogadores[i].maior_carta.numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
        nv--;
    }

    else if (fk > 1)
    {
        int maior, i = 0;
        maior = achaCartaFK(jogadores[i]).numero;
        // vendo a maior  carta de quem tem FK
        while (jogadores[++i].classificacaoJogada == FK)
        {
            if (achaCartaFK(jogadores[i]).numero < maior)
                maior = achaCartaFK(jogadores[i]).numero;
        }
        // vendo quantos jogadores estão empatados
        for (int i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == FK && achaCartaFK(jogadores[i]).numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
    }

    else if (fh > 1)
    {
        int i = 0;
        int maior = achaTriplaFH(jogadores[i]).numero;
        // vendo a maior carta de quem tem fh
        while (jogadores[++i].classificacaoJogada == FH)
        {
            if (achaTriplaFH(jogadores[i]).numero < maior)
                maior = achaTriplaFH(jogadores[i]).numero;
        }
        for (int i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == FH && achaTriplaFH(jogadores[i]).numero == maior)
            {
                vencedores[0] = jogadores[i];
                break;
            }
        }
    }

    else if (f > 1)
    {
        int i = 0;
        int maior = jogadores[0].maior_carta.numero;
        // vendo a maior carta de quem tem FK
        while (jogadores[++i].classificacaoJogada == F)
        {
            if (jogadores[i].maior_carta.numero < maior)
                maior = jogadores[i].maior_carta.numero;
        }
        // vendo quantos jogadores estão empatados
        for (i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == F && jogadores[i].maior_carta.numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
        nv--;
    }
    else if (s > 1)
    {
        int i = 0;
        int maior = jogadores[0].maior_carta.numero;
        // vendo a maior carta de quem tem FK
        while (jogadores[++i].classificacaoJogada == S)
        {
            if (jogadores[i].maior_carta.numero < maior)
                maior = jogadores[i].maior_carta.numero;
        }
        // vendo quantos jogadores estão empatados
        for (i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == S && jogadores[i].maior_carta.numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
        nv--;
    }

    else if (tk > 1)
    {
        int i = 0;
        int maior = jogadores[0].sequencia_cartas[2].numero;
        // vendo a maior carta de quem tem FK
        while (jogadores[++i].classificacaoJogada == TK)
        {
            if (jogadores[i].sequencia_cartas[2].numero < maior)
                maior = jogadores[i].sequencia_cartas[2].numero;
        }
        // vendo quantos jogadores estão empatados
        for (i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == TK && jogadores[i].sequencia_cartas[2].numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
        nv--;
    }

    else if (tp > 1)
    {
        int i = 0;
        int maior = achaMaiorParTP(jogadores[0]).numero;
        // vendo a maior carta de quem tem FK
        while (jogadores[++i].classificacaoJogada == TP)
        {
            if (achaMaiorParTP(jogadores[i]).numero < maior)
                maior = achaMaiorParTP(jogadores[i]).numero;
        }
        // vendo quantos jogadores estão empatados
        for (i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == TP && achaMaiorParTP(jogadores[i]).numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
        nv--;
    }

    else if (op > 1)
    {
        int i = 0;
        int maior = achaParOP(jogadores[0]).numero;
        // vendo a maior carta de quem tem FK
        while (jogadores[++i].classificacaoJogada == OP)
        {
            if (achaParOP(jogadores[i]).numero < maior)
                maior = achaParOP(jogadores[i]).numero;
        }
        // vendo quantos jogadores estão empatados
        for (i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == OP && achaParOP(jogadores[i]).numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
        nv--;
    }

    else if (hc > 1)
    {
        int i = 0;
        int maior = jogadores[0].maior_carta.numero;
        // vendo a maior carta de quem tem FK
        while (jogadores[++i].classificacaoJogada == HC)
        {
            if (jogadores[i].maior_carta.numero < maior)
                maior = jogadores[i].maior_carta.numero;
        }
        // vendo quantos jogadores estão empatados
        for (i = 0; i < j; i++)
        {
            if (jogadores[i].classificacaoJogada == HC && jogadores[i].maior_carta.numero == maior)
            {
                vencedores[nv - 1] = jogadores[i];
                nv++;
            }
        }
        nv--;
    }
}

carta achaParOP(jogador jog)
{
    // caso 1 1 X X X
    if (jog.sequencia_cartas[0].numero == jog.sequencia_cartas[1].numero)
        return jog.sequencia_cartas[0];

    // caso X 1 1 X X
    if (jog.sequencia_cartas[1].numero == jog.sequencia_cartas[2].numero)
        return jog.sequencia_cartas[1];

    // caso X X 1 1 X
    if (jog.sequencia_cartas[2].numero == jog.sequencia_cartas[3].numero)
        return jog.sequencia_cartas[2];
    // caso X X X 1 1
    if (jog.sequencia_cartas[3].numero == jog.sequencia_cartas[4].numero)
        return jog.sequencia_cartas[3];
}

carta achaTriplaFH(jogador jog)
{
    carta tripla;
    if (jog.sequencia_cartas[1].numero == jog.sequencia_cartas[2].numero)
        tripla = jog.sequencia_cartas[1];
    else
        tripla = jog.sequencia_cartas[3];
    return tripla;
}

carta achaMaiorParTP(jogador jog)
{
    // caso 0 0 1 1 X
    bool tp = true;
    if (jog.sequencia_cartas[0].numero != jog.sequencia_cartas[1].numero)
        tp = false;
    if (jog.sequencia_cartas[2].numero != jog.sequencia_cartas[3].numero)
        tp = false;
    if (tp)
    {
        if (jog.sequencia_cartas[1].numero < jog.sequencia_cartas[2].numero)
            return jog.sequencia_cartas[1];
        else
            return jog.sequencia_cartas[2];
    }

    tp = true;
    // caso X 0 0 1 1
    if (jog.sequencia_cartas[1].numero != jog.sequencia_cartas[2].numero)
        tp = false;
    if (jog.sequencia_cartas[3].numero != jog.sequencia_cartas[4].numero)
        tp = false;
    if (tp)
    {
        if (jog.sequencia_cartas[2].numero < jog.sequencia_cartas[3].numero)
            return jog.sequencia_cartas[2];
        else
            return jog.sequencia_cartas[3];
    }

    tp = true;
    // caso 0 0 X 1 1
    if (jog.sequencia_cartas[0].numero != jog.sequencia_cartas[1].numero)
        tp = false;
    if (jog.sequencia_cartas[3].numero != jog.sequencia_cartas[4].numero)
        tp = false;
    if (tp)
    {
        if (jog.sequencia_cartas[0].numero < jog.sequencia_cartas[3].numero)
            return jog.sequencia_cartas[0];
        else
            return jog.sequencia_cartas[3];
    }
}

carta achaCartaFK(jogador jog)
{
    carta maior;
    if (jog.sequencia_cartas[0].numero == jog.sequencia_cartas[1].numero)
        maior = jog.maior_carta;
    else
        maior = jog.sequencia_cartas[2];
    return maior;
}

void achaMaiorCarta(jogador jogadores[])
{
    for (int i = 0; i < j; i++)
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
    if (sequencia_cartas[0].numero != 1)
        rsf = false;
    if (sequencia_cartas[1].numero != 10)
        rsf = false;
    if (sequencia_cartas[2].numero != 11)
        rsf = false;
    if (sequencia_cartas[3].numero != 12)
        rsf = false;
    if (sequencia_cartas[4].numero != 13)
        rsf = false;
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
    ordenaPorNum(sequencia_cartas, 0, 4);
    // caso dos 4 primeiros
    for (int i = 1; i < 4; i++)
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
    for (int i = 1; i < 4; i++)
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
        fh = true;
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
    // caso 1 1 1 X X
    for (int i = 1; i < 3; i++)
    {
        if (sequencia_cartas[i - 1].numero != sequencia_cartas[i].numero)
            tk = false;
    }
    if (tk)
        return TK;
    tk = true;
    // caso X 1 1 1 X
    for (int i = 2; i < 4; i++)
    {
        if (sequencia_cartas[i - 1].numero != sequencia_cartas[i].numero)
            tk = false;
    }
    if (tk)
        return true;

    // caso X X 1 1 1
    tk = true;
    for (int i = 3; i < 5; i++)
    {
        if (sequencia_cartas[i - 1].numero != sequencia_cartas[i].numero)
            tk = false;
    }
    if (tk)
        return true;

    // TWO PAIRS
    // caso 0 0 1 1 X
    if (sequencia_cartas[0].numero != sequencia_cartas[1].numero)
        tp = false;
    if (sequencia_cartas[2].numero != sequencia_cartas[3].numero)
        tp = false;
    if (tp)
        return TP;
    tp = true;

    // caso X 0 0 1 1
    if (sequencia_cartas[1].numero != sequencia_cartas[2].numero)
        tp = false;
    if (sequencia_cartas[3].numero != sequencia_cartas[4].numero)
        tp = false;
    if (tp)
        return TP;

    tp = true;
    // caso 0 0 X 1 1
    if (sequencia_cartas[0].numero != sequencia_cartas[1].numero)
        tp = false;
    if (sequencia_cartas[3].numero != sequencia_cartas[4].numero)
        tp = false;
    if (tp)
        return TP;

    // ONE PAIR
    // caso 1 1 X X X
    if (sequencia_cartas[0].numero == sequencia_cartas[1].numero)
        return OP;

    // caso X 1 1 X X
    if (sequencia_cartas[1].numero == sequencia_cartas[2].numero)
        return OP;

    // caso X X 1 1 X
    if (sequencia_cartas[2].numero == sequencia_cartas[3].numero)
        return OP;
    // caso X X X 1 1
    if (sequencia_cartas[3].numero == sequencia_cartas[4].numero)
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