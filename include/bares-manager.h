#ifndef _BARES_MANAGER_H_
#define _BARES_MANAGER_H_

#include <vector>
#include <string>

#include "token.h"

class BaresManager{

    public:
        int  initialize( char * arq );
        void validarExpress();
        std::string infix_to_postfix( std::vector< Token > infix_ );

    private:
        std::vector<std::string> expressions;   //<! expressoes a serem analisadas
        std::string postfix; 					//<! expressao na forma posfixa

};



#endif