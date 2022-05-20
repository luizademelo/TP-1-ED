#include <string>


typedef struct carta 
{
    char naipe; // pode ser O P E C  
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
void escreveSaida(); 