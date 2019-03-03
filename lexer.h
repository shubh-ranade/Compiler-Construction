#ifndef _LEX_
#define _LEX_

FILE* getStream(FILE *fp);
tokenInfo getNextToken(FILE *fp);
void removeComments(char *testCaseFile, char *cleanFile);

#endif