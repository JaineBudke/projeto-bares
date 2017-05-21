/**
 * @file    parser.h
 * @brief   Arquivo cabeçalho com a implementacao de funcoes que
            realizam o parsing
 * @author  Jaine Budke (jainebudke@hotmail.com)
 * @since   02/05/2017
 * @date    23/05/2017
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream> // cout, cin
#include <iterator> // std::distance()
#include <vector>   // std::vector
#include <sstream>  // std::istringstream

#include "token.h"  // struct Token.

/*!
 * Implements a recursive descendent parser for a EBNF grammar.
 *
 *   <expr>            := <term>,{ ("+"|"-"),<term> };
 *   <term>            := <integer>;
 *   <integer>         := 0 | ["-"],<natural_number>;
 *   <natural_number>  := <digit_excl_zero>,{<digit>};
 *   <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 *   <digit>           := "0"| <digit_excl_zero>;
 */


/**
 *  Essa eh a classe Parser
 *  Aqui sao definidas as funcoes para fazer o parsing
 */
class Parser {
    public:
        /// Essa classe representa o resultado da operação parsing.
        struct ParserResult
        {
            //=== Alias
            typedef size_t size_type; //<! Usado para locação da coluna.

            // Lista de possíveis erros de sintaxe.
            enum code_t {
                    PARSER_OK = 0,
                    UNEXPECTED_END_OF_EXPRESSION,
                    ILL_FORMED_INTEGER,
                    MISSING_TERM,
                    EXTRANEOUS_SYMBOL,
                    INTEGER_OUT_OF_RANGE,
                    MISSING_CLOSING_PARENTHESIS
            };

            //=== Membros (public).
            code_t type;      //<! Error code.
            size_type at_col; //<! Guarda a coluna do erro.

            // Por padrão, o resultado é positivo.
            explicit ParserResult( code_t type_=PARSER_OK , size_type col_=0u )
                    : type{ type_ }
                    , at_col{ col_ }
            { /* empty */ }
        };

        //==== Aliases
        typedef short int required_int_type;
        typedef long long int input_int_type;

        //==== Public interface

        /** @brief Realiza o parsing
            @param e_ Expressão.
            @return Resultado. */
        ParserResult parse( std::string e_ );

        /** @brief Recupera tokens.
            @return Lista de tokens. */
        std::vector< Token > get_tokens( void ) const;

        //==== Special methods
        /// Constutor default.
        Parser() = default;
        ~Parser() = default;
        /// Desligar cópia e atribuição.
        Parser( const Parser & ) = delete;  // Construtor cópia.
        Parser & operator=( const Parser & ) = delete; // Atribuição.

    private:
        //=== Aliases
        static constexpr bool SUCCESS{ true };
        static constexpr bool FAILURE{ false };

        // Tabela de simbolos Terminal
        enum class terminal_symbol_t{  // Os simbolos:-
            TS_PLUS,                   //<! "+"
            TS_MINUS,                  //<! "-"
            TS_MULTIPLICATION,         //<! "*"
            TS_DIVISION,               //<! "/"
            TS_REST,                   //<! "%"
            TS_POTENTIATION,           //<! "^"
            TS_ZERO,                   //<! "0"
            TS_NON_ZERO_DIGIT,         //<! "1"->"9"
            TS_WS,                     //<! white-space
            TS_TAB,                    //<! tab
            TS_EOS,                    //<! End Of String
            TS_INVALID                 //<! invalid token
        };

        //==== Private members.
        std::string expr;                    //<! A expressão a ser parsed
        std::string::iterator it_curr_symb;  //<! Ponteiro para o atual char dentro da expressão.
        std::vector< Token > token_list;     //<! Resultado da lista de tokens extraído da expressão.

        /** @brief Converte de caractere para código do símbolo terminal.
            @param ch Caractere.
            @return Código do simbolo terminal. */
        terminal_symbol_t lexer( char ch ) const;

        /** @brief Converte um simbolo terminal para a string correspondente.
		    @param ch Caractere.
		    @return Código do simbolo terminal. */
        std::string token_str( terminal_symbol_t s_ ) const;

        //=== Support methods.
        
        /** @brief Iterador para avançar para próximo char na expressão. */
        void next_symbol( void );                

        /** @brief Verifica se o símbolo atual é igual ao símbolo do terminal solicitado.
            @param s_ Simbolo do terminal.
            @return 1 se for igual 0 otherwise. */
        bool peek( terminal_symbol_t s_ ) const;

        /** @brief Tenta aceitar o símbolo solicitado.
            @param s_ Simbolo do terminal.
            @return 1 se aceitar 0 otherwise. */
        bool accept( terminal_symbol_t s_ );

        /** @brief Ignora qualquer WS/Tab e tenta aceitar o símbolo solicitado.
            @param s_ Simbolo do terminal.
            @return 1 se aceitar 0 otherwise. */
        bool expect( terminal_symbol_t );

        /** @brief Ignora qualquer WS/Tab e para no próximo caractere. */
        void skip_ws( void );                    

        /** @brief Verifica se chegamos ao final da sequência de expressão.
            @return 1 se chegou 0 otherwise. */
        bool end_input( void ) const;     

        /** @brief Converte um string em um integer.
            @param ch Caractere .
            @return Caractere no tipo inteiro. */ 
        input_int_type str_to_int( std::string ch );


        //=== NTS methods.

        /** @brief Processa uma expressão.
            @return Resultado do processamento. */ 
        ParserResult expression();

        /** @brief Processa um termo.
            @return Resultado do processamento. */ 
        ParserResult term();

        /** @brief Processa um inteiro.
            @return Resultado do processamento. */ 
        ParserResult integer();

        /** @brief Processa um número natural.
            @return Resultado do processamento. */ 
        ParserResult natural_number();


        //=== Terminal methods - must return bool

        /** @brief Verifica se número é 1...9.
            @return 1 se for 0 otherwise. */
        bool digit_excl_zero();

        /** @brief Verifica se número é 0...9.
            @return 1 se for 0 otherwise. */
        bool digit();
};

#endif
