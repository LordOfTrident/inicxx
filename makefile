# Names
N_WRITING = bin/writing
N_READING = bin/reading
N_CONVERSION = bin/conversion

# Files
F_SRC = \
	examples/writing.cc\
	examples/reading.cc\
	examples/conversion.cc

F_HEADER = \
	inicxx.hh\

F_ALL = ${F_SRC} ${F_HEADER}

# Compiler related
CXX = g++
CXX_VER = c++17
CXX_FLAGS = \
	-O3\
	-Wall\
	-std=${CXX_VER}\
	-I./\

ifeq (${OS}, Windows_NT)
	N_WRITING += .exe
	N_READING += .exe
	N_CONVERSION += .exe

	CREATE_BIN_DIRECTORY = if not exist "./bin" mkdir ${D_BIN}
	CLEAN = del ${N_WRITING} && del ${N_READING} && del ${N_CONVERSION}
else
	CREATE_BIN_DIRECTORY = mkdir -p ./bin
	CLEAN = rm ${N_WRITING} && rm ${N_READING} && rm ${N_CONVERSION}
endif

compile: ${F_ALL}
	@${CREATE_BIN_DIRECTORY}
	@echo Created ./bin/

	@echo Compiling...
	@${CXX} examples/writing.cc ${CXX_FLAGS} -o ${N_WRITING}
	@echo Compiled writing.cc

	@${CXX} examples/reading.cc ${CXX_FLAGS} -o ${N_READING}
	@echo Compiled reading.cc

	@${CXX} examples/conversion.cc ${CXX_FLAGS} -o ${N_CONVERSION}
	@echo Compiled conversion.cc

	@echo Compiled successfully

clean:
	@echo Cleaning...
	@${CLEAN}
	@echo Cleaned succesfully...

all:
	@echo compile - Compiles the source
	@echo clean - Removes built files
