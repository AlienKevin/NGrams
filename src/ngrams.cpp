#include <iostream>
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "queue.h"
#include "vector.h"
#include "simpio.h"
using namespace std;

void printIntroduction();
void promptForFile(Map<Queue<string>, Vector<string> >& map);

int main() {
    printIntroduction();
    Map<Queue<string>, Vector<string> > map;
    promptForFile(map);
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

void promptForFile(Map<Queue<string>, Vector<string> >& map) {
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
    int totalWords = 0;
    while (file >> word) {
        totalWords ++;
    }

    // point istream back to beginning of file
    // https://bytes.com/topic/c/answers/881293-reading-file-multiple-times
    file.clear();
    file.seekg(0, ios::beg);

    Queue<string> window;
    Queue<string> prefixes;
    int wordCount = 0;
    Vector<string> frontWordList;
    Vector<string> backWordList;
    while (file >> word) {
        if (wordCount < n) {
            frontWordList.add(word);
        } else if (totalWords - wordCount <= n) {
            backWordList.add(word);
        }
        if (window.size() == n) {
            prefixes = window;
            map[prefixes].add(word);
            window.dequeue();
        }
        window.enqueue(word);
        wordCount ++;
    }
    Vector<string> edgeWordList = backWordList + frontWordList;
    cout << edgeWordList << endl;
    for (int i = 0; i < edgeWordList.size(); i++) {
        if (window.size() == n) {
            prefixes = window;
            map[prefixes].add(word);
            window.dequeue();
        }
        window.enqueue(word);
    }
    cout << map << endl;
}
