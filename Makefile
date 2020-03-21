YACC := $(shell command -v yacc.exe 2> /dev/null)

all: interpreter

interpreter: y.tab.o lex.yy.o symbol_table.o
	gcc -Wall y.tab.o lex.yy.o symbol_table.o -o interpreter

#symbol_table.o: symbol_table.c
#	gcc -c symbol_table.c

y.tab.c: interpreter.y
ifdef YACC
	yacc -d -v interpreter.yacc
endif
ifndef YACC
	bison -d -v -dy interpreter.y
endif

lex.yy.c: interpreter.l
	lex interpreter.l

clean:
	rm -rf interpreter lex.yy.* y.tab.* *.o *.output