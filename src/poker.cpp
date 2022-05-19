#include "poker.hpp"
#include <sstream>
#include <fstream>

int n; // numero de rodadas
int di; // dinheiro inicial dos participantes  

void leEntrada()
{
    std::ifstream file("entrada.txt"); 
    std::string str;  
    getline(file, str); 
    std::istringstream istr(str); 
    istr >> n >> di; 

    for(int i = 0; i < n; i++) 
    {
        getline(file, str); 
        istr.str(str); 
        int j; // numero de jogadores 
        int p; // valor do pingo 
        istr >> j >> p; 
        for(int k = 0; k < j; k++) 
        {
            
        }
    }

    file.close(); 
}