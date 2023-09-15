SRCS = main.c custom_error.c  scan.c gen.c cg.c stm.c tree.c ast.c symbol.c sym.c
SRCS2 = main.c custom_error.c  scan.c gen.c cgn.c stm.c tree.c ast.c symbol.c sym.c

parser1: $(SRCS)
	gcc -std=c99 -o parser1 -g $(SRCS)

parser2: $(SRCS2)
	gcc -std=c99 -o parser2 -g $(SRCS2)

clean:
	rm -f parser1 parser2 *.o *.s out

test: parse1 tests/input06
	./parse1 tests/input06
	gcc -o out out.s
	./out

test2: parse1 tests/runtests
	(cd tests; chmod +x runtests; ./runtests)