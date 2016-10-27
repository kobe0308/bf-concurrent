cc ?= gcc
CFLAGS = -Wall  -std=gnu99 -g  -pthread

EXEC = jit-kyconcurrent

all: $(EXEC)

jit-kyconcurrent: jit-kyconcurrent.c
	$(cc) $(CFLAGS) -o $@ $@.c

run: $(EXEC)
	./jit-kyconcurrent hello-concurrent.b

.PHONY: clean

clean: 
		$(RM) $(EXEC) *.o 

