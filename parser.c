#include "parser.h"
#include "helpers.h"
#include "globals.h"
#include "errors.h"


/*
 * Checks if a word is an instruction name
 * Note: The input string is expected to be a trimmed string (leading and trailing whitespace removed).
 *
 * @param word the word to be checked
 *
 * @return true if the word is an instruction, false otherwise
 * */
bool is_instruction(string word);

/*
Checks if a label name is legal.

The function checks if the label meets the following criteria:

The label starts with a letter.
The label contains only letters and numbers.
The label is not a keyword.
Note: The input label is expected to be a trimmed string (leading and trailing whitespace removed).

@param label The label to be checked.

@return true if the label is legal, false otherwise.
*/
bool is_legal_label(string label);

/*
 * Checks if a word is an directive name
 * Note: The input string is expected to be a trimmed string (leading and trailing whitespace removed).
 *
 * @param word the word to be checked
 *
 * @return true if the word is a directive, false otherwise
 * */
bool is_directive(string word);

/*
 * returns the addressing of the operand or an error code.
 * Note: The input string is expected to be a trimmed string (leading and trailing whitespace removed).
 *
 * @param operand a trimmed string containing the operand.
 *
 * @return the addressing (1 ,3 ,5) of the operand or an error code in case of an error.
 * */
int get_addressing(string operand);


/*
 * returns the line type based on the first word of the line.
 *
 * @param firstWord the first word of the line.
 *
 * @return the type of the line (LABEL, INST, DIR , ERROR) or error in case of an error.
 *
 *
 * */
int get_line_type(string firstWord);

int get_directive(string word);

/*
ASTNode *newDirectiveNode(directive dir) {
    ASTNode *node;
    node->type = DIRECTIVE_NODE;
    node->directive = dir;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode *newInstructionNode(opcode opcode) {
    ASTNode *node;
    node->type = INSTRUCTION_NODE;
    node->instruction = opcode;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode *newOperandNode(operand operand) {
    ASTNode *node;
    node->type = OPERAND_NODE;
    node->operand = operand;
    node->left = NULL;
    node->right = NULL;
    return node;
}
*/
enum states {
    START,
    LABEL,
    INST,
    DIR,
    OP,
    COMMA,
    END,
    ERROR
};


#define TWO_OP_FORMAT  " s , s"
#define ONE_OP_FORMAT " s "
#define STRING_DIR_FORMAT " \"s\" "
#define DATA_DIR_FORMAT " d , d"


bool is_instruction(string word) {
    int i;
    for (i = 0; i < 16; ++i) {
        if (strcmp(word, instructions[i].name) == 0) {
            return true;
        }
    }
    return false;
}

bool is_legal_label(string label) {
    int i;
    /*label doesnt start with a letter*/
    if (!isalpha(label[0])) {
        printf("label doesnt start with a letter\n");
        return false;
    }
    /*check if label contains only letters and numbers*/
    for (i = 1; i < strlen(label); i++) {
        if (!(isalpha(label[i]) || isdigit(label[i]))) {
            printf("label doesnt contain only letters and numbers\n");
            return false;
        }
    }
    /*check if the label is a keyword*/
    return !is_keyword(label);
}

