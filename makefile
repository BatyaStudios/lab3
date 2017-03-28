default: ./lab3.c
	gcc -o lab1.run ./lab3.c

test: ./lab3.c
	reset
	echo gcc:
	gcc -o lab1.run ./lab3.c -fsanitize=address
	echo cppcheck:
	cppcheck --enable=all --inconclusive --std=posix lab3.c
	echo codepatch.pl:
	/home/batya/linux/scripts/checkpatch.pl -f /home/batya/labs/lab3/lab3.c
