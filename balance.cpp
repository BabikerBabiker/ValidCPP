#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <stack>

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
                if (d == '\n') {
                    lineNum++;
                    cout << "unmatched symbol ' at line " << lineNum << endl;
                    return false;
                }
                else if (d == '\'') {
                    closingQuote = true;
                    cout << "pair matching ' and ' found" << endl;
                    break;
                }
            }
            if (!closingQuote) {
                cout << "unmatched symbol ' at line " << lineNum << endl;
                return false;
            }
        }

        // DOUBLE QUOTES
        if (c == '\"') {
            while (dictfile.get(d)) {
                if (d == '\n') {
                    lineNum++;
                }

                if (d == '\"') {
                    cout << "pair matching " << c << " and " << d << " found" << endl;
                    break;
                }
            }
        }

        // PARENTHESIS, BRACKETS, AND BRACES
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
            lines.push(lineNum);
        }
        else if (c == ')' || c == ']' || c == '}') {
            if (s.empty()) {
                cout << "unmatched  " << c << " symbol on line " << lineNum << endl;
                return false;
            }
            char d = s.top();
            s.pop();

            if ((d == '(' && c == ')') || (d == '[' && c == ']') || (d == '{' && c == '}')) {
                cout << "pair matching " << d << " and " << c << " found" << endl;
            }
            else {
                cout << "unmatched " << c << " symbol on line " << lineNum << endl;
                return false;
            }
        }
        else if (c == '/' && dictfile.peek() == '/') {
            dictfile.ignore(256, '\n');
            lineNum++;
        }
        else if (c == '/' && dictfile.peek() == '*') {
            dictfile.ignore();

            while (dictfile.get(d)) {
                if (d == '\n') {
                    lineNum++;
                }

                if (d == '*' && dictfile.peek() == '/') {
                    dictfile.ignore();
                    cout << "pair matching /* and */ found" << endl;
                    break;
                }
            }
        }
    }

    // Check for unmatched opening symbols
    while (!s.empty()) {
        cout << "unmatched symbol " << s.top() << " at line " << lines.top() << endl;
        s.pop();
        lines.pop();
        return false;
    }

    return true;
}

int main() {
    int choice = 0;
    ifstream infile;
    string filename;

    while (choice != 4) {
        cout << "MENU FOR FILE BALANCE" << endl;
        cout << "---------------------" << endl;
        cout << "1. Load File" << endl;
        cout << "2. Print File Content" << endl;
        cout << "3. Check File Balance" << endl;
        cout << "4. Exit Pogram" << endl;
        cout << "---------------------" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            system("cls");
            cout << "Please enter filename for C++ code: ";
            cin >> filename;
            infile.close();
            infile.open(filename.c_str());

            if (!infile)
                cout << "[ERROR] File not found!" << endl;
            else cout << "File found!" << endl;

            cout << "\n[ENTER] to go back to menu";

            cin.ignore();
            cin.ignore();
            system("cls");
            break;
        case 2:
            system("cls");
            printFile(infile);

            cout << "\n[ENTER] to go back to menu";
            cin.ignore();
            cin.ignore();

            system("cls");
            break;
        case 3:
            system("cls");
            infile.clear();
            infile.seekg(0, infile.beg);

            if (balanceFile(infile)) {
                cout << "File is balanced." << endl;
            }
            else
                cout << "File is unbalanced." << endl;

            cout << "\n[ENTER] to go back to menu";
            cin.ignore();
            cin.ignore();

            system("cls");
            break;
        case 4:
            exit(0);
        }
    }
}