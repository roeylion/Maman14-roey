#ifndef BINARY_H
#define BINARY_H

#include "globals.h"

/*
 * Converts an instruction to binary representation.
 *
 * @param opcode The opcode of the instruction.
 *
 * @param sourceOperand The source operand of the instruction.
 * Pass NULL if there is no source operand.
 *
 * @param targetOperand The target operand of the instruction.
 * Pass NULL if there is no target operand.
 *
 * @return The binary representation of the instruction.
 * Returns NULL if the instruction or operands are invalid.
 */
string instruction_to_binary(unsigned int opcode, operand *sourceOperand, operand *targetOperand);

/*
* Converts a directive to its binary representation.
*
* @param directive directive struct containing the directive to convert (.string or .data)
*
* @return The binary representation of the Directive as a string.
*/
string directive_to_binary(struct directive directive);

string * data_to_binary(int *data, unsigned int length);


#endif
