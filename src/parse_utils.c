#define _GNU_SOURCE
#include <string.h>
#include <unistd.h>
#include <stdlib.h>



void replace_char(char* mut_str, char old_chr, char new_chr) {
    int len = strlen(mut_str);
    for (int i = 0; i < len+1; i++) {
        if (mut_str[i] == old_chr) {
            mut_str[i] = new_chr;
        }
    }
}


#define INIT_COUNT_VAL 1

int parse_to_argv_and_argc(const char* input_text, int* argc, char*** argv) {
    int count = INIT_COUNT_VAL;
    char* input_text_copy = strdup(input_text);
    char* match = strtok(input_text_copy, " ");
 // First, count the number of arguments
    while(match != NULL){
        count++;
        match = strtok(NULL, " ");
    }
    free(input_text_copy);
    input_text_copy = NULL;

    *argv = malloc(sizeof(char*) * (count+1));
    (*argv)[count] = NULL;

    if (count > INIT_COUNT_VAL-1){
        int i = 0;
        input_text_copy = strdup(input_text);
        match = strtok(input_text_copy, " ");
        do{
            (*argv)[i++] = strdup(match);
            match = strtok(NULL, " ");
        } while(match != NULL);
     }
    if (input_text_copy != NULL) {
        free(input_text_copy);
        input_text_copy = NULL;
    }

    *argc = count-1;
    return 0;
}
