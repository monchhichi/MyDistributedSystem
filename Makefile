CC = g++
LOADLIBES = -lthrift
CXXFLAGS = -std=c++11 -W -Wall

EXEC = cs426_graph_server
SRCS = graph.c main.cpp
OBJS = main.o graph.o mongoose.o

GEN_SRC = gen-cpp/RemoteService.cpp \
          gen-cpp/RemoteService_types.cpp

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CXXFLAGS) $(LOADLIBES) $(GEN_SRC) $(OBJS) -o $(EXEC)

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp -o main.o

graph.o: graph.c main.o
	$(CC) $(CXXFLAGS) -c graph.c -o graph.o

mongoose.o:
	$(CC) -c mongoose.c -o mongoose.o

thrift:
	thrift --gen cpp RemoteService.thrift

ThriftTestClient:
	$(CC) $(CXXFLAGS) ThriftTestClient.cpp $(GEN_SRC) $(LOADLIBES) -o ThriftTestClient

clean:
	rm -f $(EXEC) $(OBJS)
