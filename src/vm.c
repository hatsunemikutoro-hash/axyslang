#include "vm.h"
#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"

Machine *vm_create(void)
{
    Machine *machine = malloc(sizeof(Machine));
    memset(machine->memory, 0, sizeof(machine->memory));
    machine->cursor = 0;
    machine->alias_count = 0;
    machine->ip = 0;
    machine->died = 0;
    return machine;
}

void vm_destroy(Machine *machine)
{
    if (machine == NULL) {
        return;
    }

    for (int i = 0; i < machine->alias_count; i++)
    {
        free(machine->aliases[i].name);
    }

    free(machine);
    
}

static int find_aliases(Machine *machine, const char *name, int *address)
{
    if (machine == NULL || name == NULL || address == NULL)
    {
        return 0;
    }

    for (int i = 0; i < machine->alias_count; i++)
    {
        if (strcmp(machine->aliases[i].name, name) == 0)
        {
            *address = machine->aliases[i].address;
            return 1;
        }
    }

    return 0;
}

static int add_alias(Machine *machine, const char *name, int address)
{
    if (machine == NULL || name == NULL)
    {
        return 0;
    }

    if (address < 0 || address >= MAX_MEM)
    {
        fprintf(stderr, "INVALID CELL ADDRESS %d\n", address);
        return 0;
    }

    if (machine->alias_count >= MAX_ALIASES)
    {
        fprintf(stderr, "TO MANY ALIASES\n");
        return 0;
    }

    int previous_address;

    if (find_aliases(machine, name, &previous_address)) {
        fprintf(stderr, "ALIAS %s ALREADY EXISTS\n", name);
        return 0;
    }

    machine->aliases[machine->alias_count].name = my_strdup(name);
    machine->aliases[machine->alias_count].address = address;
    machine->alias_count++;

    return 1;
}

int resolve_operand(Machine *machine, ASTnode *operand, int *result)
{
    if (operand == NULL || result == NULL)
    {
        return 0;
    }

    switch (operand->type)
    {
        case AST_INT:
            *result = operand->value.ival;
            return 1;

        case AST_IDENT:
            return find_aliases(machine, operand->value.sval, result);

        case AST_DEREF: {
            if (operand->left == NULL)
            {
                return 0;
            }

            int address;

            if (!resolve_operand(machine, operand->left, &address))
            {
                return 0;
            }

            if (address < 0 || address >= MAX_MEM)
            {
                return 0;
            }

            *result = machine->memory[address];
            return 1;
        }

        case AST_COMPARISON:
            {
                int valL;
                int valR;

                if (resolve_operand(machine, operand->left, &valL)) {
                    if (resolve_operand(machine, operand->right, &valR)) {
                        switch (operand->value.ival)
                        {
                        case EQ: *result = valL == valR; break;
                        case NEQ: *result = valL != valR; break;
                        case GT: *result = valL > valR; break;
                        case LT: *result = valL < valR; break;
                        
                        default:
                            return 0;
                        }
                    }
                }
                return 1;
            }

        default:
            return 0;
    }
}

int resolve_address(Machine *machine, ASTnode *node, int *address) {
    if (node == NULL || address == NULL) {return 0;}

    if (node->type == AST_INT) {
        *address = node->value.ival;
        return 1;
    }

    if (node->type == AST_INDEX) {
        int idx;
        if (!resolve_operand(machine, node->left, &idx)) return 0;
        if (idx < 0 || idx >= MAX_MEM) return 0;
        *address = idx;
        return 1;
    }

    return 0;
}

