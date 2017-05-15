#ifndef _BARES_MANAGER_H_
#define _BARES_MANAGER_H_

#include <vector>

class BaresManager{

    public:
        int  initialize( char * arq );
        void validarExpress();
    private:
        std::vector<std::string> expressions;

};



#endif