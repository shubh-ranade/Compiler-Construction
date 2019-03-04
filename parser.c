#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"
#include "stack.h"

int size_of_first_set[TOTAL_NUM_NT];
int size_of_follow_set[TOTAL_NUM_NT];
TK_TYPES first[TOTAL_NUM_NT][20];
TK_TYPES follow[TOTAL_NUM_NT][20];
int is_visited_node[TOTAL_NUM_NT];
int ParseTable[TOTAL_NUM_NT][TOTAL_NUM_TOKENS];

char *map_terminals_to_strings[TOTAL_NUM_TOKENS] = {
	"TK_ASSIGNOP",
	"TK_COMMENT",
	"TK_FIELDID",
	"TK_ID",
	"TK_NUM",
	"TK_RNUM",
	"TK_FUNID",
	"TK_RECORDID",
	"TK_WITH",
	"TK_PARAMETERS",
	"TK_END",
	"TK_WHILE",
	"TK_TYPE",
	"TK_MAIN",
	"TK_GLOBAL",
	"TK_PARAMETER",
	"TK_LIST",
	"TK_SQL",
	"TK_SQR",
	"TK_INPUT",
	"TK_OUTPUT",
	"TK_INT",
	"TK_REAL",
	"TK_COMMA",
	"TK_SEM",
	"TK_COLON",
	"TK_DOT",
	"TK_ENDWHILE",
	"TK_OP",
	"TK_CL",
	"TK_IF",
	"TK_THEN",
	"TK_ENDIF",
	"TK_READ",
	"TK_WRITE",
	"TK_RETURN",
	"TK_PLUS",
	"TK_MINUS",
	"TK_MUL",
	"TK_DIV",
	"TK_CALL",
	"TK_RECORD",
	"TK_ENDRECORD",
	"TK_ELSE",
	"TK_AND",
	"TK_OR",
	"TK_NOT",
	"TK_LT",
	"TK_LE",
	"TK_EQ",
	"TK_GT",
	"TK_GE",
	"TK_NE",
	"eps",
	"$"
};

char *map_nonterminals_to_strings[TOTAL_NUM_NT] = {
	"program",
	"mainFunction",
	"otherFunctions",
	"function",
	"input_par",
	"output_par",
	"parameter_list",
	"dataType",
	"remaining_list",
	"primitiveDatatype",
	"constructedDatatype",
	"stmts",
	"typeDefinitions",
	"typeDefinition",
	"fieldDefinitions",
	"fieldDefinition",
	"moreFields",
	"declarations",
	"declaration",
	"global_or_not",
	"otherStmts",
	"stmt",
	"assignmentStmt",
	"singleOrRecId",
	"new24",
	"funCallStmt",
	"outputParameters",
	"inputParameters",
	"iterativeStmt",
	"conditionalStmt",
	"elsePart",
	"ioStmt",
	"allVar",
	"newVar",
	"arithmeticExpression",
	"expPrime",
	"term",
	"termPrime",
	"factor",
	"highPrecedenceOperators",
	"lowPrecedenceOperators",
	"all",
	"temp",
	"booleanExpression",
	"var",
	"logicalOp",
	"relationalOp",
	"returnStmt",
	"optionalReturn",
	"idList",
	"more_ids"
};

struct grHead gram_rules[RULE_COUNT];

int find(char *str, int t_or_nt){

	// printf("Inside find\n");
	int i;
	if(t_or_nt){
		for(i = 0; i < TOTAL_NUM_NT; i++){
			if(strcmp(str, map_nonterminals_to_strings[i]) == 0)
				return i;
		}
	}

	else{
		for(i = 0; i < TOTAL_NUM_TOKENS; i++){
			if(strcmp(str, map_terminals_to_strings[i]) == 0)
				return i;
		}
	}
}

