//Rapid prototype of console/terminal/shell UI.
#include <iostream>
#include <string>
using namespace std;

int mainMenu();
void outputAllDatesTimes();
void handleListSubmenu();

int main() {
    int selection; int selection1; int capacity;
    string inputString;
    do {
        selection = mainMenu();
        switch (selection) {
        case 1:
            cout << "Enter Aircraft ID: ";
            cin.ignore();  //flush previous /n from input buffer.
            getline(cin, inputString);  //get multiword sentence into string object.
            //Alternately, fill charArray with cin.getline(charArray, 20);
            cout << "Enter Aircraft Capacity: ";
            cin >> capacity;
            cout << endl << "Aircraft has been entered!" << endl;
            break;
        case 2:
            cout << "[Not implemented!]" << endl;
            break;
        case 3:
            outputAllDatesTimes();
            cout << "Enter selection number: ";
            cin >> selection1;
            if (selection1 != 0) {
                cout << "Enter flying student name: ";
                cin >> inputString;
                cout << "Flying lesson created!" << endl;
            }
            else cout << "[Here enter other lesson date/time]" << endl;
            break;
        case 4:
            handleListSubmenu();
            break;
        case 0:
            break;
        }
    } while (selection != 0);
    //programEnding
    cin.ignore();
    return 0;
}
//Output main menu and get selection.
int mainMenu() {
    int mainSelection;
    cout << endl;
    cout << "Main menu:" << endl;
    cout << "1) Create Aircraft" << endl;
    cout << "2) Create Flying Student" << endl;
    cout << "3) Make Flying Lesson Reservation" << endl;
    cout << "4) List" << endl;
    cout << "0) Quit" << endl;
    cout << "Enter Selection: ";
    cin >> mainSelection;
    cout << endl;
    return mainSelection;
}

//Output selectable list of all date/times.
void outputAllDatesTimes() {
    cout << "Sailings: " << endl;
    cout << "1) June 12, 12pm" << endl;
    cout << "2) June 12, 6pm" << endl;
    cout << "3) June 13, 9am" << endl;
    cout << "4) June 14, 9am" << endl;
    cout << "5) June 15, 9am" << endl;
    cout << "6) June 16, 9am" << endl;
    cout << "7) June 16, 12pm" << endl;
    cout << "8) June 16, 6pm" << endl;
    cout << "9) June 17, 9am" << endl;
    cout << "10) June 18, 9am" << endl;
    cout << "11) June 19, 9am" << endl;
    cout << "12) June 20, 9am" << endl;
    cout << "0) Other" << endl;
}

void handleListSubmenu() {
    int listSubmenuSelection;
    cout << "List SubMenu:" <<
        endl << "1) List Aircraft" <<
        endl << "2) List Flying Students" <<
        endl << "Enter selection: ";
    cin >> listSubmenuSelection;
    cout << endl << "[A long list would come out here.]" << endl;
}

/*  Example of middle exit loop:
while(true)
    get input and do stuff;
    if(input==0)
break;   //best if starts under the 'w' of "while".
    doOtherStuff()
}

//If single break is not enough to get out of doubly-deep scope do this.
//Please ONLY use 'goto' to branch downward, ONLY to just past outer scope.
do {
    for(;;){ //another way to create infinite loop.
        getInputAndDoStuff();
        if(input==0)
goto beyondOuterScope;
        OtherStuff()
    }
} while (x > 3);
beyondOuterScope:  //This is a 'statement label' of the statement below.
cout << "other statements";
*/