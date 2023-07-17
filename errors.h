

#ifndef MAMAN14_ERRORS_H
#define MAMAN14_ERRORS_H
#include "globals.h"
/*system error messages (not assembly errors)*/
#define PRINT_ALLOCATION_FAILED_MSG fprintf(stderr, "Memory allocation failed at %s:%d\n", __FILE__, __LINE__)
#define PRINT_FOPEN_FAILED_MSG(filename) fprintf(stderr, "Failed to open file: %s\n", filename)

/*system error (memory allocation failed)*/
#define system_error(action) \
do{\
      action                  \
      exit(EXIT_FAILURE);     \
      }while(0);


 enum error_codes{
    extra_text_in_line_err=300,/*random number*/
    macro_is_keyword_err,
    missing_macro_name_err,
    illegal_word_err,
    illegal_register_err,
     pre_processor_failed,
     unidentified_operand_err,
     missing_comma_err,
     illegal_addressing_err,
     missing_operand_err,
     missing_quote_err

};


typedef struct error{
    unsigned int err_code;
    string err_message;
    string file;
   unsigned int line;
}error;

/*
 * Print an error message based on the error code.
 * Handles only errors in the assembly code.
 *
 * This function takes an error code and prints the corresponding error message along with the file name and line number
 * where the error occurred.
 *
 * @param err_code  The error code indicating the type of error. Legal values are defined in the 'error' enumeration.
 * @param file      The name of the file where the error occurred.
 * @param line      The line number where the error occurred.
 */
void print_error(unsigned int err_code, code_line lineData);

#endif
