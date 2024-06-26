#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <stack>
#include <vector>

using namespace std;

static void printFile(istream& dictfile) {
    string line;
    int lineNum = 1;

    while (getline(dictfile, line)) {
        cout << lineNum << "\t" << line << endl;
        lineNum++;
    }
}

bool balanceFile(istream& dictfile) {
    stack<char> s;
    stack<int> lines;
    char c{}, d{};
    int lineNum = 1;
    vector<string> errors;

    printFile(dictfile);
    dictfile.clear();
    dictfile.seekg(0, dictfile.beg);

    while (dictfile.get(c)) {
        if (c == '\n') {
            lineNum++;
        }

        // SINGLE QUOTES
        if (c == '\'') {
            bool closingQuote = false;
            while (dictfile.get(d)) {
                if (d == '\\') {
                    dictfile.get(d);
                } else if (d == '\'') {
                    cout << "pair matching ' and ' found" << endl;
                    closingQuote = true;
                    break;
                } else if (d == '\n') {
                    lineNum++;
                    errors.push_back("unmatched symbol ' at line " + to_string(lineNum));
                    closingQuote = true;
                    break;
                }
            }
            if (!closingQuote) {
                errors.push_back("unmatched symbol ' at line " + to_string(lineNum));
            }
        }

        // DOUBLE QUOTES
        if (c == '\"') {
            bool closingQuote = false;
            while (dictfile.get(d)) {
                if (d == '\\') {
                    dictfile.get(d);
                } else if (d == '\"') {
                    cout << "pair matching \" and \" found" << endl;
                    closingQuote = true;
                    break;
                } else if (d == '\n') {
                    lineNum++;
                }
            }
            if (!closingQuote) {
                errors.push_back("unmatched symbol \" at line " + to_string(lineNum));
            }
        }

        // PARENTHESIS, BRACKETS, AND BRACES
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
            lines.push(lineNum);
        } else if (c == ')' || c == ']' || c == '}') {
            if (s.empty()) {
                errors.push_back("unmatched symbol " + string(1, c) + " at line " + to_string(lineNum));
                continue;
            }
            char d = s.top();
            s.pop();
            lines.pop();

            if ((d == '(' && c == ')') || (d == '[' && c == ']') || (d == '{' && c == '}')) {
                cout << "pair matching " << d << " and " << c << " found" << endl;
            } else {
                errors.push_back("unmatched symbol " + string(1, c) + " at line " + to_string(lineNum));
            }
        } else if (c == '/' && dictfile.peek() == '/') { // Comments
            dictfile.ignore(256, '\n');
            lineNum++;
        } else if (c == '/' && dictfile.peek() == '*') {
            dictfile.ignore();
            bool closingComment = false;
            while (dictfile.get(d)) {
                if (d == '\n') {
                    lineNum++;
                } else if (d == '*' && dictfile.peek() == '/') {
                    dictfile.ignore();
                    cout << "pair matching /* and */ found" << endl;
                    closingComment = true;
                    break;
                }
            }
            if (!closingComment) {
                errors.push_back("unmatched /* at line " + to_string(lineNum));
            }
        }
    }

    // Check for unmatched opening symbols
    while (!s.empty()) {
        errors.push_back("unmatched symbol " + string(1, s.top()) + " at line " + to_string(lines.top()));
        s.pop();
        lines.pop();
    }

    if (!errors.empty()) {
        for (const string& error : errors) {
            cout << error << endl;
        }
        return false;
    }

    return true;
}

void cls() {
    for (int i = 0; i < 50; i++) {
        std::cout << "\n";
    }
}

int main() {
    int choice = 0;
    ifstream infile;
    string filename;
    string filepath = "../";

    while (choice != 4) {
        cout << "MENU FOR FILE BALANCE" << endl;
        cout << "---------------------" << endl;
        cout << "1. Load File" << endl;
        cout << "2. Print File Content" << endl;
        cout << "3. Check File Balance" << endl;
        cout << "4. Exit Program" << endl;
        cout << "---------------------" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            cls();
            cout << "Please enter filename for C++ code: ";
            cin >> filename;
            infile.close();
            infile.open(filepath + filename);

            if (!infile) {
                cout << "[ERROR] File not found!" << endl;
                cout << "Filename entered: " << filepath + filename << endl; // Debugging output
            } else {
                cout << "File found!" << endl;
            }

            cout << "\n[ENTER] to go back to menu";
            cin.ignore();
            cin.ignore();
            cls();
            break;
        case 2:
            cls();
            if (infile.is_open()) {
                printFile(infile);
            } else {
                cout << "[ERROR] No file loaded!" << endl;
            }

            cout << "\n[ENTER] to go back to menu";
            cin.ignore();
            cin.ignore();

            cls();
            break;
        case 3:
            cls();
            if (infile.is_open()) {
                infile.clear();
                infile.seekg(0, infile.beg);

                if (balanceFile(infile)) {
                    cout << "File is balanced." << endl;
                } else {
                    cout << "File is unbalanced." << endl;
                }
            } else {
                cout << "[ERROR] No file loaded!" << endl;
            }

            cout << "\n[ENTER] to go back to menu";
            cin.ignore();
            cin.ignore();

            cls();
            break;
        case 4:
            exit(0);
        }
    }
}