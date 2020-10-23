#ifndef _PARSERDEF_
#define _PARSERDEF_

#include <stdio.h>
#include <stdlib.h>

#define total_token_types 51
//#define total_token_types 100

//List of all possible token types
typedef enum{
	PROGRAM, SZ, OP, CL, COP, CCL, END, SQOP, SQCL, DOTS, EPSILON, COLON, SEMICOLON, ID, IDB, NUM, BOOL, INT, BOOLEAN, REAL, ARRAY, JAGGEDARRAY, DEC, DECL, OFINT, R, VALUES, EQUALS, PLUS, MINUS, MUL, DIV, AND, OR,DOLLAR 
}Tokentype;



//The value corresponding to a token can either be integer or real
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Real cannot be hard coded, is this needed???
typedef union{
	int num;
	float r_num;
}Value;


struct token{
	Tokentype type; 
	int valueType; //Valuetype: 0 for int, 1 for real, -1 for other, -2 for ERROR
	int line_no; //Line number of the token
	char* lexeme; //Actual Lexeme
	Value* value; //For integers and real numbers
};

typedef struct token Token;


//Total number of nonterminals
#define TOTAL_NON_TERMINALS 39

//Actual total terminals + $ will be the number of columns in the parse table -- so 40(actual terminals) + 1
#define TOTAL_TERMINALS 41


#define MAX_SYMBOL_SIZE 50

// what is bit vector?? 
#define BIT_VECTOR_SIZE (sizeof(int)-1+ TOTAL_TERMINALS)/sizeof(int)

//Terminal symbols are nothing but the token types
typedef Tokentype Terminal;


typedef enum{
program, stmts, stmt, more_stmts, decl, assign, var_list, type, primitive, array, rectangular, jagged, arr_dims, arr_dim, range, low, high, rows_dec_R1, rows_dec_jR1, op_dim, rowjj, rowj, expression, expression_arith, expression_bool, term, term_bool, factor, factor_bool, ind_list, more_ind_list, remaining_var, more_rowj, more_rowjj, array_op, op_no_op_or, op_no_op_and, op_plus_minus, op_mul_div
}NonTerminal;


//Either terminal or non-terminal
typedef union{
	Terminal terminal;
	NonTerminal nonterminal;
}Symbol_node_type;


struct symbol_node{
	Symbol_node_type type; //Type of the Node -- will hold either a terminal value or a non terminal 
	int term_or_nonterm; //0 for Terminal and 1 for Non Terminal

	struct symbol_node* next; //Pointer to the next node in the list
};

typedef struct symbol_node Symbol_node;


//A particular rhs of a rule is a list of symbols i.e terminals and non-terminals
struct symbol_list{
	Symbol_node* head;
	int length; 
};

typedef struct symbol_list Symbol_list;


//RHS of a non terminal can have multiple Rules , hence a linked list of rules.
struct rule{
	Symbol_list* symbols; //One rule (i.e RHS of a rule) can have terminals and nonterminals both
	struct rule* next; //Pointer to next Rule for that non-terminal
	int rule_no;
};
typedef struct rule Rule;


//One non terminal can have multiple RHS (rules) like A ==> aB | c | dEf
struct rules{
	Rule* head; //A linked list of rules for a non-terminal
	int length; //Number of rules for a particular non-terminal
};

typedef struct rules Rules;


//Grammar as an array of rules
struct grammar{
	int no_of_rules;
	Rules** rules; 	
};

typedef struct grammar Grammar;

//BIT Vectors for first and follow sets
struct firstAndFollow{
	int** first;
	int** follow;
}; 

typedef struct firstAndFollow FirstAndFollow;



//Each cell of a parsing table either points to NULL or to a rule of the grammar
struct cell{	
	
	//LHS of the rule
	int non_term_index;

	//Pointer to the RHS of the rule
	Rule* rule;
};

typedef struct cell Cell;

struct parsingTable{
	Cell*  cells[TOTAL_NON_TERMINALS][TOTAL_TERMINALS];
};

typedef struct parsingTable ParsingTable;









#endif



