#include "tokenizer.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

size_t slen(const char *str) {
    size_t len = 0;
    while (*str++) {
        len++;
    }
    return len;
    
}

char *Lower(const char *str)
{
    char *new_string = malloc(sizeof(char) * slen(str) + 1);
    for (int i = 0; str[i]; i++) {
        new_string[i] = tolower(str[i]);
    }

    new_string[slen(str)] = '\0';

    return new_string;
}

// funcao pra skipa whitespace pode pa
void sskip(Lexer *lexer)
{
    while (lexer->c[lexer->size] == ' ')
    {
        lexer->size++;
    }
}

int isAlpha(char c)
{

    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

int isNum(char c)
{
    return c >= '0' && c <= '9';
}

int isAlnum(char c)
{
    return isAlpha(c) || isNum(c);
}

void skip_comment(Lexer *lexer)
{
    if (lexer->c[lexer->size] == '@')
    {
        while (lexer->c[lexer->size] != '\n' && lexer->c[lexer->size] != '\0')
        {
            lexer->size++;
        }
    }
}

char seek(Lexer *lexer) {
    return lexer->c[lexer->size + 1];
}

// abacaxi

TokenType KW_find(const char *str)
{

    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(Lower(str), keywords[i].name) == 0)
        {
            return keywords[i].type;
        }
    }

    return IDENTIFIER;
}

Token next_token(Lexer *lexer)
{
    Token n_token;
    n_token.line = lexer->line;
    sskip(lexer);
    skip_comment(lexer);
    int start = lexer->size;

    char c = lexer->c[lexer->size];

    // string tokenization
    if (c == '"')
    {
        lexer->size++;
        int start = lexer->size;

        while (lexer->c[lexer->size] != '"' && lexer->c[lexer->size] != '\0')
        {
            lexer->size++;
        }

        if (lexer->c[lexer->size] == '\0')
        {
            fprintf(stderr, "LEXICAL ERROR string not terminated missing closing quotes\n");
            n_token.type = UNKNOWN;
            return n_token;
        }

        int len = lexer->size - start;
        char *str = malloc(len + 1);

        if (str == NULL)
        {
            n_token.type = UNKNOWN;
            return n_token;
        }

        memcpy(str, &lexer->c[start], len);
        str[len] = '\0';

        if (lexer->c[lexer->size] == '"')
        {
            lexer->size++;
        }

        n_token.type = STRING;
        n_token.val.sval = str;

        return n_token;
    }

    if (c == '-' && seek(lexer) == '>') {
        n_token.type = ALIAS;
        lexer->size += 2;
        return n_token;
    }

    if (c == '=' && seek(lexer) == '=') {
        n_token.type = EQ;
        lexer->size += 2;
        return n_token;
    }

    if (c == '!' && seek(lexer) == '=') {
        n_token.type = NEQ;
        lexer->size += 2;
        return n_token;
    }

    switch (c)
    {
        case '\0': n_token.type = END; return n_token;
        case '\n': n_token.type = NEWLINE; lexer->line++; lexer->size++; return n_token;
        case '*': n_token.type = STAR; lexer->size++; return n_token;
        case '[': n_token.type = LBRACKET; lexer->size++; return n_token;
        case ']': n_token.type = RBRACKET; lexer->size++; return n_token;
        case '+':   n_token.type = PLUS;      lexer->size++; return n_token;
        case '-':   n_token.type = MINUS;     lexer->size++; return n_token;
        case '=':   n_token.type = ASSIGN;    lexer->size++; return n_token;
        case '>':   n_token.type = GT;   lexer->size++; return n_token;
        case '<':   n_token.type = LT;      lexer->size++; return n_token;
        case '(':   n_token.type = LPARENT;      lexer->size++; return n_token;
        case ')':   n_token.type = RPARENT;      lexer->size++; return n_token;
    }

    // int tokenization
    if (isNum(lexer->c[lexer->size]))
    {
        int number = 0;
        while (isNum(lexer->c[lexer->size]))
        {

            number = number * 10 + (lexer->c[lexer->size] - '0');

            lexer->size++;
        }

        n_token.type = INT;
        n_token.val.ival = number;
        return n_token;
    }

    // identifier and keyword tokenizer
    if (isAlpha(c))
    {
        while (isAlnum(lexer->c[lexer->size]))
        {
            lexer->size++;
        }
        int len = lexer->size - start;
        char *str = malloc(len + 1);

        if (str == NULL)
        {
            n_token.type = UNKNOWN;
            return n_token;
        }

        memcpy(str, &lexer->c[start], len);
        str[len] = '\0';

        n_token.type = KW_find(str);
        n_token.val.sval = str;

        return n_token;
    }

    n_token.type = UNKNOWN;
    lexer->size++;
    return n_token;
}

// int main() {
//     Lexer lexer;
//     lexer.c = '"Hello, World"';
//     lexer.size = 0;

//     printf("%s\n", debug_type(next_token(&lexer).type));
//     printf("%s\n", debug_type(next_token(&lexer).type));
//     printf("%s\n", debug_type(next_token(&lexer).type));
//     return 0;
// }