void vm_execute(Machine *machine, ASTnode *node)
{
    if (node == NULL)
    {
        return;
    }

    switch (node->type)
    {
        // BUILD-IN FUNCTIONS

    case AST_PRINT:
        if (node->left == NULL)
        {
            printf("%d", machine->memory[machine->cursor]);
        }

        if (node->left != NULL && node->left->type == AST_INT)
        {
            printf("%d\n", node->left->value.ival);
        }
        break;

    case AST_PRINTC:
        if (node->left == NULL)
        {
            printf("%c", machine->memory[machine->cursor]);
        }

        if (node->left != NULL && node->left->type == AST_INT)
        {
            printf("%c", node->left->value.ival);
        }

        if (node->left != NULL && node->left->type == AST_STRING)
        {
            printf("%s", node->left->value.sval);
        }
        break;

    case AST_MOVE:
    {
        int operand;
        if (resolve_operand(machine, node->left, &operand))
        {
            machine->cursor = operand;
        }
        break;
    }

    case AST_JUMP:
        if (node->left != NULL && node->left->type == AST_INT)
        {
            machine->ip = node->left->value.ival;
        }
        break;

    case AST_IF:
        {
                int result = 0;
                if (!resolve_operand(machine, node->left, &result)) {
                    break;
                }

                if (result) {
                    vm_execute(machine, node->right);
                }
                break;
        }

    case AST_BLOCK:
        {
            ASTnode *cur = node->left;
            while (cur != NULL) {
                vm_execute(machine, cur);
                cur = cur->next;
            }
            break;
        }

        // MATH UFNCTIONs

    case AST_SET:
    {
        int operand;
        if (resolve_operand(machine, node->left, &operand))
        {
            machine->memory[machine->cursor] = operand;
        }
        break;
    }

    case AST_ADD:
    {
        int operand;
        if (resolve_operand(machine, node->left, &operand))
        {
            machine->memory[machine->cursor] += operand;
        }
        break;
    }

    case AST_SUB:
    {
        int operand;
        if (resolve_operand(machine, node->left, &operand))
        {
            machine->memory[machine->cursor] -= operand;
        }
        break;
    }

    case AST_MULT:
    {
        int operand;
        if (resolve_operand(machine, node->left, &operand))
        {
            machine->memory[machine->cursor] *= operand;
        }
        break;
    }

    case AST_DIV:
    {
        int operand;
        if (resolve_operand(machine, node->left, &operand))
        {
            if (operand == 0)
            {
                fprintf(stderr, "Axis error: CANNOT DIVIDE BY ZERO\n");
                break;
            }

            machine->memory[machine->cursor] /= operand;
        }
        break;
    }

    case AST_EXIT:
        machine->died = 1;
        break;

    case AST_ALIAS:
        {
            if (node->left == NULL || node->right == NULL) break;

            int addr;
            if (!resolve_address(machine, node->left, &addr))break;

            add_alias(machine, node->right->value.sval, addr);
            break;
        }


    case AST_READ:
        {
            char buffer[128];
            char *endptr;
            
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                long valor = strtol(buffer, &endptr, 10);

                if (endptr == buffer || (*endptr != '\n' && *endptr != '\0' && *endptr != ' ')) {
                    printf("READ ERROR: Expected a NUMBER\n");
                } else {
                    machine->memory[machine->cursor] = valor;
                }
            } else {
                printf("READ ERROR: Failed to read STDIN\n");
            }
            break;
        }  

    case AST_READC: {
        int c = getchar();
        if (c == EOF) {
            printf("READC ERROR: Failed to read STDIN\n");
            break;
        }
        machine->memory[machine->cursor] = (unsigned char)c;

        if (c != '\n') {
            int next;
            while ((next = getchar()) != '\n' && next != EOF) {}
        }
        break;
    }

    default:
        break;
    }
}

void vm_run(Machine *machine, Program *program)
{
    while (!machine->died && machine->ip < program->count)
    {

        ASTnode *node = program->instructions[machine->ip];

        if (DEBUG_VM)
        {
            printf("[VM] ip=%lu cursor=%d\n", machine->ip, machine->cursor);
            debug_ast(node, 1);
        }

        machine->ip++;

        vm_execute(machine, node);

        if (DEBUG_VM)
        {
            printf("[VM] cursor=%d mem[cursor]=%d\n",
                   machine->cursor, machine->memory[machine->cursor]);
        }
    }

    if (DEBUG_TAPE)
        debug_tape(machine, 16);
}
