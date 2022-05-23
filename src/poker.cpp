#include "..\include\poker.hpp"
#include <sstream>
#include <fstream>
#include <iostream> 


void leEntrada()
{
    std::ifstream file("entrada.txt"); 
    std::string str;  
    getline(file, str); 
    std::istringstream istr(str); 
    istr >> n >> di; 
    std::cout << n << " " << di << std::endl; 
    // for(int i = 0; i < n; i++) 
    // {
    //     jogador jogadores[1000]; 
    //     getline(file, str); 
    //     istr.str(str); 
    //     int j; // numero de jogadores 
    //     int p; // valor do pingo 
    //     istr >> j >> p; 
    //     for(int k = 0; k < j; k++) 
    //     {
    //         jogador kEsimo; 
    //         getline(file, str); 
    //         istr.str(str); 
    //         istr >> kEsimo.nome >> kEsimo.valor_aposta; 
    //         for(int l = 0; l < 5; l++)
    //         {
    //             std::string carta;  
    //             istr >> carta;
    //             kEsimo.sequencia_cartas[l].numero = (int)carta[0]; 
    //             kEsimo.sequencia_cartas[l].naipe = carta[1];   
    //         }
    //         jogadores[k] = kEsimo; 
    //     }

        
    // }

    file.close(); 
}