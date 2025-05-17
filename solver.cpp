#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;

// Check whether a word contains any of certain letters
bool containsAny(const string& word, const string& letters){
    for(char c : letters){
        if(word.find(c) != string::npos){
            return true; // found at least one letter
        }
    }
    return false;
}

// Function to check if a word contains adjacent letters on the same side
bool sameSideLetters(const string& word, unordered_map<char,int> ss){
    for (int i = 0; i < word.size(); ++i) {
        if (i > 0)
            if(ss[word[i]] == ss[word[i-1]]) return true;
        if (i + 1 < word.size())
            if(ss[word[i]] == ss[word[i+1]]) return true;
    }
    return false;
}

// Prints solutions to the puzzle
void getSolutions(const vector<string>& words, string currSol, int nw, int usedlen){
    // Print the solution if it includes every used letter
    unordered_set<char> sol;
    int currnw = 1;
    for(char c : currSol){
        if(c != ' ') sol.insert(c);
        else currnw++;
    }
    if(sol.size() == usedlen) cout << currSol << endl;
    
    // If not solved and less than numWords, add another word and try again
    else if(currnw < nw){
        for(const auto& w : words){
            if(currSol[currSol.length()-1] == w[0]){
                string next = currSol + " " + w;
                getSolutions(words,next,nw,usedlen);
            }
        }
    }
}

int main(){
    // Get the used letters
    string used, unused;
    int numWords;
    cout << "Enter the puzzle starting with the top side (e.g. abcdefghijkl): ";
    getline(cin, used);
    cout << "Enter max number of words you want to use: ";
    getline(cin, unused);
    numWords = stoi(unused);

    // Get the unused letters
    unordered_set<char> usedSet;
    unused = "";
    for(char c : used) usedSet.insert(tolower(c));
    for(char c = 'a'; c <= 'z'; ++c){
        if(usedSet.find(c) == usedSet.end()) unused += c;
    }

    // Get a map of used letters on the same side of the puzzle
    unordered_map<char,int> sameSide;
    for(int i = 0; i < used.length(); ++i) sameSide[used[i]] = i / 3;
    
    // Read in valid words (3+ letters, no unused letters, no same side letters)
    ifstream file("wordlist2.txt");
    vector<string> words;
    string word;
    if(!file){
        cout << "Error opening wordlist2.txt" << endl;
        return 1;
    }
    while (file >> word){
        if(word.length() >= 3 && !containsAny(word,unused) && !sameSideLetters(word,sameSide))
            words.push_back(word);
    }
    file.close();

    // Print out solutions less than or equal to numWords
    for(const auto& w : words){
        getSolutions(words,w,numWords,used.length());
    }
    
    /* // Print loaded words (optional)
    cout << "Loaded " << words.size() << " words." << endl;
    for(const auto& w : words){
        cout << w << endl;
    } */

    return 0;
}
