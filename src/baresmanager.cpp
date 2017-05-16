#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stack>

#include "bares-manager.h"
#include "parser.h"


////////////////////////////////////////////////////////////////////////////
// Funcoes auxiliares
////////////////////////////////////////////////////////////////////////////

void print_msg( const Parser::ParserResult & result, std::string str ){
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch ( result.type )
    {
        case Parser::ParserResult::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::ILL_FORMED_INTEGER:
            std::cout << ">>> Ill formed integer at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_CLOSING_PARENTHESIS:
            std::cout << ">>> Missing closing \")\" at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::INTEGER_OUT_OF_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col << ")!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
}

bool is_operand( const Token & t )
{
    return t.type == Token::token_t::OPERAND;
}

bool is_operator( const Token & t )
{
    return t.type == Token::token_t::OPERATOR;
}

/// Determines qhether the first operator is > than the second operator.
bool has_higher_precedence( char op1, char op2 )
{
/*    auto p1 = get_precedence( op1 );
    auto p2 = get_precedence( op2 );

    // special case: has the same precedence and is right association.
    if ( p1 == p2 and is_right_association( op1 ) )
    {
        return false;
    }

    return p1 >= p2 ;
*/}


////////////////////////////////////////////////////////////////////////////
// Funcoes principais
////////////////////////////////////////////////////////////////////////////

int  BaresManager::initialize( char * arq ){

    // cria input file stream (ifstream)
    std::ifstream arquivo;
    // abre arquivo passado por linha de comando
    arquivo.open( arq, std::ios::in );
    // verifica se houve algum erro ao abrir arquivo
    if (!arquivo.is_open()){
        return 0;
    }

    do {
        std::string operacao;
        getline(arquivo, operacao);
        expressions.push_back( operacao );
    } while( arquivo.good() );

    /* TESTE DE LEITURA CORRETA DO ARQUIVO
    for (auto i = std::begin(expressions); i != std::end(expressions); ++i){
        std::cout << *i << "\n";
    }*/


    // fechando arquivo
    arquivo.close();
}


std::vector< Token > BaresManager::validarExpress(){

    Parser my_parser; // Instancia um parser.

    std::vector< Token > lista;

    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions ){

        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );

        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";

        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ParserResult::PARSER_OK )
            print_msg( result, expr );
        else
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

        // Recuperar a lista de tokens.
        lista = my_parser.get_tokens();

        std::cout << ">>> Tokens: { ";
        std::copy( lista.begin(), lista.end(),
            std::ostream_iterator< Token >(std::cout, " ") );
        std::cout << "}\n";

    }

    return lista;

}


std::string BaresManager::infix_to_postfix( std::vector< Token > infix_ ){

    // Stack para ajudar a converter a expressao.
    std::stack< char > s;

    // Percorre expressao infixa
    for( auto tk : infix_ ){

        // Operando vai direto para fila de saída
        if ( is_operand( tk ) ) // 1 23 100, etc.
        {
            postfix += tk.value;
        }
    }

    // Tirar todos os operadores restantes na pilha
    while( not s.empty() )
    {
        postfix += s.top();
        s.pop();
    }

    return postfix;

}
