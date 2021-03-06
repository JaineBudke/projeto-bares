/**
 * @file    parser.cpp
 * @brief   Código fonte com a implementacao de funcoes que
            realizam o parsing e tokenização da expressão.
 * @author  Jaine Budke (jainebudke@hotmail.com)
 * @since   02/05/2017
 * @date    23/05/2017
 */


#include "../include/parser.h"



/** @brief Converte de caractere para código do símbolo terminal.
            @param ch Caractere.
            @return Código do simbolo terminal. */
Parser::terminal_symbol_t  Parser::lexer( char c_ ) const
{
    switch( c_ )
    {
        case '+':  return terminal_symbol_t::TS_PLUS;
        case '-':  return terminal_symbol_t::TS_MINUS;
        case '*':  return terminal_symbol_t::TS_MULTIPLICATION;
        case '/':  return terminal_symbol_t::TS_DIVISION;
        case '%':  return terminal_symbol_t::TS_REST;
        case '^':  return terminal_symbol_t::TS_POTENTIATION;
        case ' ':  return terminal_symbol_t::TS_WS;
        case   9:  return terminal_symbol_t::TS_TAB;
        case '0':  return terminal_symbol_t::TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':  return terminal_symbol_t::TS_NON_ZERO_DIGIT;
        case '\0': return terminal_symbol_t::TS_EOS; // end of string: the $ terminal symbol
    }
    return terminal_symbol_t::TS_INVALID;
}


/** @brief Converte um simbolo terminal para a string correspondente.
    @param ch Caractere.
    @return Código do simbolo terminal. */
std::string Parser::token_str( terminal_symbol_t s_ ) const
{
    switch( s_ )
    {
        case terminal_symbol_t::TS_PLUS             : return "+";
        case terminal_symbol_t::TS_MINUS            : return "-";
        case terminal_symbol_t::TS_MULTIPLICATION   : return "*";
        case terminal_symbol_t::TS_DIVISION         : return "/";
        case terminal_symbol_t::TS_REST             : return "%";
        case terminal_symbol_t::TS_POTENTIATION     : return "^";
        case terminal_symbol_t::TS_WS               : return " ";
        case terminal_symbol_t::TS_ZERO             : return "0";
        default                                     : return "X";
    }
}

/** @brief Iterador para avançar para próximo char na expressão. */
void Parser::next_symbol( void )
{
    // Get a valid symbol for processing
    std::advance( it_curr_symb, 1 );
}

/** @brief Verifica se o símbolo atual é igual ao símbolo do terminal solicitado.
    @param s_ Simbolo do terminal.
    @return 1 se for igual 0 otherwise. */
bool Parser::peek( terminal_symbol_t c_ ) const
{
    // Verificar se o código fornecido no argumento corresponde
    // ao caractere na "ponta da agulha" (apontado pelo iterador).
    return ( not end_input() and
             lexer( *it_curr_symb ) == c_ );
}

/** @brief Tenta aceitar o símbolo solicitado.
    @param s_ Simbolo do terminal.
    @return 1 se aceitar 0 otherwise. */
bool Parser::accept( terminal_symbol_t c_ )
{
    // Se o caractere atual for o requisitado, o método consome o
    // caractere da entrada.
    if ( peek( c_ ) )
    {
        next_symbol();
        return true;
    }

    return false;
}

/** @brief Ignora qualquer WS/Tab e tenta aceitar o símbolo solicitado.
    @param s_ Simbolo do terminal.
    @return 1 se aceitar 0 otherwise. */
bool Parser::expect( terminal_symbol_t c_ )
{
    // Salte todos os caracteres em branco e tente validar
    // o primeiro caractere que surgir.
    skip_ws();
    return accept( c_ );
}


/** @brief Ignora qualquer WS/Tab e para no próximo caractere. */
void Parser::skip_ws( void )
{
    // Simplemente salta todos os caracteres em branco.
    // Lembrar de verificar se a entrada não acabou, para evitar de
    // acessar um iterador inválido.
    while ( not end_input() and
            ( lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_WS  or
              lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_TAB ) )
    {
        next_symbol();
    }
}

/** @brief Verifica se chegamos ao final da sequência de expressão.
    @return 1 se chegou 0 otherwise. */
bool Parser::end_input( void ) const
{
    // "Fim de entrada" ocorre quando o iterador chega ao
    // fim da string que guarda a expressão.
    return it_curr_symb == expr.end();
}

/** @brief Converte um string em um integer.
    @param ch Caractere .
    @return Caractere no tipo inteiro. */ 
Parser::input_int_type str_to_int( std::string input_str_ )
{
    // Creating input stream.
    std::istringstream iss( input_str_ );

    // Resulting value.
    Parser::input_int_type value;
    iss >> value; // Ignore trailling white space.

    // Check for error during convertion.
    if ( iss.fail() )
        //throw std::runtime_error( "str_to_int(): Erro, illegal integer format." );

    return value;
}

//=== NTS methods.

/*! Processando uma expressão.
 *
 * Produção:
 * <expr> := <term>,{ ("+"|"-"),<term> }
 *
 * De acordo com a gramática (acima), uma expressão pode ser apenas um
 * termo isolado ou seguido de um ou mais termos com um + ou - entre eles.
 *
 */

/** @brief Processa uma expressão.
    @return Resultado do processamento. */ 
