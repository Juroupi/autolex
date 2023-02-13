#include "automaton.h"

#include <cctype>

using namespace std;

static string extractName(const string& fileName) {

    size_t start = fileName.rfind('/');
    if (start == string::npos) start = fileName.rfind('\\');
    if (start == string::npos) start = 0;
    else start++;

    return string(fileName, start);
}

static string transformName(const string& name) {

    string tName = name;

    for (char& c : tName) {

        if (isalnum(c)) {
            c = toupper(c);
        }

        else {
            c = '_';
        }
    }

    return tName;
}

static void printTokenEnum(const set<string>& tokens, ostream& stream) {

    stream << "enum {" << endl;

    for (const string& token : tokens) {
        stream << "\t" << token << "," << endl;
    }

    stream << "};" << endl << endl; 
}

static void printTokenNames(const set<string>& tokens, ostream& stream) {

    stream << "static const std::string tokenNames[] = {" << endl;

    for (const string& token : tokens) {
        stream << "\t\"" << token << "\"," << endl;
    }

    stream << "};" << endl << endl; 
}

void State::print(ostream& stream) {

    stream << endl << "S" << this << ":" << endl;
    
    if (transitions.size() > 0) {
        stream << "\tc = s.peek();" << endl;
    }

    for (Transition& transition : transitions) {
        
        if (transition.lowerSymbol == transition.upperSymbol) {
            if (transition.exclude) {
                stream << "\tif (c != " << (int)transition.lowerSymbol << ") { ";
            } else {
                stream << "\tif (c == " << (int)transition.lowerSymbol << ") { ";
            }
        }

        else if (transition.exclude) {
            stream << "\tif (c < " << (int)transition.lowerSymbol <<
                " || c > " << (int)transition.upperSymbol << ") { ";
        }
        
        else {
            stream << "\tif (c >= " << (int)transition.lowerSymbol <<
                " && c <= " << (int)transition.upperSymbol << ") { ";
        }

        if (transition.push) {
            stream << "PUSH(c); ";
        }

        else {
            stream << "SKIP(c); ";
        }

        stream << "goto S" << transition.dest << "; }" << endl;
    }

    stream << "\treturn " << (token == nullptr ? "-1" : *token) << ";" << endl;
}

void Automaton::printStates(ostream& stream) {

    initial->print(stream);

    for (auto& it : states) {

        State& state = it.second;

        if (&state != initial) {
            state.print(stream);
        }
    }
}

void Automaton::save(const string& fileName, ostream& hstream, ostream& cstream) {

    /* file.h */

    string name = extractName(fileName);
    string tName = transformName(name);

    hstream << "#ifndef _" << tName << "_LEXER_H_" << endl;
    hstream << "#define _" << tName << "_LEXER_H_" << endl << endl;

    hstream << "#include <string>" << endl;
    hstream << "#include <istream>" << endl << endl;

    printTokenEnum(tokens, hstream);

    hstream << "const std::string& getTokenName(int token);" << endl << endl;
    hstream << "int getToken(std::istream& stream, std::string& value);" << endl << endl;

    hstream << "#endif" << endl;

    /* file.cpp */

    cstream << "#include \"" << name << ".h\"" << endl << endl;

    cstream << "#define SKIP(c) s.ignore()" << endl;
    cstream << "#define PUSH(c) (s.ignore(), v.push_back(c))" << endl << endl;

    printTokenNames(tokens, cstream);

    cstream << "int getToken(std::istream& s, std::string& v) {" << endl << endl;
    cstream << "\tchar c;" << endl;
    printStates(cstream);
    cstream << "}" << endl << endl;

    cstream << "const std::string& getTokenName(int token) {" << endl;
    cstream << "\tstatic const std::string empty;" << endl;
    cstream << "\treturn (token >= 0 && token < " << tokens.size() << ") ? tokenNames[token] : empty;" << endl;
    cstream << "}" << endl;
}