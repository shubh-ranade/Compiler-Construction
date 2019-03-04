#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"

int size_of_first_set[TOTAL_NUM_NT];
int size_of_follow_set[TOTAL_NUM_NT];
TK_TYPES first[TOTAL_NUM_NT][20];
TK_TYPES follow[TOTAL_NUM_NT][20];
int is_visited_node[TOTAL_NUM_NT];

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
		return;
	}

	// temp = gram_rules[rule_index].first->next;
	// printf("entering while\n");
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = node;
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


void addFirst(TK_TYPES q, int ind)
{
	int i;
	for(i=0; i < size_of_first_set[ind]; i++)
	{
		if(first[ind][i] == q)
		{
			break;
		}
	}
	if(i == size_of_first_set[ind])
	{
		first[ind][size_of_first_set[ind]] = q;
		size_of_first_set[ind]++;
	}
}

void addFollow(TK_TYPES q, int ind)
{
	int i;
	for(i=0;i < size_of_follow_set[ind]; i++)
	{
		if(follow[ind][i] == q)
		{
			break;
		}
	}
	if(i == size_of_follow_set[ind])
	{
		follow[ind][size_of_follow_set[ind]] = q;
		size_of_follow_set[ind]++;
	}
}

void dfs(TK_NTTYPES node)
{
	if(is_visited_node[node])
	{
		return;
	}
	int i, j;
	for(i = 0; i < RULE_COUNT; i++)
	{
		if(gram_rules[i].nonterm_head == node)
		{
			GrNode cur = gram_rules[i].first;
			while(1)
			{
				if(cur->t == TERM)
				{
					addFirst(cur->s.nonterm_type, node);
					break;
				}
				dfs(cur->s.nonterm_type);
				int nul = 0;
				int q = cur->s.nonterm_type;
				for(j = 0; ; j++)
				{
					if (first[q][j] == -1)
					{
						break;
					}
					if(strcmp(map_terminals_to_strings[first[q][j]],"eps")==0)
					{
						nul = 1;
					}
					else
					{
						addFirst(first[q][j], node);
					}	
				}
				if(nul == 1)
				{
					cur = cur->next;
					if(cur == NULL)
					{
						addFirst(TK_EPS,node);
						break;
					}
				}
				else
				{
					break;
				}
			}		
		}
	}
	is_visited_node[(int)node] = 1;
}

void computeFirstSet()
{
	int i, j;
	for(i = 0; i < TOTAL_NUM_NT; i++)
	{
		size_of_first_set[i] = 0;
		is_visited_node[i] = 0;
		for(j = 0; j < 20; ++j)
		{
			first[i][j] = -1;
		}
	}	
	for(i = 0; i < RULE_COUNT; i++)
	{
		TK_NTTYPES tmp=gram_rules[i].nonterm_head;
		if(!is_visited_node[tmp])
		{
			dfs(tmp);
			is_visited_node[tmp] = 1;
		}
	}
	// printf("FIRST : \n");
	// for(i = 0; i < TSIZE; ++i)
	// {
	// 	printf("%s : ", nonterminal_map[i]);
	// 	for(j = 0; j < 20; ++j)
	// 	{
	// 		if (first[i][j] == -1)
	// 		{
	// 			break;
	// 		}
	// 		printf("%s ", token[first[i][j]]);
	// 	}
	// 	printf("\n");
	// }
}

void computeFollowSet()
{
	int i, j, k;
	for(i = 0; i < TOTAL_NUM_NT; i++)
	{
		size_of_follow_set[i] = 0;
		for(j = 0; j < 20; ++j)
		{
			follow[i][j] = -1;
		}
	}
	addFollow(TK_DOL, 0);
	for(k = 0; k < 2; ++k)
	{
		for(i = 0; i < RULE_COUNT; ++i)
		{
			GrNode cur = gram_rules[i].first, nxt;
			TK_NTTYPES par = gram_rules[i].nonterm_head;
			while(cur != NULL)
			{
				if(cur->t == TERM)
				{
					cur = cur->next;
				}
				else
				{
					nxt = cur->next;
					while(nxt != NULL)
					{
						if(nxt->t == TERM)
						{
							addFollow(nxt->s.term_type, cur->s.nonterm_type);
							break;
						}
						else
						{
							int flg = 0;
							int q = (int)(nxt->s.nonterm_type);
							for(j = 0; j < size_of_first_set[q]; j++)
							{
								if(strcmp(map_terminals_to_strings[first[q][j]], "eps") == 0)
								{
									flg=1;
								}
								else
								{
									addFollow(first[q][j],cur->s.nonterm_type);
								}	
							}
							if(!flg)
							{
								break;
							}
							else
							{
								nxt=nxt->next;
							}
						}	
					}
					if(nxt == NULL)
					{
						int q = par;
						for(j = 0; j < size_of_follow_set[q]; j++)
						{
							addFollow(follow[q][j],(cur->s.nonterm_type));
						}
					}
					cur = cur->next;
				}
			}
		}
	}
	// printf("FOLLOW : \n");
	// for(i = 0; i < TSIZE; ++i)
	// {
	// 	printf("%s : ", nonterminal_map[i]);
	// 	for(j = 0; j < 20; ++j)
	// 	{
	// 		if (follow[i][j] == -1)
	// 		{
	// 			break;
	// 		}
	// 		printf("%s ", token[follow[i][j]]);
	// 	}
	// 	printf("\n");
	// }
}