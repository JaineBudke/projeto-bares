#include <iostream>
#include "bares-manager.h"
#include "token.h"
#include <vector>
#include <iterator>


int main( int argc, char * argv[] ){

    // instanciar um manager
    BaresManager manager;

    // inicializar bares... Ler e guardar expressoes do arquivo de entrada
    manager.initialize( argv[1] );

    // Validar expressoes e tokenizar
    std::vector< std::vector< Token > > tokens = manager.validarExpress();

    // passar expressao do formato infixo para posfixo
    for( auto tk : tokens ){
        auto postfix = manager.infix_to_postfix( tk );
        for( auto pos : postfix ){
             std::cout << ">>> Tokens: { ";
             std::copy( pos.begin(), pos.end(),
                std::ostream_iterator< Token >( std::cout, " ") );
            std::cout << "}\n";
        }
    }

}


// g++ -std=c++11 src/main.cpp src/baresmanager.cpp src/parser.cpp -I include -o bares