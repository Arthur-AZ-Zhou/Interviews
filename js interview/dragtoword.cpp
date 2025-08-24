#include <bits/stdc++.h>
using namespace std;

class Keyboard {
    public:
        Keyboard(vector<string> keyboardLetters) : keyboardLetters(keyboardLetters) {}

        void dragToRawWord(vector<pair<int, int>> rawKeystrokes) {
            string rawWord = "";

            for (auto& [x, y] : rawKeystrokes) {
                rawWord += keyboardLetters[x][y];
            }

            cout << "Raw word is: " << rawWord << endl;
            rawWords.push_back(rawWord);
        }

        void dragToInferredWord(vector<pair<int, int>> rawKeystrokes) {
            string inferredWord = "";

            char prev = '\0';
            for (auto& [x, y] : rawKeystrokes) {
                char c = keyboardLetters[x][y];
                if (c != prev) { // skip consecutive duplicates
                    inferredWord += c;
                    prev = c;
                }
            }

            cout << "Inferred word is: " << inferredWord << endl;
            inferredWords.push_back(inferredWord);
        }

        void printKeyboardWords() {
            cout << "Printing raw words===============" << endl;

            for (string word : rawWords) {
                cout << word << " "; 
            }

            cout << endl << "Printing inferred words===============" << endl;

            for (string word : inferredWords) {
                cout << word << " "; 
            }
        }

    private:
        vector<string> keyboardLetters;
        vector<string> rawWords;
        vector<string> inferredWords;
};

int main() {
    vector<string> keyboard = {
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"
    };

    Keyboard phoneKeyboard(keyboard);

    vector<pair<int, int>> rawKeystrokes = { // assume these are within bounds
        {2, 2},
        {2, 1},
        {1, 1},
        {1, 0},
        {1, 1},
        {1, 2},
        {1, 3},
        {0, 3},
        {0, 4},
    };

    phoneKeyboard.dragToRawWord(rawKeystrokes);
    phoneKeyboard.dragToInferredWord(rawKeystrokes);
    
    phoneKeyboard.printKeyboardWords();
}