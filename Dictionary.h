#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class TrieNode {
public:
    char data;
    TrieNode** children;
    bool isTerminal;
    string meaning;
    string partOfSpeech;

    TrieNode(char data, string partOfSpeech, string meaning);
};

class Trie {
    TrieNode* root;

public:
    int count;

    Trie();

    bool insertHelper(TrieNode*& root, string word, string partOfSpeech, string meaning);
    void insertWord(string word, string partOfSpeech, string meaning);
    string searchHelper(TrieNode* root, string word);
    string search(string word);
    void updateHelper(TrieNode*& root, string word, string meaning = "\0");
    void update(string word, string meaning = "Dictionary.txt");
    void deleteWordHelper(TrieNode*& root, string word);
    void deleteWord(string word);
    void saveToFile(string filename);
    void saveToFileHelper(TrieNode* root, string currentWord, ofstream& output);
    void traverseTrie(TrieNode* node, const string& word);
    void searchPrefix(TrieNode* root, const string& prefix, string currentWord);
    string autosuggestion();
    void LoadDictionary();
};

class Console {
public:
    void displayMenu();
    int getUserChoice();
    void executeUserChoice(Trie& dictionary, int choice, string prefix);
};


#pragma once
