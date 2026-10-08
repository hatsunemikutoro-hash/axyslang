#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum TokenType{
    INT,
    IDENTIFIER,
    STRING,
    NEWLINE,
    CHAR,

    // BuildIn functions

    
    KW_MOVE,
    KW_JUMP,
    KW_SET,
   
    KW_EXIT,

    KW_PRINT,
    KW_PRINTC,
    KW_READ,
    KW_READC,

    KW_IF,
    KW_THEN,
    KW_ENDIF,

    // math shit

    KW_ADD,
    KW_SUB,
    KW_MULT,
    KW_DIV,

    // types

    STAR,

    // assign
    ALIAS, // [0] -> home
    INDEX, //  [0]

    // expression
    LBRACKET,
    RBRACKET,
    LPARENT,
    RPARENT,
    ASSIGN,
    MINUS,
    PLUS,
    GT,
    LT,
    EQ,
    NEQ,
    

    END,
    UNKNOWN
} TokenType;

typedef struct Keyword
{
    const char *name;
    TokenType type;
}Keyword;

typedef struct Token
{
    union
    {
        int ival;
        char *sval;
    } val;
    TokenType type;
    int line;
} Token;

typedef struct Lexer
{
    int size;
    char *c;
    int line;
} Lexer;

static const Keyword keywords[] = {
    // buiiçldin fuction

    {"print", KW_PRINT},
    {"move", KW_MOVE},
    {"jump", KW_JUMP},
    {"set", KW_SET},
    {"printc", KW_PRINTC},
    {"exit", KW_EXIT},
    {"read", KW_READ},
    {"readc", KW_READC},

    // CONDICIONAL

    {"if", KW_IF},
    {"then", KW_THEN},
    {"end", KW_ENDIF},

    // math shit

    {"add", KW_ADD},
    {"sub", KW_SUB},
    {"mult", KW_MULT},
    {"div", KW_DIV}
};

Token next_token(Lexer *lexer);
void debug_token(Token t);

#endif
