SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
CC:=g++
EXE:=server.EXE
FLAGS:=-Wall -g


$(EXE):$(OBJS)
	$(CC) -o $@ $^ $(FLAGS)
%.o:%.cc
	$(CC) -c $^ -o $@ $(FLAGS)

clean:
	rm -rf $(OBJS)