void insertAtEnd(GrNode node, int rule_index){

	// printf("Inside insertAtEnd\n");
	GrNode temp = gram_rules[rule_index].first;
	if(temp == NULL){
		gram_rules[rule_index].first = node;
		gram_rules[rule_index].last=node;
		return;
	}

	// temp = gram_rules[rule_index].first->next;
	// printf("entering while\n");
	while(temp->next != NULL)
	{	
		temp = temp->next;
	}
	temp->next = node;
	node->prev=temp;
	gram_rules[rule_index].last=node;
	// printf("returning\n");
	return;
}

void loadGrammar()
{
	FILE* fp = fopen("grammar.txt","r");
	int i,j;
	char *ch;
	char gline[100];
	char tnt[50];

	// printf("Inside loadGrammar\n");
	
	for(i = 0; i < RULE_COUNT; i++)
	{
		fgets(gline,100,fp);
		// printf("gline = %s\n", gline);
		ch = gline;
		j = 0;
		while(*ch != ' ')
		{
			tnt[j]=*ch;
			j++;
			ch++;
		}
		tnt[j] = '\0';						// lhs of grammar rule ends here

		//make grhead
		int nt_index = find(tnt, 1);
		gram_rules[i].nonterm_head = nt_index;
		gram_rules[i].first = NULL;
		gram_rules[i].last=NULL;

		while(*ch != '>')
			ch++;
		ch++;

		while(*ch != '\0')
		{
			if(*ch >= 'a' && *ch <= 'z')
			{
				//ch++;
				j=0;
				while(*ch != ' ' && *ch != '\n' && *ch != '\r')
				{
					tnt[j]=*ch;
					j++;
					ch++;
				}
				tnt[j]='\0';

				//handle this nonterminal
				//construct grnode
				if(strcmp(tnt, "eps") != 0)
				{
					GrNode nt_node = (GrNode) malloc(sizeof(struct grNode));
					nt_node->t = 1;
					nt_node->s.nonterm_type = find(tnt, 1);
					nt_node->next = NULL;
					nt_node->prev=NULL;
					//insert this node in the linkedlist corresponding to ith rule
					insertAtEnd(nt_node, i);
					// printf("tnt = %s find(tnt, 1) = %d\n", tnt, find(tnt, 1));
				}
				else
				{
					GrNode t_node = (GrNode) malloc(sizeof(struct grNode));
					t_node->t = 0;
					t_node->s.term_type = find(tnt, 0);
					t_node->next = NULL;
					t_node->prev=NULL;

					//insert this node in the linkedlist corresponding to ith rule
					insertAtEnd(t_node, i);
				}
			}

			else if(*ch >= 'A' && *ch <= 'Z')
			{
				j=0;
				while(*ch != ' ' && *ch != '\n' && *ch != '\r')
				{
					tnt[j] = *ch;
					j++;
					ch++;
				}
				tnt[j] = '\0';

				//handle this terminal
				//construct grnode
				GrNode t_node = (GrNode) malloc(sizeof(struct grNode));
				t_node->t = 0;
				t_node->s.term_type = find(tnt, 0);
				t_node->next = NULL;
				t_node->prev=NULL;

				//insert this node in the linkedlist corresponding to ith rule
				insertAtEnd(t_node, i);
			}
			else
				ch++;	

		}

	}
	/*printf("gram_rules[0].nonterm_head = %d\n", gram_rules[0].nonterm_head);
	printf("gram_rules[0].nonterm_head = %d\n", gram_rules[0].first->s.nonterm_type);
	printf("gram_rules[0].nonterm_head = %d\n", gram_rules[0].first->next->s.nonterm_type);*/
	fclose(fp);
	return;

}



//--------------------------------------------------------------------------------------------------------------------------


void addToFirstSet(TK_TYPES ter, int index)
{
	int i=0;
	while(i < size_of_first_set[index])
	{
		if(first[index][i] == ter)
		{
			break;
		}
		i++;
	}

	if(i == size_of_first_set[index])
	{
		first[index][size_of_first_set[index]] = ter;
		size_of_first_set[index]++;
	}
	return;
}

