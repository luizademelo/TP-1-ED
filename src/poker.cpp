#include "poker.hpp"
#include <sstream>
#include <fstream>

int n, di; 

void leEntrada()
{
    std::ifstream file("entrada.txt"); 
    std::string str;  
    getline(file, str); 
    std::istringstream istr(str); 
    istr >> n >> di; 


    file.close(); 
}