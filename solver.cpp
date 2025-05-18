#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

const string FILENAME = "wordlist.txt";
typedef vector<vector<string>> wordlist;
typedef vector<vector<pair<char,char>>> grid;

// Prints the puzzle with colors for each different word
void printPuzzle(const grid& puzzle){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 6; j++){
            switch(puzzle[i][j].second){
                case '1':
                    cout << "\033[38;5;1m";
                    break;
                case '2':
                    cout << "\033[38;5;2m";
                    break;
                case '3':
                    cout << "\033[38;5;3m";
                    break;
                case '4':
                    cout << "\033[38;5;4m";
                    break;
                case '5':
                    cout << "\033[38;5;5m";
                    break;
                case '6':
                    cout << "\033[38;5;159m";
                    break;
                case '7':
                    cout << "\033[38;5;200m";
                    break;
                case '8':
                    cout << "\033[38;5;214m";
                    break;
                case '9':
                    cout << "\033[38;5;141m";
                    break;
                default:
                    cout << "\033[0m";
            }
            cout << puzzle[i][j].first << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Prints solutions to the puzzle
void getSolution(int x, int y, int color, string word, const wordlist& words, grid puzzle){
    
}

int main(){
    // Get the puzzle
    grid puzzle;
    cout << "Enter each line of the puzzle with no spaces." << endl;
    for(int i = 0; i < 8; i++){
        vector<pair<char,char>> row;
        string buffer;
        cout << "Line " << i+1 << ": ";
        getline(cin, buffer);
        for(int j = 0; j < 6; j++){
            pair<char,char> item = {buffer[j],'0'};
            row.push_back(item);
        }
        puzzle.push_back(row);
    }
    
    // Read in valid words to their respective length list
    ifstream file(FILENAME);
    wordlist words;
    for(int i = 0; i < 7; i++){
        // Lists for 3, 4, 5, 6, 7, 8, and 9+ letter words
        vector<string> list;
        words.push_back(list);
    }

    // Read in file
    string word;
    if(!file){
        cout << "Error opening word list" << endl;
        return 1;
    }
    while (file >> word){
        if(word.length() >= 3){
            if(word.length() < 9) words[word.length()-3].push_back(word);
            else words[6].push_back(word);
        }
    }
    file.close();

    // Solve the puzzle
    string word = "";
    getSolution(0, 0, 1, word, words, puzzle);
    cout << "\n--- Solution ---\n";
    printPuzzle(puzzle);

    /* // Print loaded words (optional)
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 10; j++){
            cout << words[i][j] << endl;
        }
        cout << endl;
    } */

    return 0;
}
