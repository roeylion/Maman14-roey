

#include "errors.h"
#include "helpers.h"

void print_error(unsigned int err_code, code_line lineData) {
    static error errors[] = {{extra_text_in_line_err,   "extra text after command"},
                             {macro_is_keyword_err,     "illegal macro name"},
                             {missing_macro_name_err,   "macro without name"},
                             {pre_processor_failed,     "pre processor failed"},
                             {illegal_word_err,         "unidentified command"},
                             {illegal_register_err,     "no such register"},
                             {unidentified_operand_err, "unidentified operand"},
                             {missing_comma_err,        "expected comma after operand"},
                             {illegal_addressing_err,   "incompatible operand for instruction"},
                             {missing_operand_err,      "excepted operand"},
                             {missing_quote_err,        "expected quoted string"},

    };
    int i;
    for (i = 0; i < ARRAY_LENGTH(errors); ++i) {
        if (err_code == errors[i].err_code) {
            fprintf(stderr, "%s:%d: error: %s\n%s\n", lineData.file, lineData.lineNum, errors[i].err_message,
                    lineData.content);
            return;
        }
    }
}