int read_string(string line, directive *dir, int *index) {
    /* int i,j;
     string str;
     bool inString = false;
     BLOCK_ALLOCATION(str, char, LINE_LENGTH - (strlen(".string ") + 1));
     CHECK_NULL(str, system_error(PRINT_ALLOCATION_FAILED_MSG;));
     for (i = 0,j=0; i < strlen(line); i++) {
         if (inString){
             if (str[i]!='"'){
                 str[j]=str[i];
                 j++;
             } else{

             }
         }
     }*/
    /*
     string str = NULL, lineCopy = strduplicate(line);
     char  *start, *end;
     trim(lineCopy);
     if(lineCopy[0]!='"') {
         free(lineCopy);
         return illegal_word_err;
     }
     start=lineCopy+1;
     if(end = strchr((lineCopy+1),'"')){
         BLOCK_ALLOCATION(str,char ,end-start+1);
         CHECK_NULL(str, system_error(PRINT_ALLOCATION_FAILED_MSG;));
         memcpy(str,lineCopy,end-start+1);
         str[end-start]='\0';
         strcpy(dir->content.str,str);
         free(str);
     } else
         return missing_quote_err;


 */

    static const int max_str_len = LINE_LENGTH - strlen(".string") -1;/*the maximum length for the string( length of the line - length of the directive - 2 quotes + null terminator*/
   /* char buffer[max_str_len];
    printf("read string: %s\n",line);
    dir->content.str = NULL;
    if (sscanf("\"%[^\"]\"", buffer) == 1) {
        printf("format good\n");
        BLOCK_ALLOCATION(dir->content.str, char, strlen(buffer) + 1);
        CHECK_NULL(dir->content.str, system_error(PRINT_ALLOCATION_FAILED_MSG;));
        strcpy(dir->content.str, buffer);

    } else {
        return missing_quote_err;
    }
    return 0;*/
   if((line+*index)[0]!='"'){
       return missing_quote_err; /*unquoted text at the start*/
   }
        const char* start = strchr((line+*index), '\"');
        if (start == NULL) {
            return missing_quote_err;  /* Opening double quote not found*/
        }
        start++;  /* Move past the opening double quote*/
        const char* end = strchr(start, '\"');
        if (end == NULL) {
            return missing_quote_err;  /* Closing double quote not found*/
        }

        size_t length = end - start;
         dir->content.str = malloc(length + 1);
    CHECK_NULL(dir->content.str, system_error(PRINT_ALLOCATION_FAILED_MSG;));

        strncpy( dir->content.str, start, length);
        ( dir->content.str)[length] = '\0';
        *index += length+2;


        return 0;
    }



bool is_directive(string word) {
    return (strcmp(word, ".string") == 0 || strcmp(word, ".data") == 0 || strcmp(word, ".entry") == 0 ||
            strcmp(word, ".extern") == 0);
}

int get_addressing(string operand) {
    int i;
    /*check for 2 operands without a comma*/
    for (i = 0; i < strlen(operand); ++i) {
        if (isspace(operand[i])) {
            return missing_comma_err;
        }
    }
    /*check for a register*/
    if (operand[0] == '@') {
        if (operand[1] == 'r' && (operand[2] >= '0' && operand[2] <= '7') && strlen(operand) == 3) {
            return register_addressing;
        } else {
            return illegal_register_err;
        }
    } else {
        /*check for a label*/
        if (is_legal_label(operand)) {
            return direct_addressing;
        } else {
            /*check for a number*/
            if (operand[0] == '+' || operand[0] == '-' || isdigit(operand[0])) {
                for (i = 1; i < strlen(operand); ++i) {
                    if (!isdigit(operand[i])) {
                        if (isspace(operand[i])) {
                            /*a space between operands */
                            return missing_comma_err;
                        } else
                            return unidentified_operand_err;
                    }
                }
                /*is a number*/
                return immediate_addressing;
            } else {
                /*is not a number*/
                return unidentified_operand_err;
            }
        }
    }
}

int get_line_type(string firstWord) {
    int type;
    printf("get_line_type:\n");
    if (firstWord[strlen(firstWord) - 1] == LABEL_END) {
        /*check if the label name is legal*/
        string labelName = strduplicate(firstWord);
        CHECK_NULL(labelName, system_error(PRINT_ALLOCATION_FAILED_MSG;));

        labelName = removeChar(labelName, ':');
        printf("label name: %s\n", labelName);
        if (is_legal_label(labelName)) {
            printf("line type: label\n");
            type = LABEL;
        } else {
            printf("line type: error\n");
            type = ERROR;
        }
        free(labelName);

    } else if (firstWord[0] == '.') {
        if (is_directive(firstWord)) {
            printf("line type: dir\n");
            type = DIR;
        } else {
            printf("line type: error\n");
            type = ERROR;
        }
    } else if (is_instruction(firstWord)) {
        printf("line type: inst\n");
        type = INST;
    } else {
        printf("line type: error\n");
        type = ERROR;
    }
    return type;
}

