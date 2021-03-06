.PHONY: all clean

CXX = g++ -std=c++14
CXXFLAGS = -Wall -Wextra -O3 -m64 -g -I./include
LFLAGS = -pthread

objects = src/tmId.o \
		  src/turingMachine.o \
		  src/interactiveMarkovModel.o \
		  src/stringProcess.o \
		  src/markovTable.o \
		  src/parseArgs.o \
		  src/util.o \
		  src/sptm.o \
		  src/read_input.o \
 		  src/main.o

objects2 = src/tmId.o \
		  src/turingMachine.o \
		  src/interactiveMarkovModel.o \
		  src/stringProcess.o \
		  src/markovTable.o \
		  src/parseArgs.o \
		  src/util.o \
		  src/sptm.o \
		  src/read_input.o \
 		  src/main2.o 

objects3 = src/tmId.o \
		  src/markovTable.o \
		  src/util.o \
		  src/parseArgs.o \
 		  src/main3.o 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

sptm: $(objects)
	$(CXX) $(LFLAGS) $^ -o $@

run: $(objects2)
	$(CXX) $(LFLAGS) $^ -o $@

complextm: $(objects3)
	$(CXX) $(LFLAGS) $^ -o $@

all: sptm \
	 run \
	 complextm

clean:
	rm -f src/*.o sptm
	rm -f src/*.o run
	rm -f src/*.o complextm

# Dependencies
main.o: src/main.cpp src/parseArgs.h src/stringProcess.h

main2.o: src/main2.cpp src/interactiveMarkovModel.h

main3.o: src/main3.cpp src/parseArgs.h

src/tmId.o: src/tmId.cpp src/tmId.h 

src/util.o: src/util.cpp src/util.h

src/stringProcess.o: src/stringProcess.cpp src/stringProcess.h src/markovTable.h

src/turingMachine.o: src/turingMachine.cpp src/turingMachine.h src/tmId.h

src/parseArgs.o: src/parseArgs.cpp src/parseArgs.h src/util.h

src/markovTable.o: src/markovTable.cpp src/markovTable.h src/turingMachine.h src/util.h

src/interactiveMarkovModel.o: src/interactiveMarkovModel.cpp src/interactiveMarkovModel.h src/markovTable.h

src/sptm.o: src/sptm.cpp src/sptm.h 

src/read_input.o: src/read_input.cpp src/read_input.h 