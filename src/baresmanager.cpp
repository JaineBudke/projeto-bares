#include <iostream>
#include <string>
#include <fstream>

#include "bares-manager.h"


bool is_operator( char c ){

    std::string operators = "+-%^/*";
    return operators.find( c ) != std::string::npos; // se busca retornar indice do caractere dentro da string

}

bool is_operand( char c ){

    return ( c >= '0' and c <= '9' );

}

int  initialize( char * arq ){

    // cria input file stream (ifstream)
    std::ifstream arquivo;
    // abre arquivo passado por linha de comando
    arquivo.open( arq, std::ios::in );
    // verifica se houve algum erro ao abrir arquivo
    if (!arquivo.is_open()){
        return 0;
    }

    // processo de tokenizacao
    std::string operacao;
    bool operando = false;
    do {
        char ch;
        arquivo >> ch;
        if( is_operand(ch) ){
            operacao.push_back( ch );
            operando = true;
        } else {
            operacao.push_back(',');
            operacao.push_back( ch );
            operando = false;
        }

        // se o operando não for true, add uma virgula pra separar tokens
        if( operando == false ){
            operacao.push_back(',');
        }
    } while( arquivo.good() );

    std::cout << operacao << "\n";

    // fechando arquivo
    arquivo.close();
}