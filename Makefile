CC = g++
CCFLAGS = --std=gnu++98 -pedantic -Wall -Werror -ggdb3

#cyoa-step1: cyoa-step1.cpp
#	$(CC) $(CCFLAGS) -o cyoa-step1 cyoa-step1.cpp


PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsubst %,$(PROGS))
all: $(PROGS)
cyoa-step%: cyoa-step%.cpp
	$(CC) $(CCFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)

