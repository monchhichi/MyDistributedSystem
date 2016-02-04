PROG = main
SOURCES = mongoose.c $(PROG).c graph.c
CFLAGS = -W -Wall $(CFLAGS_EXTRA)

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

clean:
	rm -rf *.o $(PROG)
