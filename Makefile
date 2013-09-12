SOURCES	=	main.c \
		xor.c

all:
	gcc -Wall -O3 -o xor $(SOURCES)
