#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#include "tok.h"

#define TOTAL_NUM_TERMINALS 34

// typedef enum{
// 	PROGRAM, SZ, OP, CL, COP, CCL, SQOP, SQCL, DOTS, EPSILON, COLON, SEMICOLON, ID, IDB, NUM, INT, BOOLEAN, REAL, ARRAY, JAGGED, DEC, LIST, OF, VARIABLES, R, VALUES, EQUALS, PLUS, MINUS, MUL, DIV, AND, OR,DOLLAR 
// }Tokentype;


// char* TerminalMap[] = {
// 	"PROGRAM", "SZ", "OP", "CL", "COP", "CCL", "SQOP", "SQCL", "DOTS", "EPSILON", "COLON", "SEMICOLON", "ID", "IDB", "NUM", "INT", "BOOLEAN", "REAL", "ARRAY", "JAGGED", "DEC", "LIST", "OF", "VARIABLES", "R", "VALUES", "EQUALS", "PLUS", "MINUS", "MUL", "DIV", "AND", "OR","DOLLAR"
// };


// char* Type[] = {
// 	"program", "size", "(", ")", "{", "}", "[", "]", "...", "EPSILON", ":", ";", "ID", "IDB", "NUM", "integer", "boolean", "real", "array", "jagged", "declare", "list", "of", "variables", "R1", "values", "=", "+", "-", "*", "/", "&&&", "|||","$"
// };

// 14 keywords
char KEYWORDS[][100]= { 
    "program","declare","list","of","integer","boolean","real","array","variables","jagged","size","values","integer","R1"  
    };

// 7 operators
char OPERATORS[][100]={
    "+","-","*","/","=","&&&","|||",
};

// 11 symbols 
char SYMBOLS[][100]={
    ":","[","]","(",")","{","}","...","()",";" // removed ;\n
};


// char* tokenMap[] = {"PROGRAM", "SZ", "OP", "CL", "COP", "CCL", "END", "SQOP", "SQCL", "DOTS", "EPSILON", "COLON", "SEMICOLON", "ID", "IDB", "NUM", "BOOL", "INT", "BOOLEAN", "REAL", "ARRAY", "JAGGEDARRAY", "DEC", "DECL", "OFINT", "R", "VALUES", "EQUALS", "PLUS", "MINUS", "MUL", "DIV", "AND", "OR","DOLLAR"};

// char* program_tokens[] = {"program", "size", "(", ")", "{", "}", "}", "[", "]", "...", "EPSILON", ":", ";", "ID", "IDB", "NUM", "BOOL", "INT", "BOOLEAN", "REAL", "array", "jagged array", "declare", "declare list of", "of integer", "R1", "values", "=", "+", "-", "*", "/", "&&&", "|||","$"};


int line_number=0;

void get_token(Token_s * temp ) 
{   
    // 14 keywords
    for(int i=0;i<14;++i)
    {
        if(strcmp(temp->lexeme,KEYWORDS[i])==0)
        {   
            strcpy(temp->token_name,"KEYWORDS");

            return ;
        }
    }
    // for symbols 
    for(int i=0;i<9;++i)
    {
        if(strcmp(temp->lexeme,SYMBOLS[i])==0)
        {
                strcpy(temp->token_name,"SYMBOLS");
                return ;
        }
    }

    // for operators
    for(int i=0;i<7;++i)
    {
        if(strcmp(temp->lexeme,OPERATORS[i])==0)
        {
            strcpy(temp->token_name,"OPERATORS");
            return;
        }
    }

    //  for constants 
    if(temp->lexeme[0]>='0' && temp->lexeme[0]<='9')
    {
        strcpy(temp->token_name,"CONSTANTS");
        return ;
    }
    else
    {
        strcpy(temp->token_name,"IDENTIFIERS");
        return ;
    }
    
}


int findfromTerminalMaptoTerminal(char* str){
	//Terminal
        // printf(" \n FMMMM %s \n ", str);
		for(int i=0;i<TOTAL_NUM_TERMINALS;i++){
            // printf("\n Inside term map i is %d : ",i);
			if(strcmp(str,TerminalMap[i])==0){
    
				return i;}
		}
}


