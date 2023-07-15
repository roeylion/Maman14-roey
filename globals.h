/*ARE:
 * A - number
 * R - label
 * E- extern label
 * */
#ifndef GLOBALS_H
#define GLOBALS_H
/*maximun length of a line (including \n)*/
#define LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define LABEL_END ':'
/*sizes*/
#define WORD_SIZE 12
#define MEMORY 1024
#define SYSTEM_USED_MEMORY 100
#define OPCODES 16
#define ADDRESSING_TYPES 3
#define MAX_OPERANDS 2
/*type definitions*/
extern const int instructions_addressing[OPCODES][MAX_OPERANDS][ADDRESSING_TYPES];
typedef char *string;
typedef enum booleans {
    false, true
} bool;
/*enums*/
enum registers {
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    INVALID_REG = -1
};
typedef enum opcodes {
    mov_opcode = 0,
    cmp_opcode,
    add_opcode,
    sub_opcode,
    not_opcode,
    clr_opcode,
    lea_opcode,
    inc_opcode,
    dec_opcode,
    jmp_opcode,
    bne_opcode,
    red_opcode,
    prn_opcode,
    jsr_opcode,
    rts_opcode,
    stop_opcode,
    illegal_opcode
} opcode;
enum directive_types {
    string_dir,
    data_dir,
    entry_dir,
    extern_dir
};

enum addressing_types {
    immediate_addressing = 1,/*number*/
    direct_addressing = 3,/*label*/
    register_addressing = 5,/*register*/
    no_addressing = -1
};
enum A_R_E {
    A = 0,
    R = 2,
    E = 1
};
enum operands {
    source,
    target
};

/*structs*/
typedef struct label label;
typedef struct directiveData directiveData;
typedef struct directive directive;
typedef struct operand operand;
typedef struct macro macro;
typedef struct first_word first_word;
typedef struct instruction_data instruction_data;
typedef struct code_line code_line;
typedef struct instruction instruction;
typedef struct command command;
/*
 * Represents a label in the assembly code.
 */
struct label {
    /*label name*/
    char name[MAX_LABEL_LENGTH];
    /*R or E(A is not possible) the label is extern or not (R for regular label, E for extern) */
    unsigned int R_E;
    /*the label address, if extern address=0*/
    unsigned int address;
    /*instruction or data*/
    unsigned int type;
};
/*
 * Represents the data in a .data directive.
 */
struct directiveData {
    /*array of the numbers*/
    int *data;
    /*length of the array*/
    unsigned int length;
};
/*
 * Represents a directive, either a string or directive data.
 */
struct directive {
    /*the directive type - string or content*/
    unsigned int type;
    /*the directive content*/
    union directive_content {
        /*the numbers in case of .content directive*/
        struct {
            int *data;
            unsigned int dataLength;
        } data;
        /*the string in case of .string directive*/
        string str;
        string *labels;
    } content;
};
/*
 * Represents an operand in an instruction.
 */
struct operand {
    /*the addressing type 1 ,3 ,5*/
    unsigned int type;
    /* the value of the operand
     * for number - the number itself
     * for label - struct label
     *for register - the register number */
    union {
        int value;
        unsigned int reg;
        struct label label;
    } value;

};
/*
 * Represents an instruction in the assembly code.
 */
struct instruction {
    opcode opcode;
    operand source_operand;
    operand target_operand;
};
/*
* Represents either an instruction or a directive and a label if there is.
*/
enum command_types{
    inst,
    dir
};
struct command {
    int type:2;
    /*name of the label if there is*/
    string label;
    union {
        instruction instruction;
        directive directive;
    } command;
};
/*
* Represents the first word of an instruction.
*/
struct first_word {
    unsigned int A_R_E: 2;
    unsigned int target_oprnd: 3;
    unsigned int opcode: 4;
    unsigned int source_oprnd: 3;
};
/*
 * Represents data about an instruction.
 */
struct instruction_data {
    string name;
    int opcode;
    int source_addrressing[3];
    int target_addrressing[3];
    int num_of_operands;
};
/*
 * Represents a line of code in the assembly code.
 */
struct code_line {
    string content; /*the contents of the line*/
    string file;/*the name of the file that contains the line*/
    unsigned int lineNum;/*number of the line*/
};
extern const struct instruction_data instructions[OPCODES];

/*represents all the legal addressing types for each instruction.
 * the instructions are sorted by opcodes (mov in the 0 cell, cmp in the 1...)
 * each opcode contains 2 arrays - for the source operand and for the target operand
 * each of the arrays contains the legal addressing types for the instruction*/

#endif
