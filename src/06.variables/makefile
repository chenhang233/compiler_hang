SRCS = main.c custom_error.c
SRCS2 = main.c

parser1: $(SRCS)
	gcc -std=c99 -o parser1 -g $(SRCS)

parser2: $(SRCS2)
	gcc -std=c99 -o parser2 -g $(SRCS2)

clean:
	rm -f parser1 parser2 *.o *.s out