void addToFollowSet(TK_TYPES ter, int index)
{
	int i=0;
	while(i < size_of_follow_set[index])
	{
		if(follow[index][i] == ter)
		{
			break;
		}
		i++;
	}

	if(i == size_of_follow_set[index])
	{
		follow[index][size_of_follow_set[index]] = ter;
		size_of_follow_set[index]++;
	}
	return;
}

void search(TK_NTTYPES nonter)
{
	if(is_visited_node[nonter])
	{
		return;
	}
	int i=0, j;
	while(i < RULE_COUNT)
	{
		if(gram_rules[i].nonterm_head == nonter)
		{
			GrNode current = gram_rules[i].first;
			while(1)
			{
				if(current->t == TERM)
				{
					addToFirstSet(current->s.nonterm_type, nonter);
					break;
				}
				search(current->s.nonterm_type);
				int epsilon = 0;
				int p = current->s.nonterm_type;
				j=0;
				while(1)
				{
					if (first[p][j] == -1)
					{
						break;
					}
					if(strcmp(map_terminals_to_strings[first[p][j]],"eps")!=0)
					{
						addToFirstSet(first[p][j], nonter);
					}
					else
					{
						epsilon = 1;
					}
					j++;	
				}
				if(epsilon == 0)
				{
					break;
				}
				else
				{
					current = current->next;
					if(current == NULL)
					{
						addToFirstSet(TK_EPS,nonter);
						break;
					}
				}
			}		
		}
		i++;
	}
	is_visited_node[nonter] = 1;
}

void computeFirstSet()
{
	int i=0, j;
	while(i < TOTAL_NUM_NT)
	{
		size_of_first_set[i] = 0;
		is_visited_node[i] = 0;
		j=0;
		while(j < 20)
		{
			first[i][j] = -1;
			j++;
		}
		i++;
	}

	i=0;	
	while(i < RULE_COUNT)
	{
		TK_NTTYPES nonter=gram_rules[i].nonterm_head;
		if(!is_visited_node[nonter])
		{
			search(nonter);
			is_visited_node[nonter] = 1;
		}
		i++;
	}
}

void computeFollowSet()
{
	int i=0, j, k;
	while(i < TOTAL_NUM_NT)
	{
		size_of_follow_set[i] = 0;
		j=0;
		while(j < 20)
		{
			follow[i][j] = -1;
			j++;
		}
		i++;
	}
	addToFollowSet(TK_DOL, 0);

	for(k = 0; k < 2; k++)
	{
		i=0;
		while(i < RULE_COUNT)
		{
			GrNode current = gram_rules[i].first, temp;
			TK_NTTYPES parent = gram_rules[i].nonterm_head;
			while(current != NULL)
			{
				if(current->t == TERM)
				{
					current = current->next;
				}
				else
				{
					temp = current->next;
					while(temp != NULL)
					{
						if(temp->t == NON_TERM)
						{
							int epsilon = 0;
							int p = temp->s.nonterm_type;
							j=0;
							while(j < size_of_first_set[p])
							{
								if(strcmp(map_terminals_to_strings[first[p][j]], "eps") != 0)
								{
									addToFollowSet(first[p][j],current->s.nonterm_type);
								}
								else
								{
									epsilon=1;
								}	
								j++;
							}

							if(epsilon)
							{
								temp=temp->next;
							}
							else
							{
								break;
							}
						}
						else
						{	
							addToFollowSet(temp->s.term_type, current->s.nonterm_type);
							break;
						}	
					}
					if(temp == NULL)
					{
						int p = parent;
						
						j=0;
						while(j < size_of_follow_set[p])
						{
							addToFollowSet(follow[p][j],current->s.nonterm_type);
							j++;
						}
					}
					current = current->next;
				}
			}
			i++;
		}
	}
}

