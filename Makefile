build: tree.c tema2.c
	gcc tree.c tema2.c -Wall -Werror -Wextra -std=c99 -o tema2

clean:
	rm -f tema2