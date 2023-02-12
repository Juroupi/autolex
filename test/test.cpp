#include <iostream>
#include <string>

#include "lexer.h"

using namespace std;

int main() {

    string value;

    while (value != "exit") {

        value.clear();
        
        int token = getToken(cin, value);

        if (token < 0) {
            cout << "INVALID(" << (char)cin.get() << ") " << endl;
        }

        else {
            cout << getTokenName(token) << "(" << value << ") " << endl;
        }
    }

    return 0;
}