#include "globals.h"


const int instructions_addressing[OPCODES][MAX_OPERANDS][ADDRESSING_TYPES] = {
        /*mov opcode = 0 */
        {/*source operand addressing*/{1,3,5},/*target operand addressing*/{3,5}},
        /*cmp opcode = 1*/
        {{1,3,5},{1,3,5}},
        /**/
        {{1,3,5},{3,5}}
};
const string instruction_names[16] = {
        "mov",
        "cmp",
        "add",
        "sub",
        "not",
        "clr",
        "lea",
        "inc",
        "dec",
        "jmp",
        "bne",
        "red",
        "prn",
        "jsr",
        "rts",
        "stop"
};
const struct instruction_data instructions[OPCODES] = {{"mov", mov_opcode, {immediate_addressing, direct_addressing, register_addressing}, {direct_addressing,    register_addressing},2},
                                                       {"cmp", cmp_opcode, {immediate_addressing, direct_addressing, register_addressing}, {immediate_addressing, direct_addressing, register_addressing},2},
                                                       {"add", add_opcode, {immediate_addressing, direct_addressing, register_addressing}, {direct_addressing,    register_addressing},2},
                                                       {"sub", sub_opcode, {immediate_addressing, direct_addressing, register_addressing}, {direct_addressing,    register_addressing},2},
                                                       {"not", not_opcode, {no_addressing,        no_addressing,     no_addressing},       {direct_addressing,    register_addressing},1},
                                                       {"clr", clr_opcode, {no_addressing,        no_addressing,     no_addressing},       {direct_addressing,    register_addressing},1},
                                                       {"lea", lea_opcode, {direct_addressing},                                            {direct_addressing,    register_addressing},2},
                                                       {"inc", inc_opcode, {no_addressing,        no_addressing,     no_addressing},       {direct_addressing,    register_addressing},1},
                                                       {"dec", dec_opcode, {no_addressing,        no_addressing,     no_addressing},       {direct_addressing,    register_addressing},1},
                                                       {"jmp", jmp_opcode, {no_addressing,        no_addressing,     no_addressing},       {direct_addressing,    register_addressing},1},
                                                       {"bne", bne_opcode, {no_addressing,        no_addressing,     no_addressing},       {direct_addressing,    register_addressing},1},
                                                       {"red",  red_opcode,  {no_addressing,no_addressing,no_addressing}, {direct_addressing,    register_addressing},1},
                                                       {"prn",  prn_opcode,  {no_addressing,no_addressing,no_addressing}, {immediate_addressing, direct_addressing, register_addressing},1},
                                                       {"jsr",  jsr_opcode,  {no_addressing,no_addressing,no_addressing}, {direct_addressing,    register_addressing},1},
                                                       {"rts",  rts_opcode,  {no_addressing,no_addressing,no_addressing}, {no_addressing,no_addressing,no_addressing},0},
                                                       {"stop", stop_opcode, {no_addressing,no_addressing,no_addressing}, {no_addressing,no_addressing,no_addressing},0}
};


