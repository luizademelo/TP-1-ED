#ifndef POKERH 
#define POKERH 

#include <string>

typedef struct carta 
{
    char naipe; // pode ser C E O P 
    int numero; 
} carta; 

typedef struct jogador
{
    std::string nome; 
    int saldo; 
    int valor_aposta; 
    carta sequencia_cartas[5]; 
} jogador;

void leEntrada(); 
void processaRodada(jogador jogadores[], int j); 
std::string processaSequenciaCartas(carta sequencia_cartas[]); 
void ordenaMao(carta sequencia_cartas[]); 
void ordenaPorNaipe(carta sequencia_cartas[]); 
void ordenaPorNum(carta sequencia_cartas[], int ini, int fim); 
void escreveSaida(); 

#endif 