#ifndef _SYMBTABLE_
#define _SYMBTABLE_

#include "symbolDef.h"
//func table fuctions
void initializeFuncTable();
void addFunction(AstNode funcNode);

TK_TYPES findKeyword(HashTable ht, char* key);
void freeFuncTable();

#endif