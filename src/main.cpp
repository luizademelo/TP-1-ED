#include "..\include\poker.hpp"
#include <iostream> 
#include <fstream>

int main()
{
    // limpando o arquivo de saida
    std::ofstream file("saida.txt");
    file.close(); 

    leEntrada(); 
    
    return 0; 
}