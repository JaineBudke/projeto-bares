#include <iostream>
#include "bares-manager.h"
#include "token.h"
#include <vector>


int main( int argc, char * argv[] ){

    // instanciar um manager
    BaresManager manager;

    // inicializar bares... Ler e guardar expressoes do arquivo de entrada
    manager.initialize( argv[1] );

    // Validar expressoes e tokenizar
    std::vector< Token > tokens = manager.validarExpress();

    // passar expressao do formato infixo para posfixo
    manager.infix_to_postfix( tokens );

}


// g++ -std=c++11 src/main.cpp src/baresmanager.cpp src/parser.cpp -I include -o bares