void findmapping(Token_s * st){
    removeChar(st->lexeme, '\n');
    printf("Lexeme is %s \n ", st->lexeme);
    for(int i = 0; i< TOTAL_NUM_TERMINALS; i++){
        // printf("\n Type[i] is : %s and lexeme is : %s \n", Type[i], st->lexeme);
        if(!strcmp(Type[i], st->lexeme)){ // Type and lexeme will be of the same kind 
        
            int j = findfromTerminalMaptoTerminal(TerminalMap[i]);
            st->type = (Terminal)j;
            break;
            // printf("\n Terminal %d %d \n", j, st->type);
        }
        else{
            if(isInt(st->lexeme)){
            st->type = NUM;
        }
            else{
            st->type = ID;
        }
        }
    }
    // if(st->token_name == "IDENTIFIERS" ){
    //     printf("%s \n ", st->token_name);
    //     if(isInt(st->lexeme)){
    //         st->type = NUM;
    //     }
    //     else{
    //         st->type = ID;
    //     }
    // }
}

// char * findnewline(char *s){
//     for(int i=0;s[i]!='\0';i++)
// 	{
// 		if ((s[i]) == '\n') // check 
//             		return 0;
// 	}
// 	return 1;
// }

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void push(char *  str, tokenstream * s)
{
    printf("\n");
    printf("%d ",line_number);
    ++line_number;
    char delim[] = " ";
    char *ptr = strtok(str, delim);
    while( ptr != NULL )
	{
        if(s->head==NULL ) // token stream is just being initialised 
        {
            struct token_s * temp =  (struct token_s  *) malloc( sizeof(Token_s)) ;
            temp->line_number=line_number;
            temp->lexeme= (char * )malloc(strlen(ptr)+1);
            strcpy(temp->lexeme,ptr);
            // printf(" \n SOMETHING $$$$$$$$$$$$$$$ %s", ptr);
            // token_name has the identifier 

            //  retval.key = malloc(strlen(p.key) + 1);
            //  strcpy(retval.key, p.key);
            
            get_token(temp); // adds info to token name                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ;
            temp->next=NULL;
            s->head=temp;
            s->tail=temp;
            findmapping(temp);

        }

        else
        {
            struct token_s * temp =  (struct token_s*)malloc(sizeof(struct token_s));
            temp->line_number=line_number;
            temp->lexeme= (char *)malloc(strlen(ptr)+1);
            strcpy(temp->lexeme,ptr);
            // printf(" \n SOMETHING $$$$$$$$$$$$$$$ %s", ptr);

            get_token(temp);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       ;
            temp->next=NULL;
            findmapping(temp);
            s->tail->next=temp;    
            s->tail=s->tail->next;
        }
    
        printf("%s ", ptr); 
    	ptr = strtok(NULL, delim);
    }

}

// New func for checking int 
int isInt(char* s)
{
	for(int i=0;s[i]!='\0';i++)
	{
		if (isdigit(s[i]) == 0) // check 
            		return 0;
	}
	return 1;
}


void printlinklist(struct token_s * temp)
{
    while(temp)
    {
        printf("\n");
        printf(" line number : %d ",temp->line_number);
        printf(" lexeme : %s ",temp->lexeme);
        printf(" Terminal : %d", temp->type);
        printf(" tokenname : %s ",temp->token_name);

        temp=temp->next;

    }

    return ;
}


tokenstream * tokenizeSourceCode(char* sourceCode)
{
    //char * f = "Test_Cases/Sample_test1.txt";
    FILE * fileptr = fopen(sourceCode,"r");

    // struct linkedlist *head=(struct linkedlist*) malloc (sizeof(struct linkedlist));
    // head->first=NULL;


    tokenstream * s = (tokenstream *)malloc (sizeof(tokenstream));
    s->head=NULL;
    s->tail=NULL;

    if(fileptr)
    {
        while(!feof(fileptr))
        {
            char currline[5000];
            fgets(currline,5000,fileptr);
            push(currline,s);
        }

    }
    else
    {
        printf("\n The file location entered does not exist. ");
    }
        printf("\n Linklist starts here ");

        printlinklist(s->head);

        // printf("\n And for the tail: \n ");
        // printf(" line number : %d ",s->tail->line_number);
        // printf(" lexeme : %s ",s->tail->lexeme);
        // printf(" tokenname : %s \n",s->tail->token_name);
        

    return s;
}

// char * matcher(char* token){
//     char matched_grammar[20];
//     // program_tokens and tokenMap
//     return matched_grammar;
// }

int main(){
    tokenstream * s = tokenizeSourceCode("sample1.txt");
}