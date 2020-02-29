#include <iostream>
#include "matrix.h"
using namespace std;

string help(
                "\"fill\" to create and fill matrix\n"
                "\"multiply\" to multiply matrix and number\n"
                "\"exit\" to exit the program\n"
                "\"print\" to print matrix\n"
                "\"rows\" to print number of rows\n"
                "\"cols\" to print number of columns\n"
                );


int main() {
    matrix X;
    cout << "Print \"help\" for help\n";
    cout <<"> ";
    while (1) {
        string s;
        getline(cin, s);
        
        if (s == "help") {
            cout << help;
        }
        if (s == "print") {
            cout << X << endl;
        }
        if (s == "fill") {
            cout << "Enter matrix as a string:" << endl;
            string buf;
            getline(cin, buf);
            int i = 0;
            char str[1024];
            for (int k = 0; k<1024; k++)
                str[k] = '\0';
            while ((buf[i] != '\0') && (i != 1024)) {
                str[i] = buf[i];
                i++;
            }
            if (i == 1024) {
                cout << "Error. Use less symbols." << endl;
                continue;
            }
            try {
                matrix m(str);
                X = m;
                cout << "Completed."<< endl;
            }
            catch (error_catcher error) {
                cerr << error.what() << endl;
                //error.print();
            }
        }
        if (s == "rows") {
            cout << X.rows() << endl;
        }
        if (s == "cols") {
            cout << X.columns() << endl;
        }
        if (s == "multiply") {
            cout << "Enter your number: " << endl;
            double d;
            cin >> d;
            try {
                X *= d;
                cout << "Completed. Use print to view result"<< endl;
            }
            catch (error_catcher error) {
                cerr << error.what() << endl;
                //error.print();
            }
        }
        if (s == "exit") {
            cout <<"Program is finishing. Bye!"<<endl;
            break;
        }
    }
    return 0;
}