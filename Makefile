CC = gcc 
CFLAGS = -Wall -std=c99 -g # "curl-config --cflags" output is empty  
LD = gcc
LDFLAGS = -std=c99 -g 

SRCS   = sha256.c
OBJS1  = sha256.o
TARGETS= sha256.out

all: ${TARGETS}

sha256.out: $(OBJS1) 
	$(LD) -o $@ $^ $(LDFLAGS) $(LDFLAGS) 

%.o: %.c 
	$(CC) $(CFLAGS) -c $< 

%.d: %.c
	gcc -MM -MF $@ $<

-include $(SRCS:.c=.d)

.PHONY: clean
clean:
	rm -f *~ *.d *.o *~ $(TARGETS)
