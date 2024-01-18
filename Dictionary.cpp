#include "Dictionary.h"
#include <conio.h>
using namespace std;

TrieNode::TrieNode(char data, string partOfSpeech, string meaning) {
    this->data = data;
    children = new TrieNode * [27];
    for (int i = 0; i < 27; i++)
    {
        children[i] = NULL;
    }
    isTerminal = false;
    this->meaning = meaning;
    this->partOfSpeech = partOfSpeech;
}


Trie::Trie() {
    this->count = 0;
    root = new TrieNode('\0', "", "");
}

bool Trie::insertHelper(TrieNode*& root, string word, string partOfSpeech, string meaning) {
    if (word.size() == 0)
    {
        if (!root->isTerminal)
        {
            root->isTerminal = true;
            root->meaning = meaning;
            root->partOfSpeech = partOfSpeech;
            return true;
        }
        else
        {
            return false;
        }
    }
    int index;
    word[0] = tolower(word[0]);
    if (word[0] == ' ' || word[0] == '-')
    {
        index = 26;
    }
    else
    {
        index = word[0] - 'a';
    }
    TrieNode* child;

    if (root->children[index] != NULL)
    {
        child = root->children[index];
    }
    else
    {
        child = new TrieNode(word[0], partOfSpeech, meaning);
        root->children[index] = child;
    }

    return insertHelper(child, word.substr(1), partOfSpeech, meaning);
}

void Trie::insertWord(string word, string partOfSpeech, string meaning) {
    if (insertHelper(root, word, partOfSpeech, meaning))
    {
        this->count++;
    }
    saveToFile("Dictionary.txt");
}

string Trie::searchHelper(TrieNode* root, string word) {
    if (word.size() == 0 && root->isTerminal) {
        return ("(" + root->partOfSpeech + ") " + root->meaning);
    }
    int index;
    if (word[0] == ' ' || word[0] == '-')
    {
        index = 26;
    }
    else
    {
        index = word[0] - 'a';
    }
    if (root->children[index] == NULL)
    {
        return "Word not found!";
    }

    return searchHelper(root->children[index], word.substr(1));
}

string Trie::search(string word) {
    return searchHelper(root, word);
}

void Trie::updateHelper(TrieNode*& root, string word, string meaning)
{
    if (word.size() == 0 && root->isTerminal) {
        cout << "Meaning update." << endl;
        root->meaning = meaning;
    }
    int index;
    if (word[0] == ' ' || word[0] == '-')
    {
        index = 26;
    }
    else
    {
        index = word[0] - 'a';
    }
    if (root->children[index] == NULL)
    {
        cout << "Word not found.";
        return;
    }

    updateHelper(root->children[index], word.substr(1), meaning);
}

void Trie::update(string word, string meaning)
{
    updateHelper(root, word, meaning);
    saveToFile("Dictionary.txt");
}

void Trie::deleteWordHelper(TrieNode*& root, string word) {
    if (word.size() == 0)
    {
        if (root->isTerminal)
        {
            root->isTerminal = false;
            return;
        }
    }

    int index;
    if (word[0] == ' ' || word[0] == '-')
    {
        index = 26;
    }
    else
    {
        index = word[0] - 'a';
    }
    TrieNode* child;
    if (root->children[index] != NULL)
    {
        child = root->children[index];
    }
    else
    {
        // The word is not present in the Trie
        return;
    }

    deleteWordHelper(child, word.substr(1));
}

void Trie::deleteWord(string word) {
    deleteWordHelper(root, word);
    saveToFile("Dictionary.txt");
    cout << endl << "Word Deleted" << endl;
}

void Trie::saveToFile(string filename) {
    ofstream output(filename);
    saveToFileHelper(root, "", output);
    output.close();
}

void Trie::saveToFileHelper(TrieNode* root, string currentWord, ofstream& output) {
    if (root->isTerminal)
    {
        output << currentWord << " " << root->partOfSpeech << "  " << root->meaning << endl;
    }

    for (int i = 0; i < 27; i++)
    {
        if (root->children[i] != NULL)
        {
            if (i < 26)
            {
                saveToFileHelper(root->children[i], currentWord + char('a' + i), output);
            }
            else
            {
                saveToFileHelper(root->children[i], currentWord + ' ', output);
            }
        }
    }
}

