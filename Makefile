CC = gcc
CFLAGS = -W -Wall

# $@ is a target(LHS) and $< is a src(RHS)
%: %.c
	$(CC) $(CFLAGS) -o $@.out $<



clean:
	rm -f *.out
