#include <iostream>
#include "bares-manager.h"

int main( int argc, char * argv[] ){

    // instanciar um manager
    BaresManager manager;

    // inicializar bares... Ler e guardar expressoes do arquivo de entrada
    manager.initialize( argv[1] );

    // Validar expressoes e tokenizar
    manager.validarExpress();

}


// g++ -std=c++11 src/main.cpp src/baresmanager.cpp src/parser.cpp -I include -o bares