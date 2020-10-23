#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include"g.h"

int main(int argc, char* argv[]){

	

	char* grammar_file = "grammar_new.txt";
	Grammar* grm = load_grammar_from_text_file(grammar_file);
	printf("-------Grammar loaded into the data structure-------\n\n");
	print_grammar(grm);
	
	printf("\nrun first and follow\n");
	FirstAndFollow* sets = ComputeFirstAndFollowSets(grm);
	//FirstAndFollow* sets = ComputeFirstAndFollowSets(grm);
	printf("\n\n-------First and Follow Sets Created-------\n\n");

	ParsingTable* table = initialize_Table();
	createParseTable(grm,sets,table);
	printf("-------Parsing Table Created-------\n\n");
	print_parsing_table(table);
	
}
