

#ifndef PARSER_H
#define PARSER_H
#include "globals.h"
#include "helpers.h"
enum nodeTypes {
    LABEL_NODE,
    DIRECTIVE_NODE,
    INSTRUCTION_NODE,
    OPERAND_NODE
};
/*
typedef struct ASTNode {
    unsigned int type;
    union {
        opcode instruction;
        directive directive;
        operand operand;
    };
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;*/

command *line_to_struct(code_line line);
int check_format(code_line line, string format);
int get_operands(string line, int num_of_operands, instruction *inst, int *index);
#endif
