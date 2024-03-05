all: prog4

prog4: prog4.c
	gcc -Og -fno-stack-protector -o $@ prog4.c 

clean:
	rm -f prog4
