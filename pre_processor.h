
#ifndef PREPROCESSOR_H

#define PREPROCESSOR_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "helpers.h"
/*macro definitions*/




/*
 * Generates an .am file containing the code after macro expansion and comment removal.
 * checks for following errors only: 1. illegal macro name, 2.macro without name, 3. extra text after the macro name in the definition (mcro m1 extratext)
 *
 * @param src: The name of the source file to be preprocessed.
 *
 * @return the name of the preprocessed file or NULL if an error was detected.
 */
string pre_process(string src);
#endif
