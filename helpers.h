

#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "globals.h"

/*Macro definitions*/
/*
 * Returns true if the character is EOF, newline, or null character.
 *
 * @param chr: The character to check.
 * @return true if the character is EOF, newline, or null character, false otherwise.
 */
#define IS_END(chr) (chr == '\n' || chr == EOF || chr == '\0')

/*
 * Allocates memory using malloc.
 *
 * @param pointer: The pointer variable to assign the allocated memory.
 * @param type: The type of the allocated memory.
 * @param size: The number of elements of the specified type to allocate.
 */
#define BLOCK_ALLOCATION(pointer, type, size) pointer = (type*) malloc(sizeof(type) * (size))
/*
 * Checks if a pointer is NULL, and performs the provided actions if it is.
 *
 * @param ptr: The pointer to check.
 * @param message: The error message to display if the pointer is NULL.
 * @param action: The action to perform if the pointer is NULL.
 */
#define CHECK_NULL(ptr, action) do { \
    if ((ptr) == NULL) { \
        action \
    } \
} while (0)

/*
 * Makes the index point to the first non-white character in the string.
 *
 * @param string: The input string.
 * @param index: The index variable to update.
 */
#define CLEAR_SPACES(string, index) do { \
    while ((string)[(index)] && isspace((string)[index])) { \
        ++(index); \
    } \
} while (0)

/*
 * Macro to determine the length of an array.
 *
 * @param arr The input array.
 * @return The number of elements in the array.
 *
 * @note This macro assumes that the array is statically sized.
 */
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

/*
 * Removes the newline character from a string.
 *
 * @param str: The input string.
 */
#define REMOVE_NEWLINE(str) do { \
    (str)[strcspn((str), "\n")] = '\0'; \
} while (0)


/*
 * Extracts the first word of a line using strtok.
 *
 * @param line The input line from which to extract the first word.
 *
 * @return A pointer to the first word of the line, or NULL if no word is found.
 *
 * @note The input line is modified by strtok, replacing the delimiter with a null character.
 *       If you need to preserve the original line, make a copy of it before using this macro.
 */
#define FIRST_WORD_OF(line) strtok_r((line)," ",&)


#define STRCAT_FUNCTION(source, function, pointer) do { \
    pointer = function; \
    strcat(source, pointer); \
    free(pointer); \
} while (0)


#define REGISTER_VALUE(reg_name) \
    (strcmp(reg_name, "@r0") == 0) ? r0 : \
    (strcmp(reg_name,"@r1") == 0) ? r1 : \
    (strcmp(reg_name, "@r2") == 0) ? r2 : \
    (strcmp(reg_name, "@r3") == 0) ? r3 : \
    (strcmp(reg_name, "@r4") == 0) ? r4 : \
    (strcmp(reg_name, "@r5") == 0) ? r5 : \
    (strcmp(reg_name," @r6") == 0) ? r6 : \
    (strcmp(reg_name, "@r7") == 0) ? r7 : \
    INVALID_REG
/*function declarations*/
/*
 * Splits a string into individual words based on specified delimiters.
 *
 * @param line: The input string to be split into words.
 * @param delimiters: The delimiters used to separate words in the input string.
 * @param wordCount: A pointer to an integer that will store the count of words found in the input string.
 *
 * @returns: On success, a dynamically allocated array of strings representing the individual words.
 *           On failure returns NULL.
 *
 * Memory Management: The returned array of strings and the memory allocated for each individual word
 * should be freed by the caller using free_string_array when they are no longer needed.
 */string *split_to_words(string line, const string separtor, int *wordCount);


/*
* Frees an array of strings.
*
* @param string_array: The array of strings to be freed.
* @param count: The number of words in the array.
*/
void free_string_array(string *string_array, int count);

/*
 * Removes leading and trailing whitespace characters from the given string.
 *
 * @param str The string to trim (will be modified in place).
 */
void trim(string str);


bool is_keyword(string word);

unsigned int get_label_address(string name, struct label labels[], int labelCount);

/*
 * Check if an instruction supports a specific addressing type.
 *
 * This function verifies whether an instruction, identified by its opcode and the operand type (source or target),
 * supports a given addressing type. The function assumes valid parameters.
 *
 * @param opcode    The opcode of the instruction. Legal values: 0-16.
 * @param operand   The operand type to check. Legal values: enum operand - source (0) or target (1).
 * @param addressing    The addressing type to verify. Legal values: enum addressing_types - immediate_addressing (1),
 *                      direct_addressing (3), or register_addressing (5).
 *
 * @return  Returns true if the instruction supports the specified addressing type, false otherwise.
 */
bool is_legal_addressing(opcode opcode, operand operand, bool isTarget);

/*
 * Check if a given number is present in an array.
 *
 * @param arr       The array of integers to search.
 * @param size      The size of the array.
 * @param number    The number to search for in the array.
 * @return          Returns true if the number is found in the array, false otherwise.
 */
bool isNumberInArray(const int arr[], int size, int number);

/*
 * Duplicates a string by allocating memory for a new string and copying the contents of the original string (similar to strdup).
 *
 * @param str The original string to duplicate.
 * @return A pointer to the duplicated string, or NULL if memory allocation fails.
 */
string strduplicate(const string str);

int get_opcode (string instruction);

string removeChar(string str, char c);

const instruction_data * get_instruction_by_name(string name);

bool is_legal_addressing(opcode opcode, operand operand, bool isTarget);
/*
 * Checks if a string is empty.
 *
 * An empty string is defined as a string that contains only whitespace characters or is a null string.
 *
 * @param str The string to be checked.
 * @return `true` if the string is empty, `false` otherwise.
 */
bool is_empty(string str);

/*
 * Frees multiple pointers.
 *
 * This function takes a variable number of pointers and frees each of them.
 *
 * @param num_pointers The number of pointers to free.
 * @param ...          The pointers to be freed.
 */
void free_multiple(int num_pointers, ...);


#endif

