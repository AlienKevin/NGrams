#include <iostream>
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "queue.h"
#include "vector.h"
#include "simpio.h"
using namespace std;

void printIntroduction();
void promptFoFile(Map<Queue<string>, vector<string> >& map);

int main() {
    printIntroduction();
    Map<Queue<string>, vector<string> > map;
    promptFoFile(map);
    cout << "Exiting." << endl;
    return 0;
}

void printIntroduction() {
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

void promptFoFile(Map<Queue<string>, vector<string> >& map) {
    ifstream file;
    promptUserForFile(file, "Input file name? ");
    int n = 0;
    while (n < 2) {
        n = getInteger("Value of N? ");
        if (n < 2) {
            cout << "N must be 2 or greater." << endl;
        }
    }
    string word;
    while (file >> word) {
        cout << word << endl;
    }
}
