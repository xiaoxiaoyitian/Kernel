SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc, %, $(SRCS))
CC:=g++
FLAGS:=-Wall -g

.PHONY:clean rebuild ALL

ALL:$(OBJS)

%:%.cc
	$(CC)  $^ -o $@ $(FLAGS)

clean:
	rm -rf $(OBJS)

rebuild:clean ALL 

