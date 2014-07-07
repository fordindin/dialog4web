all:
	cc -ggdb -L ~/lib -lmicrohttpd -I ~/include -o d4web htmlgen.c envparser.c dialog4web.c

clean:
	@rm -rf d4web d4web.dSYM

envparser::
	cc -ggdb -L  ~/lib -lmicrohttpd -I ~/include -o envparser envparser.c htmlgen.c
