CC = "C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe"
#CFLAGS = -Wall -Werror -pedantic -std=c99
CFLAGS = -std=c99

SRC = src/main.c \
	src/utils.c \
	src/business.c \
	src/commands.c \
	src/data_access.c \
	src/file_io.c \
	src/path.c
OBJ = ${SRC:.c=.o}

ocsgm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) ocsgm $(OBJ)