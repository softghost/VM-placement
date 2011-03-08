#Variables
CC=gcc
SOBJECTS=cli.o cmd_host.o coloring.o graph.o greedyBFS.o list.o main.o partition.o
TOBJECTS=tester.o

#Make targets
all: tester simulator

simulator: $(SOBJECTS)
	$(CC) $(SOBJECTS) -lpthread -o simulator

tester: $(TOBJECTS)
	$(CC) $(TOBJECTS) -o tester
	
clean:
	rm -rf *.o simulator tester

#Define header dependencies
cli.o: cli.h graph.h
cmd_host.o: macros.h graph.h cmd_host.h partition.h
coloring.o: graph.h list.h coloring.h
graph.o: macros.h graph.h
greedyBFS.o: macros.h graph.h partition.h list.h greedyBFS.h
list.o: list.h
main.o: cmd_host.h
partition.o: macros.h graph.h partition.h