CXX = g++
CXXFLAGS = -O2

autolex: src/autolex.cpp build/automaton.o build/load.o build/save.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

build/%.o: src/%.cpp src/automaton.h build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

build:
	mkdir build

clean:
	rm -rf autolex build