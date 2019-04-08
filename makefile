exe: driver.o hashtable.o lexer.o parser.o stackTree.o
	gcc -o stage1exe driver.o hashtable.o lexer.o parser.o stackTree.o

driver.o: lexerDef.h lexer.h parserDef.h parser.h stackTree.h driver.c
	gcc -c -g driver.c -o driver.o

hashtable.o: lexerDef.h hashtable.c
	gcc -c -g hashtable.c -o hashtable.o

lexer.o: lexerDef.h lexer.c
	gcc -c -g lexer.c -o lexer.o

parser.o: lexerDef.h stackTree.h lexer.h parserDef.h parser.c
	gcc -c -g parser.c -o parser.o

stackTree.o: lexerDef.h lexer.h parserDef.h stackTree.h stackTree.c
	gcc -c -g stackTree.c -o stackTree.o

clean:
	rm -rf *.o