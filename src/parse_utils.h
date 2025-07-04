#ifndef __PARSE_UTILS_H__
#define __PARSE_UTILS_H__

void replace_char(char* mut_str, char old_chr, char new_chr);
int parse_to_argv_and_argc(const char* input_text, int* argc, char*** argv);
#endif //__PARSE_UTILS_H__
