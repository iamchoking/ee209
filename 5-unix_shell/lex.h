#ifndef LEX_h
#define LEX_h

#include "dynarray.h"

typedef struct Token * token_t;

enum TokenType {PIPE, NORMAL, REDIR_OUT, REDIR_IN};

token_t new_token(enum TokenType type, char* value);
int token_type(token_t token);
char* token_value(token_t token);

void delete_token(token_t token);
void free_token(void* token, void* dummy);
void delete_tokens(DynArray_T tokens);

int print_token(token_t token, char include_type);
void print_token_lame(void* token, void* dummy);
void print_token_lame_with_type(void* token, void* dummy);
void print_tokens(DynArray_T tokens, char include_type);

int lex_line(char* line, DynArray_T tokens, char* programName);

int add_nicely(enum TokenType type, char* value, DynArray_T tokens, char* programName);

#endif