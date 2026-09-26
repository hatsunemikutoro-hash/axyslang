#include <stdio.h>
#include "parser.h"
#include <string.h>
#include "ast.h"
#include "openfile.h"
#include "interpreter.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    
    int file_index = debug_parse_arg(argc, argv);

    if (argc <= 1) {
        fprintf(stderr, "./axis filename [args]\n");
        return 1;
    }

    const char *filename = argv[file_index];

    Interpreter *interpreter = interpreter_create(filename);
    if (!interpreter) {
        fprintf(stderr, "Failed to create the interpreter\n");
        return 1;
    }

    interpreter_run(interpreter);
    interpreter_destroy(interpreter);
    return 0;
}