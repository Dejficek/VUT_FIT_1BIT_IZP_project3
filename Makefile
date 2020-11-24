CFLAGS = -std=c99 -Wall -Wextra -Werror -DNDEBUG proj3.c -g -o proj3 -lm

comp:
	gcc $(CFLAGS)
clean:
	rm proj3
