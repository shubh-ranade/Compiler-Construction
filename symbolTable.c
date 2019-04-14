#include "symbolDef.h"
#include "symbolTable.h"

#define INT_WIDTH 2
#define REAL_WIDTH 4

funcEntry ftable[F_TABLE_SIZE];

int hashFunc(char* key)
{
	unsigned long hash = 5381;
	int c;
	while (c = *key++)
	    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return (int)(hash % F_TABLE_SIZE);
}

int hashInd(char* key)
{
	unsigned long hash = 5381;
	int c;
	while (c = *key++)
	    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return (int)(hash % IND_TABLE_SIZE);
}

IndTable initializeIndTable(){
    IndTable indtable = (IndTable) malloc(sizeof(indEntry) * IND_TABLE_SIZE);
    for(int i = 0; i < IND_TABLE_SIZE; i++){
        indtable[i].isvalid = 0;
        indtable[i].width = 0;
        indtable[i].offset = 0;
        indtable[i].next = NULL;
        //indtable[i].tlist = NULL;
    }
    return indtable;
}

RecTable initializeRecTable(){
    RecTable rectable = (RecTable) malloc(sizeof(indEntry) * REC_TABLE_SIZE);
    for(int i = 0; i < REC_TABLE_SIZE; i++){
        rectable[i].isvalid = 0;
        rectable[i].width = 0;
        rectable[i].tlist=NULL;
        rectable[i].next = NULL;
    }
    return rectable;
}

void initializeFuncTable(){
    //FuncTable ftable = (FuncTable) malloc(sizeof(funcEntry) * F_TABLE_SIZE);
    for(int i = 0; i < F_TABLE_SIZE; i++){
        ftable[i].isvalid = 0;
        ftable[i].input_par = NULL;
        ftable[i].output_par = NULL;
        ftable[i].next = NULL;
        ftable[i].idtable = NULL;
    }
}

void addVariable(char *funcName, AstNode varNode, int offs, int wid)
{
    int findex = hashFunc(funcName);
    funcEntry* temp = ftable + findex;
    /* while(strcmp(funcName, temp->name) != 0){
        temp = temp->next;
    }
    if(temp->idtable==NULL)
    {
        temp->idtable = initializeIndTable();
    } */
    
    int findvar = hashInd(varNode->value);
    indEntry *tempind = temp->idtable + findvar;
    if(tempind->isvalid==0)
    {
        tempind->isvalid = 1;
        tempind->vtype = varNode->tlist->vtype;
        strncpy(tempind->name, varNode->value, MAX_LEXEME_SIZE+1);
        tempind->width = wid;
        tempind->offset = offs;
    }
    else
    {
        while(tempind->next!=NULL && strcmp(tempind->name,varNode->value)!=0)
        {
            tempind = tempind->next;
        }
        if(tempind->next!=NULL)
        {
            printf("Line %d: Redeclaration of variable %s\n",varNode->line_number, varNode->value);
            //errflag=1
        }
        else
        {
            tempind->next = (struct indEntry*)malloc(sizeof(indEntry));
            tempind->next->isvalid=1;
            tempind->next->vtype = varNode->tlist->vtype;
            strncpy(tempind->next->name, varNode->value, MAX_LEXEME_SIZE+1);
            tempind->next->width = wid;
            tempind->next->offset = offs;                        
        }
        
    }
    return;
}

void addFunction(AstNode funcNode)
{
    int index = hash(funcNode -> value);
    int offset = 0, wid;
    AstNode fieldnode;
    if(ftable[index].isvalid == 0){
        strncpy(ftable[index].name, funcNode->value, sizeof(funcNode->value));
        ftable[index].idtable = initializeIndTable();

        //funid - inputpar - output par - stmts - typedefinitions - declarations - individual declaration
        AstNode tnode = funcNode->child->right->right->child->right->child;
        //IndTable tempInd = ftable[index].idtable;
        while(tnode!=NULL){
            switch(tnode->tlist->vtype){
                case intType:
                    addVariable(funcNode->value, tnode, offset, INT_WIDTH);
                    offset += INT_WIDTH;
                    break;
                case realType:
                    addVariable(funcNode->value, tnode, offset, REAL_WIDTH);
                    offset += REAL_WIDTH;
                    break;
                case recordType:
                    fieldnode = tnode -> child;
                    wid = 0;
                    while(fieldnode!=NULL)
                    {
                        if(fieldnode->tlist->vtype == intType)
                        {
                            wid+=INT_WIDTH;
                        }
                        else
                        {
                            wid+=REAL_WIDTH;
                        }                        
                    }
                    addVariable(funcNode->value, tnode, offset, wid);
                    offset += wid;
                    break;
            }
            tnode = tnode->right;
        }
        ftable[index].input_par = funcNode->child->right;
        ftable[index].output_par = funcNode->child->right->right;
        ftable[index].next = NULL;
        ftable[index].isvalid = 1;
    }
    else{
        FuncTable temp = (struct funcEntry*) &ftable[index];
        while(temp->next != NULL){
            if(strcmp(funcNode->child->value, temp->name) == 0){
                printf("Line %d: Redeclaration of variable %s\n", funcNode->child->line_number, funcNode->child->value);
            }
            temp = temp->next;
        }
        struct funcEntry f;
        strncpy(f.name, funcNode -> value, sizeof(funcNode -> value));
        f.isvalid = 1;
        f.idtable = initializeIndTable();
        AstNode tnode = funcNode->child->right->right->child->right->child;
        // IndTable tempInd = ftable[index].idtable;
        while(tnode!=NULL){
            switch(tnode->tlist->vtype){
                case intType:
                    addVariable(funcNode->value, tnode, offset, INT_WIDTH);
                    offset += INT_WIDTH;
                    break;
                case realType:
                    addVariable(funcNode->value, tnode, offset, REAL_WIDTH);
                    offset += REAL_WIDTH;
                    break;
                
                case recordType:
                    fieldnode = tnode -> child;
                    wid = 0;
                    while(fieldnode!=NULL)
                    {
                        if(fieldnode->tlist->vtype == intType)
                        {
                            wid+=INT_WIDTH;
                        }
                        else
                        {
                            wid+=REAL_WIDTH;
                        }                        
                    }
                    addVariable(funcNode->value, tnode, offset, wid);
                    offset += wid;
                    break;
            }
        }
        f.input_par = funcNode->child->right;
        f.output_par = funcNode->child->right->right;
        f.next = NULL;
        temp->next = (struct funcEntry*)&f;
    }
}