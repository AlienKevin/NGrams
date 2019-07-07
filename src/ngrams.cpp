#include <iostream>
#include "console.h"
using namespace std;

void printIntroduction();

int main() {
    printIntroduction();
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
