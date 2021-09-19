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

reader: reader.c
	$(CC) -o $@ -c $(CFLAGS) $(reader_source)

$(s1): $(s1_source) reader
	$(CC) -o $@ $(CFLAGS) reader $(s1_source)


current: $(s1)
	./$(s1)
