#include "debug.h"
#include <stdio.h>
#include <string.h>

int DEBUG_TOKENS = 0;
int DEBUG_AST    = 0;
int DEBUG_VM     = 0;
int DEBUG_TAPE   = 0;

char *my_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p != NULL)
    {
        memcpy(p, s, len);
    }
    return p;
}

int debug_parse_arg(int argc, char **argv) {
    int file_index = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--token") == 0) DEBUG_TOKENS = 1;
        else if (strcmp(argv[i], "--ast")   == 0) DEBUG_AST = 1;
        else if (strcmp(argv[i], "--vm")    == 0) DEBUG_VM = 1;
        else if (strcmp(argv[i], "--tape")  == 0) DEBUG_TAPE = 1;
        else if (strcmp(argv[i], "--debug") == 0) {
            DEBUG_TOKENS = DEBUG_AST = DEBUG_VM = DEBUG_TAPE = 1;
        }
        else {
            if (file_index == -1) file_index = i;
        }
    }

    return file_index;
}

const char *token_name(TokenType t) {
    switch (t) {
    case INT:        return "INT";
    case IDENTIFIER: return "IDENTIFIER";
    case STRING:     return "STRING";
    case NEWLINE:    return "NEWLINE";
    case KW_PRINT:   return "KW_PRINT";
    case KW_MOVE:    return "KW_MOVE";
    case KW_JUMP:    return "KW_JUMP";
    case KW_SET:     return "KW_SET";
    case KW_PRINTC:  return "KW_PRINTC";
    case KW_EXIT:    return "KW_EXIT";
    case KW_ADD:     return "KW_ADD";
    case KW_SUB:     return "KW_SUB";
    case KW_MULT:    return "KW_MULT";
    case KW_DIV:     return "KW_DIV";
    case STAR:       return "STAR";
    case ALIAS:      return "ALIAS";
    case ASSIGN:     return "ASSIGN";
    case INDEX:       return "INDEX";
    case LBRACKET:   return "LBRACKET";
    case RBRACKET:   return "RBRACKET";
    case END:        return "END";
    case UNKNOWN:    return "UNKNOWN";
    case MINUS:        return "MINUS";
    case PLUS:    return "PLUS";
    case GT:        return "GT";
    case LT:    return "LT";
    }
    return "???";
}

void debug_token(Token t) {
    printf("[TOKEN] %-12s line=%-3d", token_name(t.type), t.line);
    switch (t.type) {
    case INT:        printf(" val=%d",  t.val.ival); break;
    case STRING:
    case IDENTIFIER: printf(" name=%s", t.val.sval); break;
    default: break;
    }
    printf("\n");
}

void debug_ast(ASTnode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
    case AST_INT:      printf("INT(%d)\n", node->value.ival); break;
    case AST_STRING:   printf("STRING(\"%s\")\n", node->value.sval); break;
    case AST_IDENT:    printf("IDENT(%s)\n", node->value.sval); break;
    case AST_DEREF:    printf("DEREF\n"); break;
    case AST_INDEX:    printf("INDEX\n"); break;
    case AST_PRINT:    printf("PRINT\n"); break;
    case AST_PRINTC:   printf("PRINTC\n"); break;
    case AST_SET:      printf("SET\n"); break;
    case AST_ADD:      printf("ADD\n"); break;
    case AST_SUB:      printf("SUB\n"); break;
    case AST_MULT:     printf("MULT\n"); break;
    case AST_DIV:      printf("DIV\n"); break;
    case AST_MOVE:     printf("MOVE\n"); break;
    case AST_JUMP:     printf("JUMP\n"); break;
    case AST_EXIT:     printf("EXIT\n"); break;
    case AST_ALIAS:     printf("ALIAS\n"); break;
    default:           printf("?(%d)\n", node->type); break;
    }

    debug_ast(node->left,  depth + 1);
    debug_ast(node->right, depth + 1);
}

void debug_tape(Machine *m, int count) {
    printf("[TAPE] cursor=%d ip=%lu died=%d\n", m->cursor, m->ip, m->died);
    for (int i = 0; i < count; i++) {
        printf("  [%3d] = %d\n", i, m->memory[i]);
    }
}