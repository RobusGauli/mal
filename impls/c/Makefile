CC = gcc

CFLAGS =  -std=c11 -g

CLIBS = -lreadline -lm

DEFAULT_GOAL: current
s4 = step4_if_fn_do
s4_source = $(s4).c

token: token.c
	$(CC) -o token -c $(CFLAGS) token.c

reader: reader.c
	$(CC) -o reader -c $(CFLAGS) reader.c

print: print.c
	$(CC) -o print -c $(CFLAGS) print.c

mal: mal.c
	$(CC) -o mal -c $(CFLAGS) mal.c

eval: eval.c
	$(CC) -o eval -c $(CFLAGS) eval.c

env: env.c
	$(CC) -o env -c $(CFLAGS) env.c

$(s4): $(s4_source) token reader print mal eval env
	$(CC) -o $@ $(CFLAGS) $(s4_source) print eval env reader token mal $(CLIBS)

.PHONY clean:
	rm -rf step4_if_fn_do token reader mal print env eval

current: $(s4)
	./$(s4)
