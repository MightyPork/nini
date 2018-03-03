a.out: test.c nini.c
	cc -Og test.c nini.c

run: a.out
	./a.out
