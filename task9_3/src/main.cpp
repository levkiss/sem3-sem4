#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "../include/base.h"
#include "../include/circle.h"
#include "../include/rectangle.h"
#include "../include/cut.h"

using namespace::std;

string help(
                "\\CIRCLE ------- Create circle\n"
                "\\REKT --------- Create rectangle :p\n"
                "\\CUT ---------- Create cutted figure\n"
                "\\PRCRCL ------- Print circle area\n"
                "\\PRREKT ------- Print rectangle area :p\n"
                "\\PRCUT -------- Print cutted area of figure\n"
                "\\HELP --------- Help\n"
                "\\EXIT --------- Leave\n\n> "
);

void newline() {
    std::cout << "\n";
}

int main() {
    cout << "Use '\\HELP' for more information\n> ";
	Circle cir;
	Rectangle rec;
	cut cu;

    while (1) {
        string s;
        getline(std::cin, s); 
        if (s == "\\HELP") {
            cout << help;
        }
        if (s == "\\CIRCLE") {
        	double rad;
            cout << "Enter radius:\n>>> ";
            cin >> rad;
            Circle td(rad);
            cir = td;
            cout << "> ";
        }
        if (s == "\\REKT") {
        	double edg;
            cout << "Enter edge:\n>>> ";
            cin >> edg;
            Rectangle td(edg);
            rec = td;
            cout << "> ";
        }
        if (s == "\\CUT") {
        	double edg;
        	double rad;
            cout << "Enter edge:\n>>> ";
            cin >> edg;
            cout << "Enter radius:\n>>> ";
            cin >> rad;
            cut td(edg,rad);
            cu = td;
            cout << "> ";
        }
        if (s == "\\PRCRCL") {
            cout << cir.calculateArea() << "\n> ";
        }
        if (s == "\\PRREKT") {
            cout << rec.calculateArea() << "\n> ";
        }
        if (s == "\\PRCUT") {
            cout << cu.calculateArea() << "\n> ";
        }
        if (s == "\\EXIT") {
            break;
        }
        //else {
       //     cout << "No such command.\n\n> ";
      //  }
    }
	cout << "Circle" << endl;
	cout << "The area of a circle with a " << cir.getRad() << " radius is: " << cir.calculateArea() << endl;
	cout << "\nRectangle" << endl;
	cout << "The area of a rectangle with an edge of " << rec.getEdge() << " is: " << rec.calculateArea() << endl;

	return 0;

}