void Trie::traverseTrie(TrieNode* node, const string& word) {
    if (node->isTerminal) {
        cout << word << endl;
    }
    for (int i = 0; i < 27; i++) {
        if (node->children[i] != nullptr) {
            char c;
            if (i < 26)
                c = 'a' + i;
            else
                c = ' ';
            traverseTrie(node->children[i], word + c);
        }
    }
}

void Trie::searchPrefix(TrieNode* root, const string& prefix, string currentWord) {
    TrieNode* curr = root;
    for (char c : prefix) {
        int index;
        if (c == '-' || c == ' ')
            index = 26;
        else
            index = c - 'a';
        if (curr->children[index] == nullptr) {
            return; // No suggestions found
        }
        curr = curr->children[index];
    }
    // Traverse the trie to find all suggestions
    traverseTrie(curr, currentWord + prefix);
}

string Trie::autosuggestion() {
    string prefix;
    cout << "Enter a word: ";
    while (true) {
        char c = _getche();
        if (c == '\r') { // Enter key pressed
            system("cls");
            return prefix;
            break;
        }
        else if (c == '\b') { // Backspace key pressed
            if (!prefix.empty()) {
                prefix.pop_back();
                system("cls"); // Clear console screen
                cout << "Enter a word: " << prefix << endl;
                cout << "================================" << endl;
                cout << "Suggestions:" << endl;
                cout << "================================" << endl;
                searchPrefix(root, prefix, "");
            }
        }
        else {
            prefix += c;
            system("cls"); // Clear console screen
            cout << "Enter a word: " << prefix << endl;
            cout << "================================" << endl;
            cout << "Suggestions:" << endl;
            cout << "================================" << endl;
            searchPrefix(root, prefix, "");
        }
    }
}

void Trie::LoadDictionary() {
    string word;
    string partOfSpeech;
    string meaning;
    ifstream infile;
    infile.open("Dictionary.txt");
    if (!infile)
    {
        cout << "Error in Opening File" << endl;
    }
    else
    {
        cout << "Loading Dictionary...." << endl;
        while (infile >> word >> partOfSpeech)
        {
            // Read the rest of the line for the meaning
            getline(infile, meaning);
            // Insert the word into the dictionary
            insertWord(word, partOfSpeech, meaning);
        }
    }
    infile.close(); // Close the file after reading
}

void Console::displayMenu() {
    system("cls"); // Clear screen for a clean display

    cout << "================================" << endl;
    cout << setw(15) << right << "Dictionary Program" << setw(15) << left << " " << endl;
    cout << "================================" << endl;
    cout << setw(5) << right << "1." << setw(25) << left << "Search Word" << endl;
    cout << setw(5) << right << "2." << setw(25) << left << "Insert Word" << endl;
    cout << setw(5) << right << "3." << setw(25) << left << "Update Word" << endl;
    cout << setw(5) << right << "4." << setw(25) << left << "Delete Word" << endl;
    cout << setw(5) << right << "5." << setw(25) << left << "Exit" << endl;
    cout << "================================" << endl;
}

int Console::getUserChoice() {
    int choice;
    cout << "Enter your choice (1-5): ";
    cin >> choice;
    cin.ignore();
    return choice;
}

void Console::executeUserChoice(Trie& dictionary, int choice, string prefix) {
    string partOfSpeech, meaning;

    switch (choice) {
    case 1:

        system("cls");
        cout << "Meaning: " << dictionary.search(prefix) << endl;
        break;
    case 2:
        cout << "Enter part of speech: ";
        getline(cin, partOfSpeech);
        cin.ignore();
        cout << "Enter meaning: ";
        getline(cin, meaning);
        cin.ignore();
        dictionary.insertWord(prefix, partOfSpeech, meaning);
        system("cls");
        cout << "Word inserted successfully!" << endl;
        break;
    case 3:
        cout << "Enter new meaning: ";
        getline(cin, meaning);
        dictionary.update(prefix, meaning);
        system("cls");
        cout << "Meaning updated successfully!" << endl;
        break;
    case 4:
        dictionary.deleteWord(prefix);
        system("cls");
        cout << "Word Deleted Successfully!" << endl;
        break;
    case 5:
        cout << "Exiting the Program. Goodbye!" << endl;
        exit(0);
    default:
        cout << "Invalid choice. Please Enter a Number between 1 and 5." << endl;
    }
}

