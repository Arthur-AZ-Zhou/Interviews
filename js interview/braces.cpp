#include <bits/stdc++.h>
using namespace std;

class CodeEditor {
public:
    CodeEditor(const vector<string>& lines) : codeLines(lines) {}
    
    void identifyBlocks() { // Identify code blocks (each block is defined by its starting line and ending line)
        stack<pair<char, int>> codeBlockStack; // first: open bracket, second: starting line 

        for (int i = 0; i < codeLines.size(); i++) {
            string line = codeLines[i];

            for (char c : line) {
                if (c == '{') {
                    codeBlockStack.push({c, i});
                } else if (c == '}') {
                    if (codeBlockStack.empty()) {
                        //throw some error
                    }

                    allCodeBlocks[codeBlockStack.top().second] = i; // starting index of block -> current line we are on
                    collapsedBlocks[codeBlockStack.top().second] = false;
                    codeBlockStack.pop();
                }
            }   
        }
    }

    // Collapse or expand a block given the starting line number
    void toggleBlock(int startLine) {
        collapsedBlocks[startLine] = !collapsedBlocks[startLine];
    }

    // Print the code (collapsed blocks should be shown as "...")
    void printCode() {
        for (int i = 0; i < codeLines.size(); i++) {
            string line = codeLines[i];

            if (collapsedBlocks.find(i) == collapsedBlocks.end() || collapsedBlocks[i] == false) {
                cout << line << endl;
            } else {
                cout << line << "..." << endl;

                i = allCodeBlocks[i];
            }
        }
    }

private:
    vector<string> codeLines;
    unordered_map<int, int> allCodeBlocks; //start of code block -> end of code block
    unordered_map<int, bool> collapsedBlocks; //starting line num of block -> if collapsed (true if yes)
};

int main() {
    vector<string> code = {
        "print(hello)",
        "",
        "for(int i=0; i<n; i++) {",
        "   if(x) {",
        "       doSomething();",
        "   }",
        "}",
        "",
        "print(goodbye)"
    };

    CodeEditor editor(code);

    editor.identifyBlocks();
    editor.printCode();

    cout << "\nToggling line 2...\n";
    editor.toggleBlock(2);
    editor.printCode();
}
