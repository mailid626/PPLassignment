
#include <stdlib.h>
#include <stdio.h>
// #include "g.h"

typedef enum{
	PROGRAM, SZ, OP, CL, COP, CCL, SQOP, SQCL, DOTS, EPSILON, COLON, SEMICOLON, ID, IDB, NUM, INT, BOOLEAN, REAL, ARRAY, JAGGED, DEC, LIST, OF, VARIABLES, R, VALUES, EQUALS, PLUS, MINUS, MUL, DIV, AND, OR,DOLLAR 
}Terminal;


char* TerminalMap[] = {
	"PROGRAM", "SZ", "OP", "CL", "COP", "CCL", "SQOP", "SQCL", "DOTS", "EPSILON", "COLON", "SEMICOLON", "ID", "IDB", "NUM", "INT", "BOOLEAN", "REAL", "ARRAY", "JAGGED", "DEC", "LIST", "OF", "VARIABLES", "R", "VALUES", "EQUALS", "PLUS", "MINUS", "MUL", "DIV", "AND", "OR","DOLLAR"
};


char* Type[] = {
	"program", "size", "(", ")\n", "{", "}", "[", "]", "...", "EPSILON", ":", ";", "ID", "IDB", "NUM", "integer", "boolean", "real", "array", "jagged", "declare", "list", "of", "variables", "R1", "values", "=", "+", "-", "*", "/", "&&&", "|||","$"
};
struct token_s 
{
    int line_number;
    char token_name[50];
    char * lexeme;
    Terminal type;
    struct token_s * next;
}; 
typedef struct token_s Token_s;

typedef struct
{
    struct token_s * head;
    struct token_s *tail;
}tokenstream;

void get_token(Token_s * temp );
void push(char *  str, tokenstream * s);
void printlinklist(Token_s * temp);
int isInt(char*s);
int set_token(Token_s * token);
void findmapping(Token_s * st);
char * findnewline(char *s);


