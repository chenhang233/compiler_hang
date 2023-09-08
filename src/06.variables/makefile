SRCS = main.c custom_error.c  scan.c generator.c code_generator.c statements.c tree.c ast.c symbol.c
SRCS2 = main.c

parser1: $(SRCS)
	gcc -std=c99 -o parser1 -g $(SRCS)

parser2: $(SRCS2)
	gcc -std=c99 -o parser2 -g $(SRCS2)

clean:
	rm -f parser1 parser2 *.o *.s out

test: parser1 input01 input02
	./parser1 input01
	gcc -o out out.s
	./out
	./parser1 input02
	gcc -o out out.s
	./out