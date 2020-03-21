YACC := $(shell command -v yacc.exe 2> /dev/null)

all: interpreter

interpreter: y.tab.o lex.yy.o asm.o
	gcc -Wall y.tab.o lex.yy.o asm.o -o interpreter

asm.o: asm.c
	gcc -c asm.c

y.tab.c: interpreter.y
ifdef YACC
	yacc -d -v interpreter.yacc
endif
ifndef YACC
	bison -d -v -y interpreter.y
endif

lex.yy.c: interpreter.l
	lex interpreter.l

clean:
	rm -rf interpreter lex.yy.* y.tab.* *.o *.output