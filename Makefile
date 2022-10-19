.PHONY: all clean

CXX = g++ -std=c++17
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
		  src/turingMachine.o \
		  src/interactiveMarkovModel.o \
 		  src/stringProcess.o \
		  src/markovTable.o \
		  src/util.o \
		  src/parseArgs.o \
		  src/read_input.o \
		  src/metrics.o \
		  src/nc.o\
		  src/tprl.o \
 		  src/main3.o 

nrc_object = src/tmId.o \
			src/turingMachine.o \
			src/interactiveMarkovModel.o \
			src/stringProcess.o \
			src/markovTable.o \
			src/parseArgs.o \
			src/util.o \
			src/read_input.o \
			src/nrc.o \
			src/compute_nrc.o

kld_object = src/tmId.o \
			src/turingMachine.o \
			src/interactiveMarkovModel.o \
			src/stringProcess.o \
			src/markovTable.o \
			src/parseArgs.o \
			src/util.o \
			src/read_input.o \
			src/kullbackLeiblerDivergency.o \
			src/kl_divergence_main.o

spark_object = src/tmId.o \
		  src/turingMachine.o \
		  src/interactiveMarkovModel.o \
		  src/stringProcess.o \
		  src/markovTable.o \
		  src/parseArgs.o \
		  src/util.o \
		  src/sptm.o \
		  src/read_input.o \
		  src/kullbackLeiblerDivergency.o\
		  src/loss.o\
		  src/node.o\
		  src/search.o\
		  src/spark.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

sptm: $(objects)
	$(CXX) $(LFLAGS) $^ -o $@

run: $(objects2)
	$(CXX) $(LFLAGS) $^ -o $@

tprl: $(objects3)
	$(CXX) $(LFLAGS) $^ -o $@

nrc: $(nrc_object)
	$(CXX) $(LFLAGS) $^ -o $@

kld: $(kld_object)
	$(CXX) $(LFLAGS) $^ -o $@

spark: $(spark_object)
	$(CXX) $(LFLAGS) $^ -o $@


all: sptm \
	 run \
	 tprl \
	 nrc \
	 kld \
	 spark 

clean:
	rm -f src/*.o sptm
	rm -f src/*.o run
	rm -f src/*.o tprl
	rm -f src/*o nrc
	rm -f src/*o nrc
	rm -f src/*o kld
	rm -f src/*.o spark
	

# Dependencies

main.o: src/main.cpp src/parseArgs.h src/stringProcess.h

main2.o: src/main2.cpp src/interactiveMarkovModel.h

main3.o: src/main3.cpp src/parseArgs.h

kld.o: src/kl_divergence_main.cpp src/parseArgs.h src/stringProcess.h src/kullbackLeiblerDivergency.h

compute_nrc.o: src/compute_nrc.cpp src/parseArgs.h src/stringProcess.h src/nrc.h

spark.o: src/spark.cpp src/parseArgs.h src/search.h

src/interactiveMarkovModel.o: src/interactiveMarkovModel.cpp src/interactiveMarkovModel.h src/markovTable.h

src/kullbackLeiblerDivergency.o: src/kullbackLeiblerDivergency.cpp src/kullbackLeiblerDivergency.h

src/loss.o: src/loss.cpp src/loss.h src/kullbackLeiblerDivergency.h

src/nc.o: src/nc.cpp src/nc.h src/markovTable.h src/metrics.h

src/node.o: src/node.h src/node.cpp

src/nrc.o: src/nrc.cpp src/nrc.h

src/markovTable.o: src/markovTable.cpp src/markovTable.h src/turingMachine.h src/util.h

src/metrics.o: src/metrics.cpp src/metrics.h

src/parseArgs.o: src/parseArgs.cpp src/parseArgs.h src/util.h

src/search.o: src/search.cpp src/search.h src/loss.h src/node.h

src/read_input.o: src/read_input.cpp src/read_input.h 

src/sptm.o: src/sptm.cpp src/sptm.h 

src/stringProcess.o: src/stringProcess.cpp src/stringProcess.h src/markovTable.h src/interactiveMarkovModel.h

src/turingMachine.o: src/turingMachine.cpp src/turingMachine.h src/tmId.h

src/tmId.o: src/tmId.cpp src/tmId.h

src/tprl.o: src/tprl.cpp src/tprl.h src/nc.h src/metrics.h  src/markovTable.h

src/util.o: src/util.cpp src/util.h








