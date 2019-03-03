#include "lexerDef.h"
extern HashTable ht;

int main(){
	ht = initializeHashTable();
	printf("fourth = %s\n", ht[4].var_name);
	return 0;
}