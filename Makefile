CC = "C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe"
WR = "C:\Program Files\CodeBlocks\MinGW\bin\windres.exe"
#CFLAGS = -Wall -Werror -pedantic -std=c99
CPPFLAGS = -Isrc \
	-Isrc/config \
	-Isrc/my_string \
	-Isrc/data_structures \
	-Isrc/gui
CFLAGS = -std=c99 -mwindows
LDFLAGS = -lgdi32 -lole32 -luuid # -lcomdlg32 -lshell32 

SRC = \
	src/utils.c \
	src/business.c \
	src/commands.c \
	src/data_access.c \
	src/file_io.c \
	src/path.c \
	src/config/config.c \
	src/my_string/my_string.c \
	src/data_structures/pointers.c \
	src/gui/gui.c \
	src/gui/window_procs.c
OBJ = ${SRC:.c=.o}

SRC_cli = $(SRC) src/main.c
OBJ_cli = ${SRC_cli:.c=.o}

SRC_gui = $(SRC) src/main_gui.c
OBJ_gui = ${SRC_gui:.c=.o}

all: ocsgm

resources/resources.o: resources/resources.rc
	$(WR) $^ -o $@

ocsgm: $(OBJ_cli) resources/resources.o
	$(CC) $(CFLAGS) -o $@ $^

ocsgm_gui: $(OBJ_gui) resources/resources.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) ocsgm_gui ocsgm resources/resources.o $(OBJ)