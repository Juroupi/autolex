test: test.cpp lexer.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS)

lexer.cpp: test.aut
	../autolex test.aut lexer

exec: test
	./test

clean:
	rm -f lexer.h lexer.cpp test