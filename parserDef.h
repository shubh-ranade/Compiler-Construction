
#include "lexerDef.h"
typedef enum{program,otherFunctions,mainFunction,stmts,function,input_par,output_par,parameter_list,dataType,remaining_list,dataType,primitiveDatatype,constructedDatatype,typeDefinitions,declarations,otherStmts,returnStmt,typeDefinition

}TK_NTTYPES;

typedef union
{
	TK_TYPES term;
	TK_NTTYPES nonterm;
}grSymbol;

typedef enum{term, non_term} tnt_tag;

typedef struct grNode{
	tnt_tag t;
	grSymbol s;
	struct grNode* next;
}* GrNode;

typedef struct grHead{
	grSymbol s;
	GrNode first;
}* GrHead;