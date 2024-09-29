#source file/files
OBJS = main.cpp camera.cpp cube.cpp glad.c stb_image.cpp tile.cpp

#compiler
CC = g++

#flag which ignores warnings
COMPILER_FLAGS = -w

#including libary
LINKER_FLAGS = -ldl -lglfw

#object file name (file that is supposed to be runned after compiling)
OBJ_NAME = prog

#compiling
all : $(OBJS)
		$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
				
