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


// g++ src/main.cpp src/baresmanager.cpp -std=c++11 -I include -o bares