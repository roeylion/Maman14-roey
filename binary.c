#include "helpers.h"
#include "errors.h"

/* Converts a first_word struct to its  binary representation.
 * @param fw - struct first_word containing the word to convert.
 * @return A dynamically allocated string containing the binary representation of the word.
 * */
static string first_word_to_binary(struct first_word fw);

/*
Converts an integer to its binary representation.
@param number The integer to convert.
@param bits The number of bits to use for the binary representation.
@return A dynamically allocated string containing the binary representation of the integer.
 */
static string int_to_binary(int number, int bits);

/*
 * Converts a string to its binary representation with each letter in a separate 12 bits line including '\0'.
 *
 * @param str The string to convert.
 * @return A dynamically allocated string containing the binary representation of the input string.
 *         Returns NULL if memory allocation fails.
 */
static string string_to_binary(string str);
/*

Converts an array of integers to binary representation.

@param data Array of integers to convert.

@param length Length of the array.

@return Binary representation of the integers as a string.
*/
static string * data_to_binary(int data[], unsigned int length);

/*
 * Converts an operand to binary representation.
 *
 * @param op The operand to convert.
 * @return The binary representation of the operand.
 *         Returns NULL if the operand is invalid.
 */
string operand_to_binary(operand *op);


string first_word_to_binary(struct first_word fw) {
    int numBits = 12;
    string binary, temp;
    BLOCK_ALLOCATION(binary, char, numBits + 1);
    CHECK_NULL(binary, PRINT_ALLOCATION_FAILED_MSG; return NULL;);
    /*initialize binary*/
    strcpy(binary, "");

    /*Convert the fields to binary*/
    STRCAT_FUNCTION(binary, int_to_binary(fw.source_oprnd, 3), temp);
    STRCAT_FUNCTION(binary, int_to_binary(fw.opcode, 4), temp);
    STRCAT_FUNCTION(binary, int_to_binary(fw.target_oprnd, 3), temp);
    STRCAT_FUNCTION(binary, int_to_binary(fw.A_R_E, 2), temp);
    return binary;
}

 string int_to_binary(int number, int bits) {
    int mask, i;
    BLOCK_ALLOCATION(string binary, char, bits + 1);
    CHECK_NULL(binary, PRINT_ALLOCATION_FAILED_MSG; return NULL;);
    mask = 1 << (bits - 1);
    for (i = 0; i < bits; ++i) {
        binary[i] = (number & mask) ? '1' : '0';
        mask >>= 1;
    }
    binary[bits] = '\0';

    return binary;
}


string string_to_binary(string str) {
    int i;
    string binary, temp;
    /*initialize binary*/
    /*each line has 13 characters including\n, the amount of lines is 1 more than the length of the string and an extra character for \0*/
    BLOCK_ALLOCATION(binary, char, ((WORD_SIZE + 1) * (strlen(str) + 1)) +1);
    CHECK_NULL(binary, PRINT_ALLOCATION_FAILED_MSG; return NULL;);
    binary = strcpy(binary, "");
    /*convert each char to binary including '\0'*/
    for (i = 0; i <= strlen(str); ++i) {
        STRCAT_FUNCTION(binary, int_to_binary(str[i], WORD_SIZE), temp);
        strcat(binary, "\n");
    }
    return binary;
}

string * data_to_binary(int *data, unsigned int length) {
    int i;
    string *binary,temp;
    /*initialize binary*/
    BLOCK_ALLOCATION(binary,string, length);
    /*convert each number to binary*/
    for ( i = 0; i < length; ++i) {
        binary[i] = malloc(WORD_SIZE+1);
        CHECK_NULL(binary[i],PRINT_ALLOCATION_FAILED_MSG; return NULL;);
        temp = int_to_binary(data[i],WORD_SIZE);
        strcpy(binary[i],temp);
        free(temp);
      /*  STRCAT_FUNCTION(binary, int_to_binary(data[i], 12), temp);
        strcat(binary,"\n");*/
    }
    return binary;
}

