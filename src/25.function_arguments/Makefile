SRCS= ast.c cg.c custom_func.c gen.c main.c scan.c stm.c sym.c \
 types.c ../cjson/cjson.c
SRCN= cgn.c decl.c expr.c gen.c main.c misc.c scan.c stmt.c sym.c \
 tree.c types.c

comp1: $(SRCS)
	gcc -std=c99 -o comp1 -g $(SRCS)

compn: $(SRCN)
	cc -o compn -g $(SRCN)

clean:
	rm -f comp1 compn *.o *.s out

test: comp1 tests/runtests
	(cd tests; chmod +x runtests; ./runtests)

test5: comp1 tests/input05.c lib/printint.c
	./comp1 tests/input05.c
	cc -o out out.s lib/printint.c
	./out

test18: comp1 tests/input18.c lib/printint.c
	./comp1 tests/input18.c
	cc -o out out.s lib/printint.c
	./out

test18a: comp1 tests/input18a.c lib/printint.c
	./comp1 tests/input18a.c
	cc -o out out.s lib/printint.c
	./out


test20: comp1 tests/input20.c lib/printint.c
	./comp1 tests/input20.c
	cc -o out out.s lib/printint.c
	./out


test21: comp1 tests/input21.c lib/printint.c
	./comp1 tests/input21.c
	cc -o out out.s lib/printint.c
	./out

test22: comp1 tests/input22.c lib/printint.c
	./comp1 tests/input22.c
	cc -o out out.s lib/printint.c
	./out
test23: comp1 tests/input23.c lib/printint.c
	./comp1 tests/input23.c
	cc -o out out.s lib/printint.c
	./out
test24: comp1 tests/input24.c lib/printint.c
	./comp1 tests/input24.c
	cc -o out out.s lib/printint.c
	./out

test25: comp1 tests/input25.c lib/printint.c
	./comp1 tests/input25.c
	cc -o out out.s lib/printint.c
	./out
	
test27: comp1 input27a.c input27b.c
	./comp1 input27a.c
	cc -o out input27b.c out.s lib/printint.c 
	./out

test28: comp1 
	./comp1 tests/input28.c
	cc -o out out.s lib/printint.c 
	./out


# testn: compn tests/runtestsn
# 	(cd tests; chmod +x runtestsn; ./runtestsn)

# test10n: compn tests/input10
# 	./compn tests/input10
# 	nasm -f elf64 out.s
# 	cc -no-pie -o out out.o
# 	./out
