CXX = g++
CXXFLAGS = -Wall -Werror -ansi -pedantic 
#CXXFLAGS = -g

OBJECTS =
srcDIR="src"
MAIN = src/rshell.cpp
EXE = rshell
HEADERS = src/shell.h src/parse.h src/connector.h src/command.h src/commandline.h 
CPP = 
TAR = rshell.tar
TARLIST = rshell.cpp $(CPP) $(HEADERS) $(DICTLIST) $(EXE)
EXEDIR=bin
#createDIR :=$(bash mkdir -p $(EXEDIR))

all: $(HEADERS) $(MAIN)
	#Figure out a better way to do this
	mkdir -p $(EXEDIR)
	$(CXX) $(CXXFLAGS) -o $(EXEDIR)/$(EXE) $(MAIN)

rshell: $(HEADERS) $(MAIN)
	#Figure out a better way to do this
	mkdir -p $(EXEDIR)
	$(CXX) $(CXXFLAGS) -o $(EXEDIR)/$(EXE) $(MAIN)

run:
	make clean
	make all
	$(EXEDIR)/$(EXE) 

clear:
	clear
	clear
	clear	

# remove unnecessary files
clean:
	rm -rf *~ *.o a.out $(EXEDIR)/* $(EXEDIR) $(EXE)