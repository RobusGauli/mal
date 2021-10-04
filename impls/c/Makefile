CC = gcc

CFLAGS =  -std=c11 -g

CLIBS = -lreadline -lm

DEFAULT_GOAL: current
s4 = step4_if_fn_do
s4_source = $(s4).c

token: token.c
	$(CC) -o token -c $(CFLAGS) token.c

$(s4): $(s4_source) token
	$(CC) -o $@ $(CFLAGS) $(s4_source) token $(CLIBS)

.PHONY clean:
	rm -rf step4_if_fn_do token

current: $(s4)
	./$(s4)
