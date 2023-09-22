all: http client
LIBS = -lpthread #-lsocket
http: http.c
	gcc -g -W -Wall $(LIBS) -o $@ $<

client: simpleclient.c
	gcc -W -Wall -o $@ $<
clean:
	rm http
