

#ifndef MAMAN14_FIRST_PASS_H
#define MAMAN14_FIRST_PASS_H

#include <stdio.h>
#include "globals.h"
#include "table.h"

bool first_pass(FILE *source, int *IC, int *DC, string *instruction_image, string *data_image, struct hashtable *symbol_table,
                code_line *line);



#endif
