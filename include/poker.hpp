#include <string>

typedef struct jogador
{
    std::string nome; 
    int saldo; 
} jogador;

typedef struct carta 
{
    char naipe; // pode ser O P E C  
    int numero; 
} carta; 

void leEntrada(); 
void escreveSaida(); 