int get_directive(string word) {
    if (strcmp(word, ".string") == 0)
        return string_dir;
    if (strcmp(word, ".data") == 0)
        return data_dir;
    if (strcmp(word, ".entry") == 0)
        return entry_dir;
    if (strcmp(word, ".extern") == 0)
        return extern_dir;
    else
        return illegal_word_err;
}

/*free: lineCopy, firstWord, cmd*/
command *line_to_struct(code_line line) {
    int state, i, line_type, num_of_operands, code=0;
    string lineCopy, firstWord;
    command *cmd;
    const instruction_data *instructionData;
    /*memory allocations*/
    BLOCK_ALLOCATION(lineCopy, char, strlen(line.content) + 1);
    CHECK_NULL(lineCopy, system_error(PRINT_ALLOCATION_FAILED_MSG;));
    BLOCK_ALLOCATION(firstWord, char, MAX_LABEL_LENGTH + 2); /*the maximum length */
    CHECK_NULL(firstWord, free(lineCopy); system_error(PRINT_ALLOCATION_FAILED_MSG;));
    BLOCK_ALLOCATION(cmd, command, 1);
    CHECK_NULL(cmd, free(lineCopy); free(firstWord); system_error(PRINT_ALLOCATION_FAILED_MSG;));
    /*initialize values*/
    cmd->label = NULL;
    strcpy(lineCopy, line.content);
    state = START;
    i = 0;
    while (true) {
        char ch = lineCopy[i];
        /*skip spaces*/
        if (isblank(ch)) {
            i++;
            continue;
        }
        if (code != 0)
            state = ERROR;
        switch (state) {
            /*start of the command (before or after label name)*/
            case START:
                printf("state start\n");

                if (sscanf(lineCopy + i, "%s", firstWord) == 1) {
                    printf("first word is: %s\n", firstWord);
                    i += strlen(firstWord);
                    state = get_line_type(firstWord);
                    /*in case the first word is not label, instruction or directive*/
                    if(state==ERROR)
                    code = illegal_word_err;
                }
                break;
                /*label definition*/
            case LABEL:
                /*save the label name;*/
                printf("state label\n");
                cmd->label = removeChar(firstWord, ':');
                state = START;
                break;

            case INST:
                cmd->type = inst;
                instructionData = get_instruction_by_name(firstWord);
                cmd->command.instruction.opcode = instructionData->opcode;
                num_of_operands = instructionData->num_of_operands;
                printf("line[i]=%c\n", lineCopy[i]);
                state = OP;
                break;
            case OP:
                code = get_operands(lineCopy, num_of_operands, &(cmd->command.instruction), NULL);
                    state = END;
                break;
            case DIR:
                cmd->type = dir;
                code = get_directive(firstWord);
                switch (code) {
                    case string_dir:
                        cmd->command.directive.type = string_dir;
                        code = read_string(lineCopy, &(cmd->command.directive),&i);
                        break;
                    case data_dir:
                        cmd->command.directive.type = data_dir;
                        break;
                    case entry_dir:
                        cmd->command.directive.type = entry_dir;

                        break;
                    case extern_dir:
                        cmd->command.directive.type = extern_dir;
                        break;
                    default:
                        state = ERROR;
                        break;
                }
                state = END;
                break;
            case ERROR:
                print_error(code, line);
                free_multiple(3, lineCopy, firstWord, cmd);
                return NULL;
            case END:
                printf("end\n");
                if((is_empty(lineCopy+i))) {
                    printf("freeing\n");
                    free(lineCopy);
                    free(firstWord);
                   /* free_multiple(2, lineCopy, firstWord);*/
                    return cmd;
                } else {
                    code = extra_text_in_line_err;
                }
                break;


            default:
                printf("default\n");
                exit(EXIT_FAILURE);
        }


    }
    return cmd;
}


