HSRCS= function.h data.h 
SRCS= ast.c cg.c custom_func.c gen.c main.c scan.c stm.c sym.c \
 types.c ../cjson/cjson.c
# SRCN= cgn.c decl.c expr.c gen.c main.c misc.c scan.c stmt.c sym.c \
#  tree.c types.c


cwj: $(SRCS) $(HSRCS)
	cc -std=c99 -o cwj -g  $(SRCS)

test: cwj tests/runtests
	(cd tests; chmod +x runtests; ./runtests)

# test10n: compn tests/input10
# 	./compn tests/input10
# 	nasm -f elf64 out.s
# 	cc -no-pie -o out out.o
# 	./out
