default: ./lab3.c
	gcc -o lab1.run ./lab3.c

test: ./lab3.c
	echo gcc:
	gcc -o lab1.run ./lab3.c -fsanitize=address
	echo cppcheck:
	cppcheck --enable=all --inconclusive --std=posix lab3.c
	
