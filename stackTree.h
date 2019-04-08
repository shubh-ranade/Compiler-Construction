/*
GROUP NUMBER: 45
GROUP MEMBERS:
	1. SHAH NEEL KAUSHIK
	2. MEHTA AASHAY PINKESH
	3. RANADE SHUBHANKAR PRASAD
*/

#ifndef _STACK_
#define _STACK_

#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"

typedef struct attNode{
	tnt_tag t;
	grSymbol s;
} *AttNode;

typedef struct stackNode{
	AttNode grammar_node;
	struct stackNode* next;
	struct stackNode* parent;
	struct stackNode* right;
	struct stackNode* child;
	char value[MAX_FLOAT_LEN+1];
	int line;
} *StackNode;

typedef struct stack{
	StackNode head;
	int size;
} *Stack;

typedef StackNode TreeNode;

TreeNode createTree(TreeNode root);

Stack newStack();
Stack push(StackNode stNode, Stack st);
Stack pop(Stack st);
StackNode top(Stack st);
int isEmpty(Stack st);
StackNode createNode(AttNode grammar_node);

#endif
