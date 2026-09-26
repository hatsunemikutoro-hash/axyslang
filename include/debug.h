#ifndef DEBUG_H
#define DEBUG_H

#include "parser.h"   
#include "vm.h"      
#include <stdlib.h>

extern int DEBUG_TOKENS;
extern int DEBUG_AST;
extern int DEBUG_VM;
extern int DEBUG_TAPE;

int debug_parse_arg(int argc, char **argv);

const char *token_name(TokenType t);
char *my_strdup(const char *s);
void debug_token(Token t);
void debug_ast(ASTnode *node, int depth);
void debug_tape(Machine *m, int count);


#endif