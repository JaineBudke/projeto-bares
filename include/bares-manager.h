/**
 * @file    bares-manager.h
 * @brief   Arquivo cabeçalho com a implementacao de funcoes bases
            para execução do bares.
 * @author  Jaine Budke (jainebudke@hotmail.com)
 * @since   02/05/2017
 * @date    23/05/2017
 */

#ifndef _BARES_MANAGER_H_
#define _BARES_MANAGER_H_

#include <vector>   // std::vector
#include <string>   // std::string
#include <fstream>  // ifstream, ofstream
#include <iostream> // cin cout
#include <iomanip>  // setw setfill
#include <stack>    // pop push
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // runtime_error

#include "token.h"  // struct Token.
#include "parser.h" // classe Parser.


/**
 *  Essa eh a classe BaresManager
 *  Aqui sao definidas as funcoes bases para execução do bares.
 */
class BaresManager{

    public:

        /** @brief Inicializa lendo o arquivo de entrada fornecido pelo cliente.
            @param arq Nome do arquivo de entrada.
            @return 1 se o arquivo foi lido corretamente; 0 otherwise. */
        int  initialize( char * arq );

        /** @brief Valida expressões e separa em tokens.
            @return Vetor com os tokens de todas as expressões no formato infix. */
        std::vector< std::vector< Token > > validarExpress();

        /** @brief Transforma expressão do formado infix para postfix.
            @param inflix_ Tokens da expressão no formato infix
            @return Vetor com os tokens de todas as expressões no formato postfix. */
        std::vector< std::vector< Token > > infix_to_postfix( std::vector< Token > infix_ );

        /** @brief Realiza a operação.
            @param postfix Vetor com tokens da expressão no formato postfix
            @return Resultado da expressão. */
        int evaluate_postfix( std::vector< Token > postfix );

        /** @brief Apresenta resultado final das expressões.
            @param res Vetor com resultados das expressões válidas */
        void apresentarResult( std::vector< int > res );

    private:
        std::vector<std::string> expressions;          //<! expressoes a serem analisadas
        std::vector< std::vector< Token > > postfix;   //<! todas as expressao na forma posfixa

        /** @brief Apresenta mensagem final das expressões com erro de sintaxe.
            @param cont Contador
            @param arqsaida Arquivo de saída dos resultados
            @param result O resultado do Parser */
        void message( int cont, std::ofstream & arqsaida, const Parser::ParserResult & result );

};



#endif