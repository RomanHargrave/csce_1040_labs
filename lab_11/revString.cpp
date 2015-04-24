#include <string>
#include <stack>
#include <iostream>
#include <sstream>

using namespace std;

int main ( void ) {
    
    while (!cin.eof()) {
        string toReverse;
        getline( cin, toReverse );

        {
            stack<string> tokenPile;
            stringstream stream(toReverse);

            while (!stream.eof()) {
                string token;
                stream >> token;
                tokenPile.push(token);
            }

            while(!tokenPile.empty()) {
                cout << tokenPile.top() << " ";
                tokenPile.pop();
            }

            cout << endl;

        }
    }

    return 0;
}
