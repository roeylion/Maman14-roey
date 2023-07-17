#include "pre_processor.h"
#include "errors.h"

#define MACRO_START "mcro"
#define MACRO_END "endmcro"
#define MAX_MACRO_NAME_SIZE 30
#define DEFAULT_MACRO_CAPACITY 5
#define COMMENT_CHAR ';'


struct macro {
    string name;
    string content;
};

/*
 * Finds a macro by name in the given macro list.
 *
 * @param name: The name of the macro to find.
 * @param macroList: An array of macro structures representing the macro list.
 *
 * @return The index of the macro in the list if found, or -1 if not found.
 */
int find_macro(string name, macro *macroList);

void free_macro_list(macro *macroList, int count);

string pre_process(const string src) {
    /*declarations*/
    int i, wordCount = 0;
    string *wordsInLine;
    string destName, sourceName;
    int macroCount = 0, macroCapacity = DEFAULT_MACRO_CAPACITY, currentMacro;
    bool inMacro = false, errors_found = false;
    FILE *source, *dest;
    code_line line;

    /*memory allocations*/
    BLOCK_ALLOCATION(macro *macroList, macro, macroCapacity);
    CHECK_NULL(macroList, PRINT_ALLOCATION_FAILED_MSG; return NULL;);
    BLOCK_ALLOCATION(sourceName, char, strlen(src) + 4); /* allocate memory for the name + .as extension*/
    CHECK_NULL(sourceName, PRINT_ALLOCATION_FAILED_MSG; free(macroList); return NULL;);
    BLOCK_ALLOCATION(destName, char, strlen(src) + 4);/* allocate memory for the name + .am extension*/
    CHECK_NULL(destName, PRINT_ALLOCATION_FAILED_MSG; free(macroList); free(sourceName); return NULL;);
    BLOCK_ALLOCATION(line.content, char, LINE_LENGTH);
    CHECK_NULL(line.content,
               free(macroList); free(sourceName); free(destName); system_error(PRINT_ALLOCATION_FAILED_MSG;));


    /*set the input and output files name*/
    strcpy(destName, src);
    strcpy(sourceName, src);
    strcat(sourceName, ".as");
    strcat(destName, ".am");

    /*open the files*/
    source = fopen(sourceName, "r");
    CHECK_NULL(source,
               PRINT_FOPEN_FAILED_MSG(sourceName); free(macroList); free(sourceName); free(destName); return NULL;);
    dest = fopen(destName, "w");
    CHECK_NULL(dest, PRINT_FOPEN_FAILED_MSG(destName); free(macroList); free(sourceName); free(destName); free(
            line.content); fclose(
            source); return NULL;);
    macroList[0].name = NULL;
    /*initialize line info*/
    line.file = sourceName;
    line.lineNum = 0;

    /*go over the file line by line*/
    while (fgets(line.content, LINE_LENGTH, source) != NULL) {
        (line.lineNum)++;
        /*start = 0; the first non blank char of the line*/
        /*check if the line exceeds the maximum length*/
        if (line.content[strlen(line.content) - 1] != '\n') {
            /*print an error delete the .am file and return null*/
            fprintf(stderr, "Line exceeded maximum length: %s\n", line.content);
            remove(destName);
            return NULL;
        }
        /* CLEAR_SPACES(line.content, start) ;*/
        trim(line.content);
        /*check for comment or empty line*/
        if (*(line.content /*+ start*/) != COMMENT_CHAR && !IS_END(*(line.content /*+ start*/))) {
            /*split the line to separate words*/
            wordsInLine = split_to_words((line.content /*+ start*/), " ", &wordCount);
            /*put back the newline char for printing*/
            strcat(line.content, "\n");

            /*make sure each word is free of whitespaces*/
            for (i = 0; i < wordCount; ++i) {
                trim(wordsInLine[i]);
            }

            /*if a real macro found*/
            if ((currentMacro = find_macro(wordsInLine[0], macroList)) != -1) {
              /* -d  printf("(currentMacro = find_macro(wordsInLine[0], macroList)) != -1\n");*/
                fprintf(dest, "%s", macroList[currentMacro].content);/*place macro content*/
            }

                /*reading a macro*/
            else if (inMacro) {
                /*check if endmcro reached*/
                if (strcmp(wordsInLine[0], MACRO_END) == 0) {
                    /*stop reading the macro and increase the macro count*/
                    inMacro = false;
                    macroCount++;
                } else {
                    /*add the line the content of the macro*/
                    strcat((*(macroList + macroCount)).content, (line.content /*+ start*/));
                }
            } else {
                /*start of macro definition */
                if (strcmp(wordsInLine[0], MACRO_START) == 0) {
                    /*resize the macro list if needed*/
                    if (macroCount == macroCapacity - 1) {
                        macroCapacity += 5;
                        macroList = (macro *) realloc(macroList, sizeof(macro) * macroCapacity);
                        CHECK_NULL(macroList,
                                   PRINT_ALLOCATION_FAILED_MSG; free(destName); fclose(source); fclose(dest); free(
                                           line.content); free(sourceName); return NULL;);
                    }

                    /*check for case of only mcro without a macro name or extra text after the macro name*/
                    if (wordCount != 2) {
                        if (wordCount > 2)
                            print_error(extra_text_in_line_err, line);
                        else if (wordCount < 2)
                            print_error(missing_macro_name_err, line);
                        errors_found = true;
                    } else {
                        /*check if the macro name is a keyword*/
                        if (is_keyword(wordsInLine[1])) {
                            print_error(macro_is_keyword_err, line);
                            errors_found = true;
                        } else {
                            inMacro = true;
                            /*allocate memory for the name*/
                            BLOCK_ALLOCATION((*(macroList + macroCount)).name, char, MAX_MACRO_NAME_SIZE);
                            CHECK_NULL((*(macroList + macroCount)).name,
                                       PRINT_ALLOCATION_FAILED_MSG; free(macroList); free(destName); fclose(
                                               source); fclose(
                                               dest); return NULL;);
                            /*add the macro name to the list*/
                            strcpy((*(macroList + macroCount)).name, wordsInLine[1]);
                            /*allocate memory and initialize the content*/
                            BLOCK_ALLOCATION((*(macroList + macroCount)).content, char, LINE_LENGTH * 3);
                            CHECK_NULL((*(macroList + macroCount)).content,
                                       PRINT_ALLOCATION_FAILED_MSG;free((*(macroList + macroCount)).name); free(
                                               macroList); free(
                                               destName); fclose(source); fclose(dest); return NULL;);
                            strcpy((*(macroList + macroCount)).content, "");
                            /*mark the last macro */
                            (*(macroList + macroCount + 1)).name = NULL;
                            (*(macroList + macroCount + 1)).content = NULL;
                        }
                    }
                } else /*regular line*/{
                    /*print the line to file without leading spaces*/
                    fprintf(dest, "%s", (line.content /*+ start*/));
                }
            }
          /*-d  printf("about to free\n");*/
            free_string_array(wordsInLine, wordCount);
        }

    }
    free(sourceName);
    free(line.content);
    free_macro_list(macroList, macroCapacity);
    fclose(source);
    fclose(dest);
    if (errors_found) {
        printf("errors in source file\n");
        remove(destName);
        free(destName);
        return NULL;
    }
/*return the name of the .am file*/
    return destName;
}

int find_macro(string name, macro *macroList) {
    int i;
    /*go over the macro list*/
    for (i = 0; (*(macroList + i)).name != NULL; ++i) {
        /*check if the macro name matches and return its index*/
        if (!strcmp(macroList[i].name, name)) {
            return i;
        }
    }

    return -1;
}

void free_macro_list(macro *macroList, int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (macroList[i].name != NULL)
            free(macroList[i].name);
        else break;
        if (macroList[i].content != NULL)
            free(macroList[i].content);
        else break;
    }
    free(macroList);
}