int get_operands(string line, int num_of_operands, instruction *inst, int *index) {
    int addressing, source_length = 0, target_length = 0, space_length = 0;
    string lineCopy;
    lineCopy = strduplicate(line+*index);
    trim(lineCopy);
    printf("length of line after trim: %d\n", strlen(lineCopy));
    /*check for empty string*/
    if (lineCopy[0] == '\0') {
        free(lineCopy);
        return missing_operand_err;
    }
    if (num_of_operands == 2) {
        operand source_op;
        string source_content;
        if (strchr(lineCopy, ',') == NULL) {
            free(lineCopy);
            return missing_comma_err;
        }
        source_length = strcspn(lineCopy, ",");
        space_length = 1;
        printf("source length: %d\n", source_length);
        BLOCK_ALLOCATION(source_content, char, source_length + 1);
        CHECK_NULL(source_content, system_error(PRINT_ALLOCATION_FAILED_MSG;));
        strncpy(source_content, lineCopy, source_length);
        source_content[source_length] = '\0';
        trim(source_content);
        printf("source_op:%s\n", source_content);
        addressing = get_addressing(source_content);
        switch (addressing) {
            case immediate_addressing:
                source_op.type = immediate_addressing;
                source_op.value.value = atoi(source_content);
                break;
            case direct_addressing:
                source_op.type = direct_addressing;
                strcpy(source_op.value.label.name, source_content);
                break;
            case register_addressing:
                source_op.type = register_addressing;
                source_op.value.reg = REGISTER_VALUE(source_content);
                break;
            default:
                free(source_content);
                free(lineCopy);
                return addressing;

        }
        if (!is_legal_addressing(inst->opcode, source_op, false)) {
            free(source_content);
            free(lineCopy);
            return illegal_addressing_err;
        } else {
            inst->source_operand = source_op;
            num_of_operands--;
        }
        free(source_content);
    }
    if (num_of_operands == 1) {
        string target_content;
        operand target_op;
        int i;
        BLOCK_ALLOCATION(target_content, char, MAX_LABEL_LENGTH + 1);
        CHECK_NULL(target_content, system_error(PRINT_ALLOCATION_FAILED_MSG;));
        printf("target operand:\nlineCopy:%s\n", lineCopy);
        /*Find the start of the target_content word by skipping spaces*/
        i = source_length + 1;
        while (lineCopy[i] != '\0' && isspace(lineCopy[i])) {
            i++;
        }

        /* Determine the number of spaces before the target_content*/
        space_length = i - (source_length + 1);
        if (sscanf((lineCopy + source_length + space_length + 1), "%s", target_content) != 1) {
            free(lineCopy);
            return missing_operand_err;
        }
        target_length = strlen(target_content);
        trim(target_content);
        addressing = get_addressing(target_content);
        printf("target_op:%s\n", target_content);
        switch (addressing) {
            case immediate_addressing:
                target_op.type = immediate_addressing;
                target_op.value.value = atoi(target_content);
                break;
            case direct_addressing:
                target_op.type = direct_addressing;
                strcpy(target_op.value.label.name, target_content);
                break;
            case register_addressing:
                target_op.type = register_addressing;
                target_op.value.reg = REGISTER_VALUE(target_content);
                break;
            default:
                free(target_content);
                free(lineCopy);
                return addressing;

        }
        if (!is_legal_addressing(inst->opcode, target_op, true)) {
            free(lineCopy);
            free(target_content);
            return illegal_addressing_err;
        } else {
            inst->target_operand = target_op;
            num_of_operands--;
        }
        free(target_content);
    }
    *index += source_length + target_length + space_length + 1;

    free(lineCopy);

    return 0;

}
