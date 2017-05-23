/**
 * @file    baresmanager.cpp
 * @brief   Código fonte com a implementacao de funcoes que
            realizam a execução geral do bares.
 * @author  Jaine Budke (jainebudke@hotmail.com)
 * @since   02/05/2017
 * @date    23/05/2017
 */


#include "bares-manager.h" // classe BaresManager.


////////////////////////////////////////////////////////////////////////////
// Funcoes auxiliares
////////////////////////////////////////////////////////////////////////////

/**
 * @brief Imprime mensagem de expressão com erro de sintaxe para usuário.
 * @param result Resultado do parser.
 * @param str String com expressão
 */
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

/** @brief Apresenta mensagem final das expressões com erro de sintaxe.
    @param cont Contador
    @param arqsaida Arquivo de saída dos resultados
    @param result O resultado do Parser */
void BaresManager::message( int cont, std::ofstream & arqsaida, const Parser::ParserResult & result ){

    int tam = expressions[cont].size();
    std::cout << tam << "\n";

    switch ( result.type )
    {
        case Parser::ParserResult::UNEXPECTED_END_OF_EXPRESSION:
            arqsaida << "Unexpected end of input at column (" << result.at_col << ")!";
            break;
        case Parser::ParserResult::ILL_FORMED_INTEGER:
            arqsaida << "Ill formed integer at column (" << result.at_col << ")!";
            break;
        case Parser::ParserResult::MISSING_TERM:
            arqsaida << "Missing <term> at column (" << result.at_col << ")!";
            break;
        case Parser::ParserResult::EXTRANEOUS_SYMBOL:
            arqsaida << "Extraneous symbol after valid expression found at column (" << result.at_col << ")!";
            break;
        case Parser::ParserResult::MISSING_CLOSING_PARENTHESIS:
            arqsaida << "Missing closing \")\" at column (" << result.at_col << ")!";
            break;
        case Parser::ParserResult::INTEGER_OUT_OF_RANGE:
            arqsaida << "Integer constant out of range beginning at column (" << result.at_col << ")!";
            break;
        default:
            arqsaida << "Unhandled error found!";
            break;
    }

}


/**
 * @brief Verifica se token é um operando.
 * @param t Token.
 * @return 1 se é operando 0 otherwise
 */
bool is_operand( const Token & t ){
    return t.type == Token::token_t::OPERAND;
}

/**
 * @brief Verifica se token é um operador.
 * @param t Token.
 * @return 1 se é operando 0 otherwise
 */
bool is_operator( const Token & t ){
    return t.type == Token::token_t::OPERATOR;
}

/**
 * @brief Verifica a prioridade do operador passado por parametro.
 * @param c operador.
 * @return valor de prioridade.
 */
int get_precedence( char c )
{
    int weight(0);
    switch( c )
    {
        case '^': weight = 3;
                  break;
        // ==================================================
        case '*':
        case '%':
        case '/': weight = 2;
                  break;
        // ==================================================
        case '+':
        case '-': weight = 1;
                  break;
        // ==================================================
        case '(':
                  weight = 0;
                  break;
        // ==================================================
        default:
            /* Empty */;
    }

    return weight;
}

/**
 * @brief Verifica se operador é potenciação ou não.
 * @param op operador.
 * @return 1 se é potencia 0 otherwise.
 */
bool is_right_association( char op )
{
    return op == '^';
}

/**
 * @brief Determina se o primeiro operador é maior do que o segundo operador.
 * @param op1 Primeiro operador para comparação.
 * @param op2 Segundo operador para comparação.
 * @return 1 se op1 >= op2 0 otherwise.
 */
bool has_higher_precedence( char op1, char op2 ) {
    auto p1 = get_precedence( op1 );
    auto p2 = get_precedence( op2 );

    // special case: has the same precedence and is right association.
    if ( p1 == p2 and is_right_association( op1 ) )
    {
        return false;
    }

    return p1 >= p2 ;
}

/**
 * @brief Executa uma operação.
 * @param n1 Primeiro número inteiro para a operação.
 * @param n2 Segundo número inteiro para a operação.
 * @param opr Operador da operação.
 * @return Resultado da operação.
 */
long int execute_operator( long int  n1, long int  n2, char opr ){

    long int  result(0);
    switch ( opr )
    {
        case '^' : result = static_cast<long int >( pow( n1, n2 ) );
                   break;
        case '*' : result =  n1 * n2;
                   break;
        case '/' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero" );
                   result = n1/n2;
                   break;
        case '%' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero" );
                   result = n1%n2;
                   break;
        case '+' : result = n1 + n2;
                   break;
        case '-' : result =  n1 - n2;
                   break;
        default: assert(false);
    }

    return result;

}

/**
 * @brief Transforma um char em inteiro.
 * @param ch Char que será transformado em inteiro.
 * @return Valor inteiro equivalente ao char passado.
 */
