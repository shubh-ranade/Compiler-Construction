exe: driver.o hashtable.o lexer.o
	gcc -o exe driver.o hashtable.o lexer.o

driver.o: lexerDef.h driver.c
	gcc -c driver.c -o driver.o

hashtable.o: lexerDef.h hashtable.c
	gcc -c hashtable.c -o hashtable.o

lexer.o: lexerDef.h lexer.c
	gcc -c lexer.c -o lexer.o

clean:
	rm -rf *.o