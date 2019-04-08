/*
GROUP NUMBER: 45
GROUP MEMBERS:
	1. SHAH NEEL KAUSHIK
	2. MEHTA AASHAY PINKESH
	3. RANADE SHUBHANKAR PRASAD
*/

#include "lexerDef.h"
#include "lexer.h"
#include "parser.h"
#include "stackTree.h"
#include <time.h>

extern HashTable ht;

//Reads <filename> and prints all lexemes along with their respective line numbers
void printTokens(FILE *fp){
	while(1){
		tokenInfo token = getNextToken(fp);
		if(token.type_of_token == NOT_KEYWORD){
			break;
		}
		printf("Line %d: Lexeme = %s\n", token.line_num, token.value);		
	}
}

int main(int argc, char *argv[]){

	//Initializes a hash-table ADT to store keywords
	ht = initializeHashTable();
	FILE *fp = fopen(argv[1], "r");
	FILE *outfile = fopen(argv[2], "w");
	if(argc!=3){
		printf("Format: ./stage1exe [inputfile] [outputfile]\n");
		return 0;
	}
	int option;
	TreeNode root;
	clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    printf("FIRST and FOLLOW sets automated.\nBoth lexical and syntax analysis modules implemented.\n\n");
	while(1){
		printf("Enter option: ");
		scanf("%d", &option);
		switch(option){

			//Press 0 to exit the program
			case 0:
				fclose(fp);
				fclose(outfile);
				freeHashTable();
				return 0;

			//Press 1 to create a copy of the source code without any comments
			case 1:
				removeComments(argv[1], "no_comments.txt");
				system("cat no_comments.txt");
				break;

			//Press 2 to print a list of all lexemes in the source file
			case 2:
				printTokens(fp);
				if(feof(fp)){
					printf("Rewinding1\n");
					rewind(fp);
				}
				break;

			case 3:
				loadGrammar();
				computeFirstSet();
				computeFollowSet();
				createParsingTable();
				root = parse(fp);
				printTree(root, &outfile);
				freeParseTree(root);
				freeGrammar();
				break;

			case 4:				
			    start_time = clock();
				loadGrammar();
				computeFirstSet();
				computeFollowSet();
				createParsingTable();
				root = parse(fp);
				printTree(root, &outfile);
				freeParseTree(root);
				freeGrammar();
				end_time = clock();
			    total_CPU_time  =  (double) (end_time - start_time);
			    total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
			    printf("time taken = %fs, ticks = %f\n", total_CPU_time_in_seconds, total_CPU_time);
				break;

			//Default case to handle all other options
			default:
				printf("Not a valid option\n");
		}
	}
	return 0;
}