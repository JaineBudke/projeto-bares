#ifndef _BARES_MANAGER_H_
#define _BARES_MANAGER_H_

#include <vector>
#include <string>

#include "token.h"

class BaresManager{

    public:
        int  initialize( char * arq );
        std::vector< std::vector< Token > > validarExpress();
        std::vector< std::vector< Token > > infix_to_postfix( std::vector< Token > infix_ );

    private:
        std::vector<std::string> expressions;               //<! expressoes a serem analisadas
        std::vector< std::vector< Token > > postfix;   //<! todas as expressao na forma posfixa

};



#endif