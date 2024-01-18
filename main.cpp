#include "Dictionary.h"

int main() {
    Trie dictionary;
    dictionary.LoadDictionary(); // Load initial data from the file
    system("cls");
    Console obj;

    string prefix;
    char choice;

    while (true) {
        system("cls");
        obj.displayMenu();
        choice = obj.getUserChoice();
        // Use autosuggestion to get the word from the user
        if (choice < 5)
            prefix = dictionary.autosuggestion();
        obj.executeUserChoice(dictionary, choice, prefix);
        // Press any key to continue...

        system("pause");
        system("cls");
    }

    return 0;
}
