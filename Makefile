CXX = g++
CXXFLAGS = -O2

autolex: autolex.cpp build/automaton.o build/load.o build/save.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

build/%.o: %.cpp automaton.h build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

build:
	mkdir build

clean:
	rm -rf autolex build