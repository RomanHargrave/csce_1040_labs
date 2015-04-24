// file: revString.cpp
// String manipulation example
// This is an inlab exercise and is incomplete

#include <iostream>
#include <strstream>   // for string streams
#include <string>
using namespace std;

int main()
{
    string line = "";
    string word;

    getline( cin, line );

    // open a stream bound to the line
    istrstream input( line.c_str() ); 

    while ( input >> word )
    {
        cout << word << endl;
    }

    return 0;
}