Parser::ParserResult Parser::expression()
{
    skip_ws(); // Salta todos os espaços em branco.

    // (1) Vamos validar um termo.
    auto result = term();

    // Usamos um 'enquanto' pois podem vir 1 ou mais termos ligados por +/-.
    while ( result.type == ParserResult::PARSER_OK and not end_input() )
    {
        // ============================================================
        // Se chegamos aqui significa que um termo **válido** foi
        // consumido da entrada (expressão), ou através do método
        // term() fora do laço (na 1ª vez) ou através do método
        // term() invocado no final do laço (para as demais vezes).
        // Portanto, devemos verificar se existem novos termos
        // na entrada (expressão), precedidos por +/-.
        // ============================================================

        // (2) Pode vir um '+', ou seja, "esperamos" um '+'...
        if ( expect( terminal_symbol_t::TS_PLUS ) )
        {
            // Ok, recebemos:
            token_list.push_back( Token("+", Token::token_t::OPERATOR) );
        }
        // (3) ... mas pode vir um '-', ou seja, também "esperamos" um '-'.
        else if ( expect( terminal_symbol_t::TS_MINUS ) ) // ou um '-'
        {
            // Ok, recebemos:
            token_list.push_back( Token("-", Token::token_t::OPERATOR) );
        } else if ( expect( terminal_symbol_t::TS_MULTIPLICATION ) ) // ou um '*'
        {
            // Ok, recebemos:
            token_list.push_back( Token("*", Token::token_t::OPERATOR) );
        } else if ( expect( terminal_symbol_t::TS_DIVISION ) ) // ou um '/'
        {
            // Ok, recebemos:
            token_list.push_back( Token("/", Token::token_t::OPERATOR) );
        } else if ( expect( terminal_symbol_t::TS_REST ) ) // ou um '%'
        {
            // Ok, recebemos:
            token_list.push_back( Token("%", Token::token_t::OPERATOR) );
        } else if ( expect( terminal_symbol_t::TS_POTENTIATION ) ) // ou um '^'
        {
            // Ok, recebemos:
            token_list.push_back( Token("^", Token::token_t::OPERATOR) );
        } else // ... mas se vier outra coisa, é um erro de sintaxe!
        {
            return result;
        }

        // (4) Se chegamos aqui é porque recebemos com sucesso um
        // operador. Então agora TEM QUE VIR UM TERMO!.
        // Se não vier um termo, então temos um erro de sintaxe.

        result = term(); // consumir um termo da entrada (expressão).
        if ( result.type != ParserResult::PARSER_OK ) // deu pau, não veio um termo.
        {
            // Se o termo não foi encontrado, atualizamos a mensagem
            // de erro (ParserResult) recebida com um tipo mais
            // explicativo para o cliente.
            result.type = ParserResult::MISSING_TERM;
            return result;
        }



    }

    return result;
}

/** @brief Processa um termo.
    @return Resultado do processamento. */ 
Parser::ParserResult Parser::term(){
    skip_ws();

    auto begin = it_curr_symb;
    auto result =  integer();

    std::string val;
    val.insert( val.begin(), begin, it_curr_symb );

    if( not val.empty() ){ // se não for vazio
        token_list.push_back( Token( val, Token::token_t::OPERAND) ); // add token
    }

    return result;

}

/** @brief Processa um inteiro.
    @return Resultado do processamento. */ 
Parser::ParserResult Parser::integer()
{
    // Será que é um zero?
    if( lexer( *it_curr_symb ) == terminal_symbol_t::TS_ZERO ){
        return ParserResult( ParserResult::PARSER_OK );
    }

    // Tratar o '-' unário
    while( accept( terminal_symbol_t::TS_MINUS ) ){ }

    return natural_number();

}

/** @brief Processa um número natural.
    @return Resultado do processamento. */ 
Parser::ParserResult Parser::natural_number()
{
    if( digit_excl_zero() ){
        while( digit() ); /* Empty */

        return ParserResult( ParserResult::PARSER_OK );
    }

    return ParserResult( ParserResult::ILL_FORMED_INTEGER, std::distance( expr.begin(), it_curr_symb ) );
}

/** @brief Verifica se número é 1...9.
    @return 1 se for 0 otherwise. */
bool Parser::digit_excl_zero()
{
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

/** @brief Verifica se número é 0...9.
    @return 1 se for 0 otherwise. */
bool Parser::digit()
{
    return (accept( terminal_symbol_t::TS_ZERO ) or
            accept( terminal_symbol_t::TS_NON_ZERO_DIGIT ));
}


/** @brief Realiza o parsing
    @param e_ Expressão.
    @return Resultado. */
Parser::ParserResult
Parser::parse( std::string e_ )
{
    // We reset the parsing process each new expression.
    expr = e_;  // The expression in a string.
    it_curr_symb = expr.begin(); // Iterator to the 1st character in the expression.
    token_list.clear(); // Clear the list of tokens.

    // Default result.
    ParserResult result( ParserResult::PARSER_OK );

    skip_ws();
    if ( end_input() )
    {
        return ParserResult( ParserResult::UNEXPECTED_END_OF_EXPRESSION,
                std::distance( expr.begin(), it_curr_symb ) );
    }

    // Tentar validar a expressao...
    result = expression();

    // incluir verificação de "lixo" no final da string.
    // Gerar erro: ParserResult::EXTRANEOUS_SYMBOL
    if( result.type == ParserResult::PARSER_OK ){
        skip_ws();
        if( not end_input() ){
            return ParserResult( ParserResult::EXTRANEOUS_SYMBOL, std::distance( expr.begin(), it_curr_symb ) );
        }
    }

    return result;
}


/** @brief Recupera tokens.
    @return Lista de tokens. */
std::vector< Token > Parser::get_tokens( void ) const {
    return token_list;
}



//==========================[ End of parse.cpp ]==========================//