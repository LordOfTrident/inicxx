Example1 = writing
Example2 = reading
Example3 = conversion

SourceFolder = ./shell/
BinFolder = ../bin/
Include = ../
Extension = .cc
ExtensionHeader = .hh
SourceFiles = ${Example1}${Extension} ${Example2}${Extension} ${Example3}${Extension} 
HeaderFiles = ${Include}inicxx${ExtensionHeader}
AllFiles = ${SourceFIles} ${HeaderFiles}
Compiler = g++

ifeq (${OS},Windows_NT)
	Print = echo
    Msg_CreatingBinFolder = "* CMPL: Creating ${BinFolder}\n"
    Msg_Compiling1 = "* CMPL: Compiling ${Example1}${Extension}...\n"
    Msg_Compiling2 = "* CMPL: Compiling ${Example2}${Extension}...\n"
    Msg_Compiling3 = "* CMPL: Compiling ${Example3}${Extension}...\n"
    Msg_Compiled = "* CMPL: Compiled all examples successfully\n"
    MakeBinFolder = if not exist "${BinFolder}" mkdir ${BinFolder}
    CompilerPath = ./
else
	Print = printf
    Msg_CreatingBinFolder = "\e[0;36m* CMPL: \e[0;33mCreating ${BinFolder}\n\e[0m"
    Msg_Compiling1 = "\e[0;36m* CMPL: \e[0;33mCompiling ${Example1}${Extension}...\n\e[0m"
    Msg_Compiling2 = "\e[0;36m* CMPL: \e[0;33mCompiling ${Example2}${Extension}...\n\e[0m"
    Msg_Compiling3 = "\e[0;36m* CMPL: \e[0;33mCompiling ${Example3}${Extension}...\n\e[0m"
    Msg_Compiled = "\e[0;36m* CMPL: \e[0;32mCompiled all examples successfully\n"
    MakeBinFolder = mkdir -p ${BinFolder}
    CompilerPath = /usr/bin/${Compiler}
endif

compile: ${SourceFiles} ${CompilerPath}
	@${Print} ${Msg_CreatingBinFolder}
	@${MakeBinFolder}

	@${Print} ${Msg_Compiling1}
	@${Compiler} -I${Include} ${Example1}${Extension} -o ${BinFolder}${Example1} -O3 -Wall -std=c++17

	@${Print} ${Msg_Compiling2}
	@${Compiler} -I${Include} ${Example2}${Extension} -o ${BinFolder}${Example2} -O3 -Wall -std=c++17

	@${Print} ${Msg_Compiling3}
	@${Compiler} -I${Include} ${Example3}${Extension} -o ${BinFolder}${Example3} -O3 -Wall -std=c++17

	@${Print} ${Msg_Compiled}