void createParsingTable(){

	int i, j, k, flag;
	GrNode temp;
	for(i = 0; i < TOTAL_NUM_NT; i++){
		for(j = 0; j < TOTAL_NUM_TOKENS; j++){
			ParseTable[i][j] = -1;
		}
	}
	for(i = 0; i < RULE_COUNT; i++){
		temp = gram_rules[i].first;
		while(temp != NULL){
			flag = 0;
			if(temp->t == TERM){
				ParseTable[gram_rules[i].nonterm_head][temp->s.term_type] = i;
				break;
			}
			else{
				j = 0;
				while(first[temp->s.nonterm_type][j] != -1){
					if(first[temp->s.nonterm_type][j] != TK_EPS){
						ParseTable[gram_rules[i].nonterm_head][first[temp->s.nonterm_type][j]] = i;
					}
					else{
						flag++;
					}
					j++;
				}
			}
			if(!flag){
				break;
			}
			temp = temp->next;
		}
		if(temp == NULL){
			k = 0;
			while(follow[gram_rules[i].nonterm_head][k] != -1){
				ParseTable[gram_rules[i].nonterm_head][follow[gram_rules[i].nonterm_head][k]] = i;
				k++;
			}
		}
	}
}

void printParseTable(){
	int i, j;
	for(i = 0; i < TOTAL_NUM_NT; i++){
		for(j = 0; j < TOTAL_NUM_TOKENS; j++){
			printf("%d ", ParseTable[i][j]);
		}
		printf("\n");
	}
}

void parse(FILE *fp){
	//printf("In parse\n\n");
	//initailize stack
	Stack pStack = newStack();
	//printf("Stack built\n\n");
	//push dollar and start symbol
	//make attNode
	AttNode dollarNode = (AttNode) malloc(sizeof(struct attNode));
	dollarNode->t = TERM;
	dollarNode->s.term_type = TK_DOL;

	AttNode startNode = (AttNode) malloc(sizeof(struct attNode));
	startNode->t = NON_TERM;
	startNode->s.nonterm_type = program;

	//printf("About to push\n\n");
	//push
	pStack = push(dollarNode, pStack);
	pStack = push(startNode, pStack);
	tokenInfo token;
	AttNode tstack, push_node;
	token=getNextToken(fp);
	//printf("%s\n",token.value);
	int rule;
	GrNode temp;

	//printf("About to enter while\n\n");
	while(1)
	{
		tstack=top(pStack);
		//if(tstack->t==NON_TERM)
		//	printf("%s %d\n",map_nonterminals_to_strings[tstack->s.nonterm_type],tstack->t);

		pStack=pop(pStack);
		if(token.type_of_token==NOT_KEYWORD)
		{
			if(tstack->t==TERM)
			{
				if(tstack->s.term_type==TK_DOL)
				{
					printf("compiled successfully\n");
					break;
				}		
			}
			printf("Line %d: file ended unexpectedly\n",token.line_num);
		}

		if(tstack->t==TERM)
		{
			if(tstack->s.term_type==TK_DOL)
			{
				printf("Line %d: Extra tokens occurring after main function\n",token.line_num);
				break;
			}
			if(token.type_of_token==tstack->s.term_type)
			{
				printf("%s %d %s\n",map_terminals_to_strings[tstack->s.nonterm_type],tstack->t,token.value);
				token=getNextToken(fp);
				continue;
			}
			else
			{
				printf("Line %d: The token %s for lexeme %s does not match with the expected token %s\n",token.line_num,map_terminals_to_strings[token.type_of_token],token.value,map_terminals_to_strings[tstack->s.term_type]);
				// token=getNextToken(fp);
			}
		}
		else
		{
			rule=ParseTable[tstack->s.nonterm_type][token.type_of_token];
			//printf("%d  %d\n", tstack->s.nonterm_type, token.type_of_token);
			if(rule==-1)
			{
				//printf("3.1\n");
			}
			else
			{
				temp=gram_rules[rule].last;
				while(temp!=NULL)
				{
					push_node=(AttNode)malloc(sizeof(struct attNode));
					push_node->t=temp->t;
					if(temp->t==TERM)
					{
						push_node->s.term_type=temp->s.term_type;
					}
					else
					{
						push_node->s.nonterm_type=temp->s.nonterm_type;
					}
					push(push_node,pStack);
					temp = temp -> prev;
				}
			}
		}
	}
}