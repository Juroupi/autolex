#include <iostream>

#include "automaton.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2 || argc > 3) {
        cout << "usage: " << argv[0] << " <input file> <output file>" << endl;
    }
    
    else try {
        Automaton(argv[1]).save(argv[2]);
    }
    
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}