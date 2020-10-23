#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include"g.h"



//Mappings of ENUM to String
char* TerminalMap[] = {
	"PROGRAM", "SZ", "OP", "CL", "COP", "CCL", "END", "SQOP", "SQCL", "DOTS", "EPSILON", "COLON", "SEMICOLON", "ID", "IDB", "NUM", "BOOL", "INT", "BOOLEAN", "REAL", "ARRAY", "JAGGEDARRAY", "DEC", "DECL", "OFINT", "R", "VALUES", "EQUALS", "PLUS", "MINUS", "MUL", "DIV", "AND", "OR","DOLLAR"
};

char* NonTerminalMap[] = {
    "program", "stmts", "stmt", "more_stmts", "decl", "assign", "var_list", "type", "primitive", "array", "rectangular", "jagged", "arr_dims", "arr_dim", "range", "low", "high", "rows_dec_R1", "rows_dec_jR1", "op_dim", "rowjj", "rowj", "expression", "expression_arith", "expression_bool", "term", "term_bool", "factor", "factor_bool", "ind_list", "remaining_var", "more_rowj", "more_rowjj", "array_op"
};




//PART 1

/*************************************************************************************************************************/

//Functions to load grammar from text file into the data structure




//Loads grammar from text file into the corresponding data structure -- assuming the grammar is writtern in the mentioned format
Grammar* load_grammar_from_text_file(char* grammar_text_file){

	int next_rule_no = 1;

	FILE* input = fopen(grammar_text_file, "r");
    printf("Loaded");
	if(input==NULL){
		fprintf(stderr, "Error Opening Grammar File\n");
		return NULL;
	}

	int end_of_file = 0;

	Grammar* grm = initialize_grammar();
    printf("\n Init");

	char ch;
    int i =1;
	while(1){

		if(end_of_file==1)
			return grm;

		//Waiting for LHS of a rule
		while((ch = fgetc(input))!='<'){
			//If no new rule is available
			if(ch==EOF){
				return grm;
			}
		}
		
		//Load a new rule
        printf("\n Loading new rule %d", i);
        i++;
		char* non_term_str = (char*)malloc(sizeof(char)*MAX_SYMBOL_SIZE);
		int i = 0;
		while((ch = fgetc(input))!='>'){
			non_term_str[i++] = ch;
		}
		non_term_str[i] = '\0';


		//Load LHS of the rule -- the corresponding enum
		NonTerminal non_term  =  find(non_term_str,1);
		//Now look for RHS of the rule
		Rule* new_rule = initialize_rule(&next_rule_no);
		Symbol_list* list = initialize_symbols_list();
		Symbol_node* curr_node = list->head;
        printf("\n Line 1");

		while(1){
			if(end_of_file==1)
				break;

			ch = fgetc(input);

			while(ch==' '|| ch == '\t' || ch== '=' || ch== '>'){
				ch = fgetc(input);
			}

			//For new line, new rule begins
			if(ch == '\n')
				break;

			//If EOF, notify not to read further for new rules
			else if(ch == EOF){
				end_of_file = 1;
				break;
			}

			else if(ch>='A'&&ch<='Z' || ch=='e'){
				
				char* term_str = (char*)malloc(sizeof(char)*MAX_SYMBOL_SIZE);
				int i = 0;
				term_str[i++] = ch;
				ch = fgetc(input);
				while(ch>='A'&&ch<='Z'){
					term_str[i++] = ch;
					ch = fgetc(input);
				}

				term_str[i] = '\0';
								
				Symbol_node* sym_node = create_symbol(find(term_str,0), 0);

				//Adding Symbol to the list
				curr_node = add_symbol_to_symbol_list(list, sym_node, curr_node);
				

				if(ch == EOF || ch == '\n'){
					if(ch==EOF)
						end_of_file = 1;
					break;
				}
			}

			else if(ch=='<'){
				
				char* non_term_str = (char*)malloc(sizeof(char)*MAX_SYMBOL_SIZE);
				int i = 0;
				while((ch = fgetc(input))!='>'){
					non_term_str[i++] = ch;
				}

				non_term_str[i] = '\0';
				
				Symbol_node* sym_node = create_symbol(find(non_term_str,1), 1);

				//Adding Symbol to the list
				curr_node = add_symbol_to_symbol_list(list, sym_node, curr_node);
				
			}
		}
		
		//Assigning the symbol list to the new_rule
		new_rule->symbols = list;
		
		//Add new rule to the grammar
		add_rule_to_grammar(grm,non_term,new_rule);		
	}
} 


