#ifndef _BARES_MANAGER_H_
#define _BARES_MANAGER_H_

#include <vector>
#include <string>
#include <fstream>

#include "token.h"
#include "parser.h"

class BaresManager{

    public:
        int  initialize( char * arq );
        std::vector< std::vector< Token > > validarExpress();
        std::vector< std::vector< Token > > infix_to_postfix( std::vector< Token > infix_ );
        int evaluate_postfix( std::vector< Token > postfix );
        void apresentarResult( std::vector< int > res );
		void message( int cont, std::ofstream & arqsaida, const Parser::ParserResult & result, std::string str );

    private:
        std::vector<std::string> expressions;          //<! expressoes a serem analisadas
        std::vector< std::vector< Token > > postfix;   //<! todas as expressao na forma posfixa

};



#endif