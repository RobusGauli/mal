CC = gcc

CFLAGS =  -std=c11 -g

CLIBS = -lreadline

s0 = step0_repl
s0_source = $(s0).c

DEFAULT_GOAL: current

s1 = step1_read_print
s1_source = $(s1).c


$(s0): $(s0_source)
	$(CC) -o $@ $(CFLAGS) $(s0_source)

s2 = step2_eval
s2_source = $(s2).c

reader_source = reader.c
debug_source = debug.c
token_source = token.c
eval_source = eval.c
env_source = env.c
stringview_source = stringview.c
node_source = node.c
str_source = str.c

reader: $(reader_source)
	$(CC) -o $@ -c $(CFLAGS) $(reader_source)

debug: $(debug_source)
	$(CC) -o $@ -c $(CFLAGS) $(debug_source)

token: $(token_source)
	$(CC) -o $@ -c $(CFLAGS) $(token_source)

env: $(env_source)
	$(CC) -o $@ -c $(CFLAGS) $(env_source)

eval: $(eval_source)
	$(CC) -o $@ -c $(CFLAGS) $(eval_source)

node: $(node_source)
	$(CC) -o $@ -c $(CFLAGS) $(node_source)

stringview: $(stringview_source)
	$(CC) -o $@ -c $(CFLAGS) $(stringview_source)

str: $(str_source)
	$(CC) -o $@ -c $(CFLAGS) $(str_source)

$(s1): $(s1_source) reader debug token eval env str
	$(CC) -o $@ $(CFLAGS) reader debug token eval env $(s1_source) $(CLIBS)

$(s2): $(s2_source) reader debug token eval env stringview node str
	$(CC) -o $@ $(CFLAGS) str token reader debug eval env stringview node  $(s2_source) $(CLIBS)

.PHONY clean:
	rm -rf reader debug step1_read_print eval env token node str

current: $(s2)
	./$(s2)