Grammar* initialize_grammar(){
	Grammar* grm = (Grammar*)malloc(sizeof(struct grammar));
	grm -> no_of_rules = 0;

	//Total non-terminals will be equal to total number of rules
	grm -> rules = (Rules**)malloc(sizeof(Rules*)*TOTAL_NON_TERMINALS);

	for(int i = 0; i<TOTAL_NON_TERMINALS;i++){
		grm->rules[i] = (Rules*)malloc(sizeof(struct rules));
		grm->rules[i]->head = NULL;
		grm->rules[i]->length = 0;		
	}

	return grm;
}


Symbol_list* initialize_symbols_list(){
	Symbol_list* list = (Symbol_list*)malloc(sizeof(Symbol_list));
	list->head = NULL;
	list->length = 0;

	return list;
}

Rule* initialize_rule(int* next_rule_no){
	Rule* new_rule = (Rule*)malloc(sizeof(Rule));
	new_rule->next = NULL;
	new_rule->symbols = NULL;

	new_rule->rule_no = *next_rule_no; 
	*next_rule_no = *next_rule_no+1;

	return new_rule;
}

Symbol_node* create_symbol(int enum_int, int term_or_nonterm){
	Symbol_node* sym_node = (Symbol_node*)malloc(sizeof(Symbol_node));
	sym_node->next=NULL;
	Symbol_node_type type;
	if(term_or_nonterm==0){
		type.terminal = (Tokentype)enum_int;
	}
	else{
		type.nonterminal = (NonTerminal)enum_int;
	}
	sym_node->type = type;
	sym_node->term_or_nonterm = term_or_nonterm;

	return sym_node;


}

int find(char* str, int term_or_nonterm){
	
	//Terminal
	if(term_or_nonterm==0){
		for(int i=0;i<total_token_types;i++){
			if(strcmp(str,TerminalMap[i])==0)
				return i;
		}
	}

	//Non terminal
	else{
		for(int i=0;i<TOTAL_NON_TERMINALS;i++){
			if(strcmp(str,NonTerminalMap[i])==0)
				return i;
		}
	}
}


Symbol_node* add_symbol_to_symbol_list(Symbol_list* list, Symbol_node* sym_node, Symbol_node* curr_node){
	
	//If initially the list is empty
	if(curr_node==NULL){
		sym_node->next = list->head;
		list->head = sym_node;
		curr_node = list->head;
	}
	else{
		curr_node->next = sym_node;
		curr_node = curr_node->next;
	}

	list->length++;

	return curr_node;
}

void add_rule_to_grammar(Grammar* grm, NonTerminal non_term, Rule* new_rule){

	new_rule->next = grm->rules[non_term]->head;
	grm->rules[non_term]->head = new_rule;

	//One more Rule added for the non terminal
	grm->rules[non_term]->length++;

	grm->no_of_rules++;
}

void print_grammar(Grammar* grm){
	printf("\n------------------------ Printing Grammar from Data Structure---------------------\n\n");

	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		
		printf("%d.  <%s> ===> ",(i+1), NonTerminalMap[i]);

		Rules* rules = grm->rules[i];

		Rule* temp = rules->head;

		for(int j = 0;j< rules->length;j++){
			if(j!=0)
				printf("| ");

			Symbol_list* symbols = temp->symbols;
			Symbol_node* temp2 = symbols->head;

			for(int k=0;k<symbols->length;k++){
				if(temp2->term_or_nonterm==0)
					printf("%s", TerminalMap[temp2->type.terminal]);
				else
					printf("<%s>", NonTerminalMap[temp2->type.nonterminal]);

				printf(" ");
				temp2 = temp2->next;
			}

			
			temp = temp->next;
		}
		printf("\n");
	}
	printf("\n------------------------ Grammar Over----------------------\n");
	
}

//Functions to load grammar from the text file and printing it are over

/*************************************************************************************************************************/

//PART 2


/*************************************************************************************************************************/

//Functions to Compute the first and follow sets

FirstAndFollow* ComputeFirstAndFollowSets(Grammar* grm){

	printf("\nentercomputer fist and follow\n");
	FirstAndFollow* sets = (FirstAndFollow*)malloc(sizeof(FirstAndFollow));
	printf("\nmemory alocated\n");
	//Initialize sets to empty
	intitialize_sets(sets);	

	//Computer First Sets
	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		if(i<22 && i>22)

		compute_First(grm,(NonTerminal)i, sets->first);
		printf("\n%d\n", i);		
	}
	printf("\nfirsts\n");
	//Compute Follow Sets
	compute_Follow(grm,sets);
	printf("\nfollow\n");
	return sets;
	printf("\nreturn sets\n");

}

