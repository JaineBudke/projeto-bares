/**
 * @file    main.cpp
 * @brief   Código fonte do programa
 * @author  Jaine Budke (jainebudke@hotmail.com)
 * @since   02/05/2017
 * @date    23/05/2017
 */

#include <iostream>
#include <vector>
#include <iterator>

#include "bares-manager.h"
#include "token.h"

/**
*@brief Função principal que contém toda a execução do programa.
*/
int main( int argc, char * argv[] ){

    std::vector< std::vector< Token > > postfix;
    int result;
    std::vector< int > resultados;

    // instanciar um manager
    BaresManager manager;

    // inicializar bares... Ler e guardar expressoes do arquivo de entrada
    manager.initialize( argv[1] );

    // Validar expressoes e tokenizar
    std::vector< std::vector< Token > > tokens = manager.validarExpress();

    // passar expressao do formato infixo para posfixo
    for( auto tk : tokens ){
        postfix = manager.infix_to_postfix( tk );
    }

    // Imprimir para usários os tokens válidos e já no formato posfixo
    std::cout << "\nTokens válidos e no formato postfix:\n";
    for( auto pos : postfix ){
        std::cout << ">>> Tokens: { ";
        std::copy( pos.begin(), pos.end(),
        std::ostream_iterator< Token >( std::cout, " ") );
        std::cout << "}\n";
    }
    std::cout << "\n";


    // encontrar resultado da expressao
    for( auto pf : postfix ){
        result = manager.evaluate_postfix( pf );
        resultados.push_back( result );
    }

    manager.apresentarResult( resultados );

    return 0;
}