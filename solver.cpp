#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

#include <windows.h> // Required for Sleep

using namespace std;

const string FILENAME = "wordlist.txt";
typedef unordered_set<string> wordlist;
typedef vector<vector<pair<char,char>>> grid;

// Prints the puzzle with colors for each different word
void printPuzzle(const grid& puzzle, int h, int w){
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
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
    cout << "\033[0m" << endl;
}

// Make sure you don't set a color not adjacent to another color
bool adjacentToColor(const grid& puzzle, int y, int x, char c, int h, int w){
    // Would return true if this is first time that color is used
    bool firstOfColor = true;
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            if(puzzle[i][j].second == c){
                // Check if the color is already on the board
                firstOfColor = false;
                // Check if adjacent (and not just x,y)
                if(((abs(i-y) <= 1) && (abs(j-x) <= 1)) && (i != y || j != x))
                    return true;
            }
        }
    }
    return firstOfColor;
}

// Check whether the sorted letters of each group can be found in a sorted dict
// (not perfect, but letter groups are likely to be words and it's fast)
bool validSolution(const wordlist& words, const grid& puzzle, int h, int w){
    // Build color groups
    unordered_map<char, string> colorGroups;
    for(int y = 0; y < h; ++y){
        for(int x = 0; x < w; ++x){
            char letter = puzzle[y][x].first;
            char color = puzzle[y][x].second;
            colorGroups[color] += letter;
        }
    }

    // Check if sorted group letters match any word in the dictionary
    for(const auto& [color, group] : colorGroups){
        string sortedGroup = group;
        sort(sortedGroup.begin(), sortedGroup.end());
        if(words.find(sortedGroup) == words.end()) return false;
    }
    return true;
}

// Prints solution to the puzzle
bool getSolution(const wordlist& words, grid& puzzle, int h, int w){
    // Try grid until the colors are all in touching groups
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            if(puzzle[y][x].second == '0'){
                for(char c = '1'; c < '9'; c++){
                    if(adjacentToColor(puzzle,y,x,c,h,w)){
                        puzzle[y][x].second = c;
                        if(getSolution(words,puzzle,h,w)) return true;
                        puzzle[y][x].second = '0';
                    }
                }
                return false;
            }
        }
    }

    // Check whether each group probably contains a word
    if(validSolution(words,puzzle,h,w)){
        cout << "\nSolution\n";
        printPuzzle(puzzle,h,w);
        return true;
    } else return false;
}

int main(){
    // Get the dimensions
    grid puzzle;
    int w, h;
    char x;
    cout << "Enter dimensions (WxH): ";
    cin >> w >> x >> h;
    cin.clear();
    cin.ignore(100,'\n');

    // Get the letters
    cout << "Enter each line of the puzzle with no spaces." << endl;
    for(int i = 0; i < h; i++){
        vector<pair<char,char>> row;
        string buffer;
        cout << "Line " << i+1 << ": ";
        getline(cin, buffer);
        for(int j = 0; j < w; j++){
            pair<char,char> item = {buffer[j],'0'};
            row.push_back(item);
        }
        puzzle.push_back(row);
    }
    
    // Read in valid words
    ifstream file(FILENAME);
    wordlist words;
    string word;
    if(!file){
        cout << "Error opening word list" << endl;
        return 1;
    }
    while (file >> word){
        if(word.length() >= 4){
            sort(word.begin(),word.end());
            words.insert(word);
        }
    }
    file.close();

    // Solve the puzzle and see how long it took
    auto start = chrono::high_resolution_clock::now();
    if(!getSolution(words, puzzle, h, w))
        cout << "No solution found." << endl;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Execution time: " << duration.count() << " s" << endl;

    /* // Print loaded words
    for(auto it = words.begin(); it != words.end(); it++) cout << *it << endl; */

    return 0;
}
