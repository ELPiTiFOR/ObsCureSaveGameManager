CC = "C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe"
WR = "C:\Program Files\CodeBlocks\MinGW\bin\windres.exe"
#CFLAGS = -Wall -Werror -pedantic -std=c99
CPPFLAGS = -Isrc \
	-Isrc/config \
	-Isrc/my_string \
	-Isrc/data_structures \
	-Isrc/gui \
	-Isrc/my_crc
CFLAGS_cli = -std=c99 #-mwindows
CFLAGS_gui = -std=c99 -mwindows
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
	src/my_crc/my_crc.c
OBJ = ${SRC:.c=.o}

SRC_cli = $(SRC) \
	src/main.c
OBJ_cli = ${SRC_cli:.c=.o}

SRC_gui = $(SRC) \
	src/main_gui.c \
	src/gui/gui.c \
	src/gui/window_procs.c
OBJ_gui = ${SRC_gui:.c=.o}

all: ocsgm

resources/resources.o: resources/resources.rc
	$(WR) $^ -o $@

ocsgm: $(OBJ_cli)
	$(CC) $(CFLAGS_cli) -o $@ $^

ocsgm_gui: $(OBJ_gui) resources/resources.o
	$(CC) $(CFLAGS_gui) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) ocsgm_gui ocsgm resources/resources.o $(OBJ) $(OBJ_cli) $(OBJ_gui)