void print_first_and_follow_sets(FirstAndFollow* sets){

	printf("\n\n----------------------------Printing First Set--------------------------------\n\n");
	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		printf("%d. %s => ",(i+1),NonTerminalMap[i]);
		print_set(sets->first[i]);
	}	

	printf("\n---------------------------------First Set Over-----------------------------------\n");

	printf("\n\n----------------------------Printing Follow Set--------------------------------\n\n");
	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		printf("%d. %s => ",(i+1),NonTerminalMap[i]);
		print_set(sets->follow[i]);
	}	

	printf("\n---------------------------------Follow Set Over-----------------------------------\n");

}

void print_set(int* set){
	printf("{ ");
	for(int i=0;i<TOTAL_TERMINALS;i++){
		if(checkPresence(set,i)){
			printf("%s ",TerminalMap[i]);
		}
	}
	printf("}\n");
}


void intitialize_sets(FirstAndFollow* sets){

	sets->first = (int**)malloc(TOTAL_NON_TERMINALS*sizeof(int*));
	sets->follow = (int**)malloc(TOTAL_NON_TERMINALS*sizeof(int*));

	
	for(int i=0;i<TOTAL_NON_TERMINALS;i++){

		sets->first[i] = (int*)malloc(BIT_VECTOR_SIZE*sizeof(int));
		sets->follow[i] = (int*)malloc(BIT_VECTOR_SIZE*sizeof(int));

		//Initializing bit vector to 0
		memset(sets->first[i],0,sizeof(sets->first[i]));
		memset(sets->follow[i],0,sizeof(sets->follow[i]));		
	}
}


void compute_First(Grammar* grm, NonTerminal non_term, int** first){

	Rules* rules = grm->rules[non_term];

	Rule* temp = rules->head;

	for(int j = 0;j< rules->length;j++){

		Symbol_list* symbols = temp->symbols;
		Symbol_node* temp2 = symbols->head;

		int k;
		for(k=0;k<symbols->length;k++){

			//If a terminal has occurred
			if(temp2->term_or_nonterm==0){

				addToSet(first[non_term],temp2->type.terminal);				
				break;
			}

			else{
				compute_First(grm, temp2->type.nonterminal,first);

				//if e not in first of the nonterminal
				int whetherChanged = union_sets(first[non_term], first[temp2->type.nonterminal]);

				if(!checkPresence(first[temp2->type.nonterminal], EPSILON)){
					break;
				}
			}			

			temp2 = temp2->next;
		}

		//All traversed
		if(k==symbols->length){			
			addToSet(first[non_term],EPSILON);
		}
		
		temp = temp->next;
	}

}

// returns set1 = set1 union set2

//It does not add epsilon to set1 if it exists in set2
int union_sets(int* set1, int* set2){
	int whetherChanged = 0;
	for(int i=0;i<TOTAL_TERMINALS;i++){
		if(i== (int)EPSILON){
			continue;
		}

		if(checkPresence(set2,i)){
			
			if(!checkPresence(set1,i))
				whetherChanged = 1;
			addToSet(set1,i);			
		}
	}
	return whetherChanged;
}

void compute_Follow(Grammar* grm, FirstAndFollow* sets){
	
	addToSet(sets->follow[program],DOLLAR);
	int whetherChanged = 1;
	while(whetherChanged==1){
		whetherChanged = 0;
		whetherChanged = compute_Follow_Util(grm,sets->first,sets->follow, whetherChanged);
	}
}

int compute_Follow_Util(Grammar* grm, int** first, int** follow, int whetherChanged){
	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		
		Rules* rules = grm->rules[i];

		Rule* temp = rules->head;

		for(int j = 0;j< rules->length;j++){
			
			Symbol_list* symbols = temp->symbols;
			Symbol_node* temp2 = symbols->head;

			for(int k=0;k<symbols->length;k++){

				//See if it's a non_terminal 
				if(temp2->term_or_nonterm==1){

					//Start from the next node
					Symbol_node* temp3 = temp2->next;

					while(temp3!=NULL){

						//If terminal
						if(temp3->term_or_nonterm==0){
							
							if(!checkPresence(follow[temp2->type.nonterminal], temp3->type.terminal))
								whetherChanged = 1;

							
							addToSet(follow[temp2->type.nonterminal], temp3->type.terminal);
							break;
						}
						//If non-terminal
						else{
							whetherChanged = MAX(whetherChanged,union_sets(follow[temp2->type.nonterminal],first[temp3->type.nonterminal]));

							//If epsilon not a part
							if(!checkPresence(first[temp3->type.nonterminal], EPSILON)){
								break;
							}
						}

						temp3 = temp3->next;						
					}

					//If entire rule is traversed
					if(temp3 == NULL){
						whetherChanged = MAX(whetherChanged,union_sets(follow[temp2->type.nonterminal],follow[i]));
					}
				}								

				temp2 = temp2->next;
			}

			temp = temp->next;
		}		
	}
	return whetherChanged;
}

