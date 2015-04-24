#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

static const char VOWELS[] = { 'a', 'e', 'i', 'o', 'u' };

static bool isVowel(char subject) {
    cout << "vowel? " << subject << endl;
    return find(VOWELS, VOWELS + 5, subject) ? true : false;        
}

static string toPigLatin(string subject) {
    
    stringstream buffer;

    if (isVowel(subject.at(0))) {
        buffer << subject << "yay";   
    } else if(subject.size() > 1) {
        buffer << subject.substr(1, string::npos) << subject.at(0) << "ay";
    }

    return buffer.str();

}

int main (void) {

    while (!cin.eof()) {
        string line;
        getline( cin, line );

        {
            stringstream stream(line);

            while (!stream.eof()) {
                string word;
                stream >> word;
                cout << toPigLatin(word) << " ";
            }

            cout << endl;
        }
    }

    return 0;    

}
