#include <iostream>
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "queue.h"
#include "vector.h"
#include "simpio.h"
#include "random.h"
using namespace std;

void printIntroduction();
void promptForFile(int& n, Map<Queue<string>, Vector<string> >& map);
void promptForNumberOfWords(int n, const Map<Queue<string>, Vector<string> >& map);
void generateRandomText(int numberOfWords, int n, const Map<Queue<string>, Vector<string> >& map);

int main() {
    printIntroduction();
    Map<Queue<string>, Vector<string> > map;
    int n;
    promptForFile(n, map);
    promptForNumberOfWords(n, map);
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

void promptForFile(int& n, Map<Queue<string>, Vector<string> >& map) {
    ifstream file;
    promptUserForFile(file, "Input file name? ");
    n = 0;
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
//            cout << prefixes << endl;
            map[prefixes].add(word);
            window.dequeue();
        }
        window.enqueue(word);
        wordCount ++;
    }
    file.close();
    Vector<string> edgeWordList = backWordList + frontWordList;
//    cout << edgeWordList << endl;
    for (int i = n; i < edgeWordList.size(); i++) {
        word = edgeWordList[i];
        if (window.size() == n) {
            prefixes = window;
//            cout << prefixes << endl;
            map[prefixes].add(word);
            window.dequeue();
        }
        window.enqueue(word);
    }
//    cout << map << endl;
}

void promptForNumberOfWords(int n, const Map<Queue<string>, Vector<string> >& map) {
    int numberOfWords = getInteger("# of random words to generate (0 to quit)? ");
    while (numberOfWords < n) {
        if (numberOfWords == 0) { // quit
            return;
        }
        cout << "N must be " << n << " or greater." << endl;
        numberOfWords = getInteger("# of random words to generate (0 to quit)? ");
    }
    generateRandomText(numberOfWords, n, map);
}

void generateRandomText(int numberOfWords, int n, const Map<Queue<string>, Vector<string> >& map) {
    string output = "";
    int randomKeyIndex = randomInteger(0, map.size() - 1);
    Queue<string> randomKey = map.keys()[randomKeyIndex];
//    cout << randomKey << endl;
    Queue<string> copyOfRandomKey = randomKey;
    while (!copyOfRandomKey.isEmpty()) {
        output += copyOfRandomKey.dequeue() + " ";
    }
//    cout << output << endl;
    for (int i = 0; i < numberOfWords - n; i++) {
        string randomValue = randomElement(map[randomKey]);
        randomKey.dequeue();
        randomKey.enqueue(randomValue);
        output += randomValue + " ";
    }
    cout << output << endl;
}
