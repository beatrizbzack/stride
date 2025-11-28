all: stride vm

stride.tab.c stride.tab.h: stride.y
	bison -d -o stride.tab.c stride.y

lex.yy.c: stride.l stride.tab.h
	flex -o lex.yy.c stride.l

stride: stride.tab.c lex.yy.c
	gcc -o stride stride.tab.c lex.yy.c -lfl

vm: vm.c
	gcc -o vm vm.c

clean:
	rm -f stride vm stride.tab.c stride.tab.h lex.yy.c out.asm *.o
