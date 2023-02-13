#ifndef _AUTOMATON_H_
#define _AUTOMATON_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <istream>
#include <ostream>

class State;

struct Transition {

    State* const dest;
    const bool push, exclude;
    const char lowerSymbol, upperSymbol;

    Transition(State* dest, bool push, bool exclude, char lowerSymbol, char upperSymbol);
};

class State {

    std::vector<Transition> transitions;

public:

    const std::string name;
    const std::string* token = nullptr;

    State(const std::string& name);

    void addTransitions(State& dest, bool push, bool exclude, char symbol);
    void addTransitions(State& dest, bool push, bool exclude, char lowerSymbol, char upperSymbol);

    const std::vector<Transition>& getTransitions() const;

    void print(std::ostream& stream);
};

class Automaton {

    State* initial = nullptr;
    std::map<std::string, State> states;
    std::set<std::string> tokens;

    State& getState(const std::string& name);
    const std::string& getToken(const std::string& name);

    void parseFile(std::istream& file);
    void parseInitial(std::istream& line);
    void parseTerminal(std::istream& line);
    void parseTransition(std::istream& line);

    void printStates(std::ostream& stream);

public:

    Automaton(const std::string& fileName);
    Automaton(std::istream& stream);
    
    void save(const std::string& fileName);
    void save(const std::string& fileName, std::ostream& hstream, std::ostream& cstream);
};

#endif