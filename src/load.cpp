#include "automaton.h"

#include <iostream> // debug
#include <sstream>
#include <stdexcept>
#include <cctype>

using namespace std;

static bool isEmpty(const string& line) {

    for (char c : line) {

        if (!isspace(c)) {
            return false;
        }
    }

    return true;
}

static const char* skipSpaces(const char* c) {

    while (isspace(*c)) {
        c++;
    }

    return c;
}

static bool matchSection(const string& line, const string& section) {

    const char* cline = line.c_str();
    const char* csection = section.c_str();

    while (*(cline = skipSpaces(cline)) == *csection) {

        if (*cline == '\0') {
            break;
        }

        cline++, csection++;
    }

    return *cline == '\0' && *csection == '\0';
}

enum {
    UNDEFINED, INITIAL, TERMINALS, TRANSITIONS
};

void Automaton::parseFile(istream& file) {

    string line;
    int lineNum = 0;
    int section = UNDEFINED;

    try {

        while (getline(file, line)) {

            lineNum++;

            if (matchSection(line, "initial:")) {
                section = INITIAL;
            }

            else if (matchSection(line, "terminals:")) {
                section = TERMINALS;
            }

            else if (matchSection(line, "transitions:")) {
                section = TRANSITIONS;
            }

            else {

                istringstream lineStream(line);

                if (section == INITIAL) {
                    parseInitial(lineStream);
                }

                else if (section == TERMINALS) {
                    parseTerminal(lineStream);
                }

                else if (section == TRANSITIONS) {
                    parseTransition(lineStream);
                }

                else if (!isEmpty(line)) {
                    throw runtime_error("section not defined");
                }
            }
        }
    }

    catch (exception& e) {
        throw runtime_error("line " + to_string(lineNum) + ": " + e.what());
    }
    
    if (initial == nullptr) {
        throw runtime_error("initial state not defined");
    }
}

void Automaton::parseInitial(istream& line) {

    string stateName;

    if (line >> stateName) {

        if (initial == nullptr) {
            initial = &getState(stateName);
        }

        else {
            throw runtime_error("initial state already defined");
        }
    }
}

void Automaton::parseTerminal(istream& line) {

    string stateName, tokenName;

    if (line >> stateName) {

        if (line >> tokenName) {
            getState(stateName).token = &getToken(tokenName);
        }

        else {
            throw runtime_error("missing terminal token");
        }
    }
}

static bool parseMethod(istream& line) {

    string methodName;

    if (line >> methodName) {

        if (methodName == "push") {
            return true;
        }

        else if (methodName == "skip") {
            return false;
        }
    }

    throw runtime_error("missing transition method");

    return false;
}

static char getEscapedChar(string&& symbol) {

    if (symbol == "s") return ' ';
    if (symbol == "r") return '\r';
    if (symbol == "n") return '\n';
    if (symbol == "t") return '\t';
    if (symbol == "v") return '\v';

    throw runtime_error("invalid escaped char");
}

static void parseTransitionSymbol(int& lower, int& upper, const string&& symbol) {

    if (symbol.length() == 1) {
        lower = symbol[0];
        upper = symbol[0];
    }

    else if (symbol.length() == 3 && symbol[1] == '-') {
        lower = symbol[0];
        upper = symbol[2];
    }

    else if (symbol.length() > 1 && symbol[0] == '\\') {
        lower = getEscapedChar(symbol.substr(1));
        upper = lower;
    }

    else {
        throw runtime_error("invalid transition symbol");
    }
}

static void parseTransitionSymbol(int& lower, int& upper, bool& exclude, const string& symbol) {

    exclude = symbol.length() > 1 && symbol[0] == '!';

    if (exclude) {
        parseTransitionSymbol(lower, upper, symbol.substr(1));
    }

    else {
        parseTransitionSymbol(lower, upper, move(symbol));
    }
}

static bool parseTransitionSymbol(State& srcState, State& dstState, bool push, istream& line) {

    string symbol;

    if (line >> symbol) {

        int lower, upper;
        bool exclude;

        parseTransitionSymbol(lower, upper, exclude, symbol);

        srcState.addTransitions(dstState, push, exclude, lower, upper);

        return true;
    }

    return false;
}

static void parseTransitionSymbols(State& srcState, State& dstState, bool push, istream& line) {

    if (parseTransitionSymbol(srcState, dstState, push, line)) {
        while (parseTransitionSymbol(srcState, dstState, push, line));
    }

    else {
        throw runtime_error("missing transition symbol");
    }
}

void Automaton::parseTransition(istream& line) {

    string srcStateName, dstStateName;

    if (line >> srcStateName) {
    
        if (line >> dstStateName) {

            parseTransitionSymbols(
                getState(srcStateName),
                getState(dstStateName),
                parseMethod(line),
                line
            );
        }

        else {
            throw runtime_error("invalid transition");
        }
    }
}
