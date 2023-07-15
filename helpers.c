#include "helpers.h"
#include "errors.h"
string *split_to_words(string line, string separator, int *wordCount) {
    int length = strlen(line);
    int wordCapacity = 4, i; /*holds the maximum amount of words, used to extend the size*/
    string *words;
    string temp;
    string token;
    *wordCount = 0;
    BLOCK_ALLOCATION(words, string, wordCapacity);
    CHECK_NULL(words, PRINT_ALLOCATION_FAILED_MSG; return NULL;);
    BLOCK_ALLOCATION(temp, char, length + 1);
    CHECK_NULL(temp, PRINT_ALLOCATION_FAILED_MSG; free(words); return NULL;);
    strcpy(temp, line);
    REMOVE_NEWLINE(temp);
    token = strtok(temp, separator);
    /* walk through other tokens */
    while (token != NULL) {
        if (*wordCount == wordCapacity) {
            wordCapacity += 10;/*add 10 possible words */
            words = (string *) realloc(words, sizeof(string) * wordCapacity);
            CHECK_NULL(words, PRINT_ALLOCATION_FAILED_MSG; free(temp); return NULL;);
        }
        BLOCK_ALLOCATION(words[*wordCount], char ,(strlen(token) + 1));
        CHECK_NULL(words[*wordCount], system_error(PRINT_ALLOCATION_FAILED_MSG;));
        strcpy(words[*wordCount], token);
        (*wordCount)++;
        token = strtok(NULL, separator);
    }
    free(temp);
  /*-d  printf("splitted line: %s\nit has %d words:\n",line,*wordCount);
    for ( i = 0; i < *wordCount ; ++i) {
        printf("%s\n",words[i]);
    }*/
    words = realloc(words,(*wordCount) * sizeof(string));
   /*-d printf("size of words: %u\n\n", sizeof(words)*(*wordCount));*/
    return words;

}


void free_string_array(string *string_array, int count) {
    int i;
  /*-d  printf("freed words: %d\n\n", count * sizeof(string_array));*/
    for (i = 0; i < count; i++) {
        free(string_array[i]);
    }
    free(string_array);
}


void trim(string str) {
    string start = str;
    string end;

    /*find the first non space character*/
    while (isspace(*start)) {
        ++start;
    }

    /*find the last non space character*/
    end = str + strlen(str) - 1;
    while (end > start && isspace(*end)) {
        --end;
    }

    /*Null-terminate the trimmed string*/
    *(end + 1) = '\0';

    /*Move the trimmed string to the beginning of the original string*/
    if (str != start) {
        memmove(str, start, end - start + 2);
    }
}

bool is_keyword(string word) {
    static const string keywords[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7",
                                      "mov", "cmp", "add", "sub", "not", "clr", "lea",
                                      "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", ".data",
                                      ".string", ".entry", ".extern"};
    static const int keywordCount = ARRAY_LENGTH(keywords);
    int i;
    for (i = 0; i < keywordCount; i++) {
        if (strcmp(word, keywords[i]) == 0) {
        /* -d    printf("word is keyword: %s", keywords[i]);*/
            return true;
        }
    }
    /* -d printf("word is not a keyword\n");*/
    return false;
}


unsigned int get_label_address(string name, struct label labels[], int labelCount) {
    int i;

    for (i = 0; i < labelCount; ++i) {
        if (strcmp(name, labels[i].name) == 0) {
            return labels[i].address;
        }
    }
    return -1;
}


bool isNumberInArray(const int arr[], int size, int number) {
    int i;
    for ( i = 0; i < size; i++) {
        if (arr[i] == number) {
            return true;
        }
    }
    return false;
}



bool is_legal_addressing(opcode opcode, operand operand, bool isTarget) {
    if (!isTarget) {
        return isNumberInArray(instructions[opcode].source_addrressing, ADDRESSING_TYPES, operand.type);
    } else
        return isNumberInArray(instructions[opcode].target_addrressing, ADDRESSING_TYPES, operand.type);
}
/*
string strduplicate(const string str) {
    size_t len = strlen(str) + 1;
    string duplicate = malloc(len);
    printf("strduplicate:\n");
    if (duplicate != NULL) {
       strcpy(duplicate, str);
    }
    printf("%p: %s\n\n",duplicate, duplicate);

    return duplicate;
}*/
string strduplicate(const string str)
{
    string dup;
    size_t size = strlen(str) + 1;

    dup = malloc(size);
    if (dup) {
        memcpy(dup, str, size);
    }
    return dup;
}

int get_opcode (string instruction){
int i;
    for ( i = 0; i < OPCODES ; ++i) {
        if(strcmp(instructions[i].name,instruction)==0){
            return instructions[i].opcode;
        }
    }
    return -1;
}


const instruction_data * get_instruction_by_name(string name){
    int i;
    for(i=0; i<OPCODES; i++){
        if(strcmp(instructions[i].name,name)==0){
            return (instructions+i);
        }
    }
    return NULL;
}




string removeChar(string str, char c) {
    string result;
    size_t len;
    size_t src, dst = 0;
    if (str == NULL)
        return NULL;

   len = strlen(str);
    BLOCK_ALLOCATION(result,char ,(len+1));
    CHECK_NULL(result, system_error(PRINT_ALLOCATION_FAILED_MSG;));
    for ( src = 0; str[src] != '\0'; src++) {
        if (str[src] != c) {
            result[dst] = str[src];
            dst++;
        }
    }
    result[dst] = '\0';

    return result;
}



bool is_empty(string str) {
    int i;
    for (i=0;str[i]!='\0'; i++){
        if(!isspace(str[i]))
            return false;
    }
    return true;
}

void free_multiple(int num_pointers, ...) {
    va_list args;
    int i;
    va_start(args, num_pointers);

    for ( i = 0; i < num_pointers; i++) {
        void* ptr = va_arg(args, void*);
        free(ptr);
    }

    va_end(args);
}