int MAX(int a, int b){
	return (a>b)? a:b;
}


void addToSet(int* bitVector, int i){
	bitVector[i/sizeof(int)] = (bitVector[i/sizeof(int)]) | (1 << (i%sizeof(int)));
}

void removeFromSet(int* bitVector, int i){
	bitVector[i/sizeof(int)] = (bitVector[i/sizeof(int)]) & (~(1 << (i%sizeof(int))));
}

int checkPresence(int* bitVector,  int i){
	return ((bitVector[i/sizeof(int)] & (1 << (i%sizeof(int)) )) != 0 ); 
}




//Functions for computing First and Follow sets are over

/*************************************************************************************************************************/

void createParseTable(Grammar* grm, FirstAndFollow* sets, ParsingTable* table){
	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		Rules* rules = grm->rules[i];

		Rule* temp = rules->head;

		for(int j = 0;j< rules->length;j++){

			Symbol_list* symbols = temp->symbols;
			Symbol_node* temp2 = symbols->head;

			int to_be_continued = 0;

			for(int k=0;k<symbols->length;k++){

				to_be_continued = 0;

				//Terminal
				if(temp2->term_or_nonterm==0){

					//If it is epsilon
					if(temp2->type.terminal==EPSILON){
						for(int x=0;x<TOTAL_TERMINALS;x++){
							
							if(checkPresence(sets->follow[i], x)){							

								if(table->cells[i][x]!=NULL){
									printf("ERROR: Multiple rules clashing in a table-cell <%s> -> %s\n",NonTerminalMap[i],TerminalMap[x]);
								}

								table->cells[i][x] = get_cell(temp,i);
							}
						}

						continue;
					}

					//any other terminal
					else{
						if(table->cells[i][temp2->type.terminal]!=NULL){
							printf("ERROR: Multiple rules clashing in a table-cell <%s> -> %s\n",NonTerminalMap[i],TerminalMap[temp2->type.terminal]);
						}
						table->cells[i][temp2->type.terminal] = get_cell(temp,i);
						break;
					}
				}

				//Non-terminal
				else{
					for(int x=0;x<TOTAL_TERMINALS;x++){

						if(checkPresence(sets->first[temp2->type.nonterminal], x)){

							if(x!=EPSILON){

								if(table->cells[i][x]!=NULL){
									printf("ERROR: Multiple rules clashing in a table-cell <%s> -> %s\n",NonTerminalMap[i],TerminalMap[x]);
								}
								table->cells[i][x] = get_cell(temp,i);
							}

							//If epsilon exists, then continue
							else{
								to_be_continued = 1;
							}
						}
					}
				}

				if(to_be_continued==1)
					continue;
				else
					break;

				temp2 = temp2->next;
			}


			temp = temp->next;
		}

		
	}
}

Cell* get_cell(Rule* rule, int non_term_index){
	Cell* new_cell = (Cell*)malloc(sizeof(Cell));
	new_cell->rule = rule;
	new_cell->non_term_index = non_term_index;

	return new_cell;
}

ParsingTable* initialize_Table(){
	ParsingTable* table = (ParsingTable*)malloc(sizeof(ParsingTable));
	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		for(int j=0;j<TOTAL_TERMINALS;j++){
			table->cells[i][j] = NULL;
		}
	}

	return table;
}

void print_parsing_table(ParsingTable* table){
	printf("\n\n----------------------------Printing Parsing Table--------------------------------\n\n");	

	printf("Note: 'x' corresonds to an empty cell pointing to no rule, whereas 'O' represents a cell pointing to a rule\n\n");

	for(int i=0;i<TOTAL_NON_TERMINALS;i++){
		printf("%2d. %27s : ",(i+1),NonTerminalMap[i]);
		for(int z=0;z<TOTAL_TERMINALS;z++){
			if(table->cells[i][z]!=NULL){
				printf("O ");
				//printf("%d ",table->cells[i][z]->rule->rule_no);
			}
			else
				printf("x ");
			printf(" ");
		}

		printf("\n");
	}	

	printf("\n---------------------------------Parsing Table Over-----------------------------------\n");
}

/************************************************************************************************************************/








