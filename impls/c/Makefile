CC = gcc

CFLAGS =  -std=c11 -g

CFILES = header.c

s0 = step0_repl
s0_source = $(s0).c

DEFAULT_GOAL: current

s1 = step1_read_print
s1_source = $(s1).c

$(s0): $(s0_source)
	$(CC) -o $@ $(CFLAGS) $(s0_source)

reader_source = reader.c
debug_source = debug.c

reader: $(reader_source)
	$(CC) -o $@ -c $(CFLAGS) $(reader_source)

debug: $(debug_source)
	$(CC) -o $@ -c $(CFLAGS) $(debug_source)

$(s1): $(s1_source) reader debug
	$(CC) -o $@ $(CFLAGS) reader debug $(s1_source) -lreadline

.PHONY clean:
	rm -rf reader debug step1_read_print

current: $(s1)
	./$(s1)