string operand_to_binary(operand *op) {
    string binary, temp;
    BLOCK_ALLOCATION(binary,char ,WORD_SIZE+1);
    switch (op->type) {
        case immediate_addressing:
            STRCAT_FUNCTION(binary, int_to_binary(op->value.value, 10), temp);
            STRCAT_FUNCTION(binary, int_to_binary(A, 2), temp);
            break;
        case direct_addressing:
            STRCAT_FUNCTION(binary, int_to_binary(op->value.label.address, 10), temp);
            STRCAT_FUNCTION(binary, int_to_binary(op->value.label.R_E, 2), temp);
            break;
        case register_addressing:
            STRCAT_FUNCTION(binary, int_to_binary(op->value.reg, 5), temp);
            STRCAT_FUNCTION(binary, int_to_binary(A, 2), temp);
            break;
        default:
            return NULL;
    }
    return binary;
}


string directive_to_binary(struct directive directive){
    string binary;
    switch (directive.type) {
        case string_dir:
            binary = string_to_binary(directive.content.str);
            break;
        case data_dir:
            binary = data_to_binary(directive.content.data.data, directive.content.data.dataLength);
            break;
    }
    return binary;
}
string instruction_to_binary(unsigned int opcode, operand *sourceOperand, operand *targetOperand) {
    struct first_word firstWord;
    string binary, nextWords, temp; /*temp is used to free the returned values from the helping functions*/
    int lines = 3;/*the maximum amount of lines in machine code that can be created by a single instruction*/

    /*initialize binary*/
    BLOCK_ALLOCATION(binary, char, (WORD_SIZE + 1) * lines);
    CHECK_NULL(binary, PRINT_ALLOCATION_FAILED_MSG; return NULL;);
    strcpy(binary, "");

    /*initialize firstWord*/
    firstWord.opcode = opcode;
    firstWord.source_oprnd = sourceOperand->type;
    firstWord.target_oprnd = targetOperand->type;
    firstWord.A_R_E = A;
    printf("A_R_E: %d\nopcode: %d\nsource addressing:%d\ntarget addressing:%d\n", firstWord.A_R_E, firstWord.opcode,
           firstWord.source_oprnd, firstWord.target_oprnd);

    /*add the first word to binary*/
    STRCAT_FUNCTION(binary, first_word_to_binary(firstWord), temp);
    strcat(binary, "\n");

    /*check for operands*/
    if (targetOperand!=NULL|| sourceOperand!=NULL) {
        printf("operands exist\n");
        /*initialize nextWords*/
        BLOCK_ALLOCATION(nextWords, char, WORD_SIZE + 1);
        CHECK_NULL(nextWords, PRINT_ALLOCATION_FAILED_MSG; return NULL;);
        strcpy(nextWords, "");

        /*handle private case of both source and target operands are registers*/
        if (sourceOperand->type == register_addressing && targetOperand->type == register_addressing) {
            /*bits 7-11 are source register, bits 2-6 are target register, bits 0-2 are A_R_E - A*/
            STRCAT_FUNCTION(nextWords, int_to_binary(sourceOperand->value.reg, 5), temp);
            STRCAT_FUNCTION(nextWords, int_to_binary(targetOperand->value.reg, 5), temp);
            STRCAT_FUNCTION(nextWords, int_to_binary(A, 2), temp);
        } else {
            if (sourceOperand!=NULL) {
                printf("source operand exists\n");
                /*encode the source operand*/
                STRCAT_FUNCTION(nextWords,operand_to_binary(sourceOperand),temp);
                    strcat(nextWords, "\n");
            }
            printf("target operand exists\n");
            STRCAT_FUNCTION(nextWords, operand_to_binary(targetOperand),temp);
        }
        strcat(binary, nextWords);
        free(nextWords);
    }
    return binary;
}
