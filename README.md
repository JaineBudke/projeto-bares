# PROJETO-BARES


## Sobre o projeto:

O Bares é um programa desenvolvido com o propósito de realizar operações aritméticas fornecidas por meio de um arquivo externo. A análise sintática das expressões é feita tratando os erros apresentados no tópico seguinte.


## Erros tratados:

- Unexpected end of expression;
- Ill formed integer;
- Missing term;
- Extraneous symbol;
- Missing closing parenthesis;
- Integer out of range.


## Como compilar:

Para compilar o projeto, basta digitar, pelo terminal, o comando abaixo

    g++ -std=c++11 src/main.cpp src/baresmanager.cpp src/parser.cpp -I include -o bares
    

Comando para executar o programa
    
    ./bares data/_ARQUIVO-COM-OPERACOES_


## TODO

- [X] Receber dados via leitura de arquivo.
- [X] Valida expressão e cria lista de tokens.
- [X] Detecta corremente um conjunto de erros especificados.
- [ ] Converte do formato infixo para posfixo.
- [ ] Realiza a operação corretamente.
- [ ] Inclui e trata corretamente o uso de parênteses.
- [ ] Salvar dados em arquivo externo.


Programa desenvolvido por _Jaine Budke_ (< *jainebudke@hotmail.com* >), 2017.1
