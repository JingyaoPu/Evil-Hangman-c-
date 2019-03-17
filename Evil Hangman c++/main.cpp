
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <utility>
#include <vector>
#include "RBTreeMap.hpp"
// Any other classes you make

using namespace std;

bool debug;

bool PromptYesNoMessage(string msg) {
    while (true) {
        cout << msg << "(y/n) ";
        string response;
        cin >> response;
        if (response == "y")
            return true;
        if (response == "n")
            return false;
        cout << "Please enter y or n..." << endl;
    }
}

int PromptGuesses()
{
    while (true) {
        int len;
        cout << "Please enter how many guesses: ";
        cin >> len;
        if (len > 0)
            return len;
    }
}

int PromptWordLength()
{
    int len;
    cout << "Please enter word length: ";
    while(!(cin>>len)) {
        cin.clear();
        cin.ignore(999,'\n');
        cout<<"Invalid integer! Please enter a integer:";
    }
    return len;
}

string PromptGuess(string& guessedLetters)
{
    string guess = "";
    
    // Your code here...
    
    return guess;
}

char PromptLetter(string& guessedLetters){
    char letter;
    do{
        cout << "Guessed Letters:"<<guessedLetters<<endl;
        cout << "Enter Guess Letter: ";
        cin >> letter;
    }while(letter < 'a' || letter > 'z' || guessedLetters.find(letter) != string::npos);
    cin.clear();
    cin.ignore(999,'\n');
    return letter;
}

void PlayEvilHangman(string file)
{
    cout<<"Loading dictionary..."<<endl;
    ifstream infile;
    infile.open(file);
    if( !infile.is_open() ) {
        cout << "Unable to Load File" << endl;
        exit(1);
    }
    pair<string,set<string>> allWords;
    int wordLen;
    do{
        infile.clear();
        infile.seekg(0, ios::beg);
        wordLen = PromptWordLength();
        string word;
        while(infile >> word){
            if(word.size() == wordLen){
                allWords.second.insert(word);
            }
        }
    }while(allWords.second.empty()); //if no word with such len in dic
    for(int i=0;i<wordLen;i++){allWords.first.append("*");}
    int guesses = PromptGuesses();
    string guessedLetters = "";
    
    
    while(allWords.first.find('*') != string::npos && guesses != guessedLetters.size()){
        cout<<endl<<allWords.first<<endl;
        cout<<"Guesses left: "<<guesses-guessedLetters.size()<<endl;
        if(debug == true)
            cout<<"Candidate words: "<<allWords.second.size()<<endl;
        char guessLetter = PromptLetter(guessedLetters);
        //partition the word families.
        RBTree families = *new RBTree();
        for(string word:allWords.second){
            size_t found = word.find(guessLetter);
            vector<int> foundIndexs;
            //find all indexs of the letter in word.
            while(found != string::npos){
                foundIndexs.push_back(static_cast<int>(found));
                found=word.find(guessLetter,found+1);
            }
            //construct the Key
            string familyKey = allWords.first;
            for(auto i:foundIndexs){
                familyKey[i] = guessLetter;
            }
            //Add family into families
            //No such family in families
            if(families.get(familyKey) == nullptr){
                set<string> words = {word};
                families.insert(familyKey, words);
            }else{//insert word into the existed family
                families.get(familyKey)->info.insert(word);
            }
        }
        //print failies
        if(debug == true)
            families.print();
        // Find the family with the largest size
        pair<string,set<string>> largestFamily;
        Node *maxFamily = families.getMaxFamily();
        largestFamily.first = maxFamily->key;
        largestFamily.second = maxFamily->info;
        if(debug)
            cout<<"Max family size is "<<maxFamily->info.size()<< " for "<<maxFamily->key<<endl;
        //Narrow allWords
        allWords = largestFamily;
        //record the guessed letter
        guessedLetters.append(&guessLetter,1);
    }
    // decide the end state
    if(allWords.first.find('*') == string::npos){
        cout<<"You win!"<<endl;
    }
    else if(guesses == guessedLetters.size()){
        cout<<"You lose, run out of guesses"<<endl;
    }
}

int main()
{
    string file = "smallDictionary.txt"; //"dictionary.txt";
    //cout << "Enter dictionary file: ";
    //cin >> file;
    
    
    while (true) {
        debug = PromptYesNoMessage("Turn debugging on?");
        PlayEvilHangman(file);
        if (!PromptYesNoMessage("Would you like to play again ?"))
            break;
    }
}
