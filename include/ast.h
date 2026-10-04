#ifndef AST_H
#define AST_H

typedef enum
{

    // bildinkkkkkk

    AST_PRINT,
    AST_JUMP,
    AST_MOVE,
    AST_SET,
    AST_PRINTC,
    AST_EXIT,
    AST_ALIAS,
    AST_READ,

    // math shit

    AST_ADD,
    AST_SUB,
    AST_MULT,
    AST_DIV,

    // TYPES
    AST_INT,
    AST_STRING,
    AST_IDENT,
    AST_INDEX,
    AST_DEREF

} ASTType;

typedef struct ASTnode
{
    ASTType type;
    union
    {
        int ival;
        char *sval;
    } value;

    struct ASTnode *left;
    struct ASTnode *right;
} ASTnode;

#endif