long int char2integer( std::string ch ){

    int tam = ch.size();
    int val = 0;

    long int num;

    if( ch[0] == '-' ){
    	int menos = 1;
    	int cnt = 1;
    	while( ch[cnt] == '-' ){
    		menos += 1;
    		cnt += 1;
    	}
    	for( auto i( std::begin(ch)+menos ) ; i != std::end(ch) ; i++ ){
	        num = *i - '0';
	        int dec = pow( 10, (tam-1-menos) );

	        val += num*dec;
	        tam --;
	    }
	    return val*pow((-1), menos);
    } else {
    	for( auto i( std::begin(ch) ) ; i != std::end(ch) ; i++ ){
	        num = *i - '0';
	        int dec = pow( 10, (tam-1) );

	        val += num*dec;
	        tam --;
	    }
	    return val;
    }

}


////////////////////////////////////////////////////////////////////////////
// Funcoes principais
////////////////////////////////////////////////////////////////////////////

/** @brief Inicializa lendo o arquivo de entrada fornecido pelo cliente.
    @param arq Nome do arquivo de entrada.
    @return 1 se o arquivo foi lido corretamente; 0 otherwise. */
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

    // fechando arquivo
    arquivo.close();
}

/** @brief Valida expressões e separa em tokens.
    @return Vetor com os tokens de todas as expressões no formato infix. */
std::vector< std::vector< Token > > BaresManager::validarExpress(){

    Parser my_parser; // Instancia um parser.

    std::vector< std::vector< Token > > allTokens;

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

        auto lista = my_parser.get_tokens();

        if ( result.type == Parser::ParserResult::PARSER_OK ) {
            allTokens.push_back( lista );
        }

        std::cout << ">>> Tokens: { ";
        std::copy( lista.begin(), lista.end(),
            std::ostream_iterator< Token >(std::cout, " ") );
        std::cout << "}\n";

    }

    return allTokens;

}

/** @brief Transforma expressão do formado infix para postfix.
    @param inflix_ Tokens da expressão no formato infix
    @return Vetor com os tokens de todas as expressões no formato postfix. */
std::vector< std::vector< Token > > BaresManager::infix_to_postfix( std::vector< Token > infix_ ){

    // Stack para ajudar a converter a expressao.
    std::stack< char > s;

    std::vector< Token > temp;

    // Percorre expressao infixa
    for( auto & tk : infix_ ){
        // Operando vai direto para fila de saída
        if ( is_operand( tk ) ) // 1 23 100, etc.
        {
            temp.push_back( Token( tk.value, Token::token_t::OPERAND ) );
        }
        else if ( is_operator( tk ) ) // + - ^ % etc.
        {

            // conversao std::string para char
            char op = (tk.value)[0];

            // Tirar todos os elementos com prioridade alta
            while( not s.empty() and
                   has_higher_precedence( s.top() , op ) )
            {
                std::string top;
                top.push_back( s.top() );
                temp.push_back( Token( top, Token::token_t::OPERATOR ) );
                s.pop();
            }

            // The incoming operator always goes into the stack.
            s.push( op );

        }
        else // anything else.
        {
            // ignore this char.
        }
    }

    // Tirar todos os operadores restantes na pilha
    while( not s.empty() )
    {
        std::string top;
        top.push_back( s.top() );
        temp.push_back( Token( top, Token::token_t::OPERATOR ) );

        s.pop();
    }

    postfix.push_back( temp );

    return postfix;

}

/** @brief Realiza a operação.
    @param postfix Vetor com tokens da expressão no formato postfix
    @return Resultado da expressão. */
int BaresManager::evaluate_postfix( std::vector< Token > postfix ) {

    std::stack< long int > s;

    // Percorre expressao posfixa
    for( auto tk : postfix ){

        if ( is_operand( tk ) ) { // verifica se o token é um operando
            s.push( char2integer( tk.value ) );
        }

        else if ( is_operator( tk ) ) {
            // Recupera os dois operandos na ordem inversa
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            char ch = (tk.value)[0];
            std::cout << ">>> Performing " << op1 << " " << ch << " " << op2 << "\n";
            int result = execute_operator( op1, op2, ch );
            s.push(result);
        }

        else {
            assert(false);
        }

    }

    int result = s.top();

    std::cout << ">>> The result is: "  << result << std::endl;
    std::cout << "\n";

    return result;

}

/** @brief Apresenta resultado final das expressões.
    @param res Vetor com resultados das expressões válidas */
void BaresManager::apresentarResult( std::vector< int > res ){

    Parser my_parser; // Instancia um parser.

    std::vector< std::vector< Token > > allTokens;

    // Configurando saída dos dados em arquivo
    std::ofstream arqsaida;
    // Cria e abre arquivo
    arqsaida.open( "resultados.txt" , std::ios::out );
    // Se houver erro, sai do programa
	if ( !arqsaida.is_open() )
		return;


    int cont = 0;

    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions ){

        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );

        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ParserResult::PARSER_OK ){
            message( cont, arqsaida, result );
        }
        else{
        	int tam = expressions[cont].size();
            arqsaida << res[cont];
            cont++;
        }

        arqsaida << "\n";

    }

    arqsaida.close();

}