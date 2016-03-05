PROG = main
SOURCES = mongoose.c $(PROG).c graph.c log.c
CFLAGS = -std=c99 -W -Wall $(CFLAGS_EXTRA)

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o cs426_graph_server $(CFLAGS)

clean:
	rm -rf *.o cs426_graph_server
