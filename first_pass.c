
#include "first_pass.h"
#include "helpers.h"
#include "parser.h"


bool first_pass(FILE *source, int *IC, int *DC, string *instruction_image, string *data_image, struct hashtable *symbol_table, code_line *line) {
line->lineNum=0;
*IC=0;
*DC=0;
label *temp_label;
    while (fgets(line,LINE_LENGTH,stdin)){
        command *cmd = line_to_struct(*line);
        if(cmd==NULL)
            continue;
        if(cmd->label!=NULL){
          temp_label = search(symbol_table,cmd->label);
            if(temp_label){
                temp_label->address=*IC;

            }
        }
    }
}