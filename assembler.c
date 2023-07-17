#include <stdio.h>
#include "pre_processor.h"
#include "binary.h"
#include "parser.h"
#include "errors.h"
#include "helpers.h"
#include "first_pass.h"

/*hello;j;lj*/
bool assemble(string sourceName);
int main(int argc, char* argv[]){
command *cmd;
label lbl;
operand src, dst;
int code, num_of_ops=2;
instruction inst;
directive dir;
code_line line;
string str1 ,str2 ,str3;
line.lineNum=2;
line.file="file";
line.content = malloc(LINE_LENGTH);
lbl.name;
inst.target_operand.value.label=lbl;
    fgets(line.content,80,stdin) ;
    cmd = line_to_struct(line);
    if(cmd!=NULL) {
        dir = cmd->command.directive;
        printf("%s\n", cmd->command.directive.content.str);
        free( cmd->command.directive.content.str);
    }
    free(line.content);
    free(cmd);
        /*    = cmd->command.instruction;
    printf("opcode: %d\n source op: %d %d\n target op: %d %d\n",inst.opcode,inst.source_operand.type,inst.source_operand.value.value,inst.target_operand.type,inst.target_operand.value.reg);*/
   /* line_to_struct(line);*/
/*if(code==0){
    if(num_of_ops==2)
    printf("instruction: %d-\n source op: type:%d value: %d\ntarget op: type: %d- value: %d\n",inst.opcode,inst.source_operand.type,inst.source_operand.value.value,inst.target_operand.type,inst.target_operand.value.reg);
    else if(num_of_ops==1)
        printf("instruction: %d\n target op: type: %d value: %d\n",inst.opcode,inst.target_operand.type,inst.target_operand.value.reg);

} else
    print_error(code,line);*/

    return 0;
}

bool assemble(string sourceName){

    code_line line;
    string am_file_name;
    FILE *am_file;
    int IC , DC;
    string *data_image, *instruction_image;
    label *symbol_table;
    am_file_name = pre_process(sourceName);
    CHECK_NULL(am_file_name, printf("build failed\n"); printf("freed am_file_name\n"); return false;);
    printf("pre processed file: %s\n",am_file_name);

   /* am_file = fopen(am_file_name,"r");
    CHECK_NULL(am_file, PRINT_FOPEN_FAILED_MSG(am_file_name); return false;);
    BLOCK_ALLOCATION(line.content,char , LINE_LENGTH);
    CHECK_NULL(line.content, system_error(PRINT_ALLOCATION_FAILED_MSG;));
    line.file = am_file_name;
    line.lineNum=0;
    first_pass(am_file, &IC, &DC, &data_image, &instruction_image, &symbol_table, &line);*/
    free(am_file_name);
    return true;

}

