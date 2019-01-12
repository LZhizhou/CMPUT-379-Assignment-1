all: covertsigs.double
covertsigs.single:
	gcc -Wall -std=c99  -DSINGLE -o covertsigs covertsigs.c
covertsigs.double:
	gcc -Wall -std=c99 -o covertsigs covertsigs.c
	
clean:
	rm -f covertsigs