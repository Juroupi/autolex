#include "automaton.h"

#include <fstream>

using namespace std;

Transition::Transition(State* dest, bool push, bool exclude, char lowerSymbol, char upperSymbol) :
    dest(dest), push(push), exclude(exclude), lowerSymbol(lowerSymbol), upperSymbol(upperSymbol) {}

State::State(const string& name) :
    name(name) {}

void State::addTransitions(State& dest, bool push, bool exclude, char symbol) {
    addTransitions(dest, push, exclude, symbol, symbol);
}

void State::addTransitions(State& dest, bool push, bool exclude, char lowerSymbol, char upperSymbol) {
    transitions.emplace_back(&dest, push, exclude, lowerSymbol, upperSymbol);
}

const vector<Transition>& State::getTransitions() const {
    return transitions;
}

State& Automaton::getState(const string& name) {
    auto it = states.emplace(name, name);
    return it.first->second;
}

const string& Automaton::getToken(const string& name) {
    auto it = tokens.emplace(name);
    return *it.first;
}

Automaton::Automaton(const string& fileName) {

    ifstream file(fileName);

    if (!file.good()) {
        throw runtime_error("can't open file '" + fileName + "'");
    }

    parseFile(file);
}

Automaton::Automaton(istream& file) {
    parseFile(file);
}
    
void Automaton::save(const string& fileName) {

    ofstream hFile(fileName + ".h");

    if (!hFile.good()) {
        throw runtime_error("can't open file '" + fileName + ".h'");
    }

    ofstream cFile(fileName + ".cpp");

    if (!cFile.good()) {
        throw runtime_error("can't open file '" + fileName + ".cpp'");
    }

    save(fileName, hFile, cFile);
}