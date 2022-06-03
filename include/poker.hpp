#ifndef POKERH 
#define POKERH 

#define RSF 1 
#define SF 2 
#define FK 3
#define FH 4 
#define F 5
#define S 6 
#define TK 7
#define TP 8 
#define OP 9 
#define HC 10 


#include <string>

typedef struct carta 
{
    char naipe; // pode ser C E O P 
    int numero; 
} carta; 

typedef struct jogador
{
    std::string nome; 
    int classificacaoJogada; 
    int saldo; 
    int valor_aposta; 
    carta sequencia_cartas[5]; 
    carta maior_carta; 
} jogador;

void leEntrada(); 
void processaRodada(jogador jogadores[], int j); 
int processaSequenciaCartas(carta sequencia_cartas[]); 
void ordenaJogadorPorClassificacao(jogador jogadores[], int j); 
void ordenaMao(carta sequencia_cartas[]); 
void ordenaPorNaipe(carta sequencia_cartas[]); 
void ordenaPorNum(carta sequencia_cartas[], int ini, int fim); 
void escreveSaida(jogador jogadores[]); 
std::string jogadaParaString(int classificacaoJogada); 
void trataEmpateJogada(jogador jogadores[]);
void achaMaiorCarta(jogador jogadores[]); 

#endif 