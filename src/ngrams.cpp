/**
 * NGrams assignment for CS106B at Stanford Summer Session.
 * Author: Kevin Li
 * Description: Randomly generate a new text in an author's writing style.
 * This program generates a map of prefixes to suffixes and use it to create
 * weighted random generation of texts.
 * Citations:
 * point istream back to beginning of file
 * https://bytes.com/topic/c/answers/881293-reading-file-multiple-times
 */
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
void promptForFile(int& n, Vector<Queue<string> >& startOfSentencePrefixes, bool& hasEndPunctuations, Map<Queue<string>, Vector<string> >& map);
void promptForNumberOfWords(int n, const Vector<Queue<string> >& startOfSentencePrefixes, bool hasEndPunctuations, const Map<Queue<string>, Vector<string> >& map);
void generateRandomText(int numberOfWords, int n, const Vector<Queue<string> >& startOfSentencePrefixes, bool hasEndPunctuations, const Map<Queue<string>, Vector<string> >& map);
bool isEndPunctuation(char ch);

int main() {
    printIntroduction();
    Map<Queue<string>, Vector<string> > map;
    int n;
    Vector<Queue<string> > startOfSentencePrefixes;
    bool hasEndPunctuations = false; // default to not have end punctuations
    promptForFile(n, startOfSentencePrefixes, hasEndPunctuations, map);
    promptForNumberOfWords(n, startOfSentencePrefixes, hasEndPunctuations, map);
    cout << "Exiting." << endl;
    return 0;
}

/**
 * @brief Print introduction text.
 */
void printIntroduction() {
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

/**
 * @brief Prompt user for input file and n value and construct a map of prefixes to suffixes.
 * @param n the size of grams to use, high n means more prefixes
 * @param map the map used to store prefixes to suffixes
 */
void promptForFile(int& n, Vector<Queue<string> >& startOfSentencePrefixes, bool& hasEndPunctuations, Map<Queue<string>, Vector<string> >& map) {
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

    file.clear();
    file.seekg(0, ios::beg);

    Queue<string> window;
    Queue<string> prefixes;
    int wordCount = 0;
    Vector<string> frontWordList;
    Vector<string> backWordList;
    while (file >> word) {
        if (isEndPunctuation(word[word.size() - 1])) {
            hasEndPunctuations = true;
        }
        if (wordCount < n) {
            frontWordList.add(word);
        } else if (totalWords - wordCount <= n) {
            backWordList.add(word);
        }
        if (window.size() == n) {
            prefixes = window;
            // isupper function
            // http://www.cplusplus.com/reference/cctype/isupper/
            if (isupper(prefixes.peek()[0])) { // first letter is uppercase
                startOfSentencePrefixes.add(prefixes);
            }
            map[prefixes].add(word);
            window.dequeue();
        }
        window.enqueue(word);
        wordCount ++;
    }
    file.close();
    Vector<string> edgeWordList = backWordList + frontWordList;
    for (int i = n; i < edgeWordList.size(); i++) {
        word = edgeWordList[i];
        if (window.size() == n) {
            prefixes = window;
            if (isupper(prefixes.peek()[0])) { // first letter is uppercase
                startOfSentencePrefixes.add(prefixes);
            }
            map[prefixes].add(word);
            window.dequeue();
        }
        window.enqueue(word);
    }
}

/**
 * @brief Prompt user for number of words they want to randomly generate.
 * @param n the size of grams to use, high n means more prefixes
 * @param map the map used to store prefixes to suffixes
 */
void promptForNumberOfWords(int n, const Vector<Queue<string> >& startOfSentencePrefixes, bool hasEndPunctuations, const Map<Queue<string>, Vector<string> >& map) {
    while (true) {
        int numberOfWords = getInteger("\n# of random words to generate (0 to quit)? ");
        while (numberOfWords < n) {
            if (numberOfWords == 0) { // quit
                return;
            }
            cout << "Must be at least " << n << " words." << endl;
            numberOfWords = getInteger("\n# of random words to generate (0 to quit)? ");
        }
        generateRandomText(numberOfWords, n, startOfSentencePrefixes, hasEndPunctuations, map);
    }
}

/**
 * @brief Generate random texts based on the number of words and the map of prefixes to suffixes.
 * @param numberOfWords the number of words to generate, must be >= n
 * @param n the size of grams to use, high n means more prefixes
 * @param map the map used to store prefixes to suffixes
 */
void generateRandomText(int numberOfWords, int n, const Vector<Queue<string> >& startOfSentencePrefixes, bool hasEndPunctuations, const Map<Queue<string>, Vector<string> >& map) {
    string output = "";
    Queue<string> randomKey;
    if (startOfSentencePrefixes.size() > 0) {
        randomKey = randomElement(startOfSentencePrefixes);
    } else {
        int randomKeyIndex = randomInteger(0, map.size() - 1);
        randomKey = map.keys()[randomKeyIndex];
    }
    Queue<string> copyOfRandomKey = randomKey;
    while (!copyOfRandomKey.isEmpty()) {
        output += copyOfRandomKey.dequeue() + " ";
    }
    string randomValue;
    for (int i = 0; i < numberOfWords - n; i++) {
        randomValue = randomElement(map[randomKey]);
        randomKey.dequeue();
        randomKey.enqueue(randomValue);
        output += randomValue + " ";
    }
    char lastChar = randomValue[randomValue.size() - 1];
    while (!isEndPunctuation(lastChar) && hasEndPunctuations) { // text not ended in full sentence
        randomValue = randomElement(map[randomKey]);
        randomKey.dequeue();
        randomKey.enqueue(randomValue);
        output += randomValue + " ";
        lastChar = randomValue[randomValue.size() - 1];
    }
    if (hasEndPunctuations && startOfSentencePrefixes.size() > 0) {
        cout << output << endl;
    } else if (startOfSentencePrefixes.size() > 0){
        cout << output << "..." << endl;
    } else {
        cout << "... " << output << "..." << endl;
    }
}

bool isEndPunctuation(char ch) {
    return (ch == '.' || ch == '?' || ch == '!');
}
