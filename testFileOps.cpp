//==========================================================================
//==========================================================================
/*
MODULE NAME: testFileOps.cpp
Rev.1 - 17/07/2025 - test file operations module created
----------------------------------------------------------------------------
This module contains the operations and functions that will test the program
----------------------------------------------------------------------------
*/

//==========================================================================
//==========================================================================
/*
MODULE NAME: testFileOps.cpp
Rev.1 - 17/07/2025 - test file operations module created
----------------------------------------------------------------------------  
This module contains the operations and functions that will test the program
----------------------------------------------------------------------------  
*/

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <limits>
#include "Vessel.h"
#include "Vehicle.h"
#include "Sailing.h"
using namespace std;

bool validSailingId(const string& s) {  
    if (s.size() != 9) return false;  
    if (s[3] != '-' || s[6] != '-') return false;  
    for (int i = 0; i < 3; ++i) if (!isalpha(s[i])) return false;  
    for (int i : {4,5,7,8})      if (!isdigit(s[i])) return false;  
    return true;  
}  
  
int main() {  
    // make sure data files exist  
    { ofstream tmp("vessel.txt", ios::app); }  
    { ofstream tmp("sailing.txt", ios::app); }  
  
    // open vessel file for lookup  
    ifstream vesselIn("vessel.txt");  
    if (!vesselIn) {  
        cerr << "Could not open vessel.txt for reading\n";  
        return 1;  
    }  
  
    // persistent streams for creates & reports  
    ofstream sailingOut("sailing.txt", ios::app);  
    if (!sailingOut) {  
        cerr << "Could not open sailing.txt for appending\n";  
        return 1;  
    }  
    ifstream sailingIn("sailing.txt");  
    if (!sailingIn) {  
        cerr << "Could not open sailing.txt for reading\n";  
        return 1;  
    }  
  
    int choice = -1;  
    do {  
        cout << "\n=== Sailing Module Test Harness ===\n";  
        cout << "1) Create a Sailing\n";  
        cout << "2) Delete a Sailing\n";  
        cout << "3) View Sailings Report\n";  
        cout << "4) Query a Sailing\n";  
        cout << "0) Exit\n";  
        cout << "Enter choice (0-4): ";  
        cin >> choice;  
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  
  
        switch (choice) {  
            case 1:  
                cout << "\n--- Create Sailing ---\n";  
                createSailing(vesselIn, sailingOut, sailingIn);  
                break;  
  
            case 2: {
                cout << "\n--- Delete Sailing ---\n";
                // 0) Close the persistent handles so we can delete/rename the file
                sailingIn.close();
                sailingOut.close();

                while (true) {
                    // 1) Open fresh streams on the file
                    ifstream inFile("sailing.txt");
                    ofstream outFile("sailing_tmp.txt", ios::trunc);
                    if (!inFile || !outFile) {
                        cerr << "Error opening files\n";
                        break;
                    }

                    // 2) Prompt for ID, filter one record
                    bool ok = deleteSailing(outFile, inFile);
                    inFile.close();
                    outFile.close();

                    // 3) Now that no handles are open, this will succeed
                    remove("sailing.txt");
                    rename("sailing_tmp.txt", "sailing.txt");

                    cout << "Would you like to delete another sailing? (Y/N) ";
                    string resp;
                    getline(cin, resp);
                    if (resp.empty() || (resp[0] != 'Y' && resp[0] != 'y'))
                        break;
                }

                // 4) Reopen persistent streams for the rest of the harness
                sailingOut.open("sailing.txt", ios::app);
                sailingIn.open("sailing.txt");
                break;
            }  
            case 3:  
                printReport(sailingIn);  
                break;  
  
            case 4:  
                cout << "\n--- Query a Single Sailing ---\n";  
                querySailing(sailingIn);  
                break;  
  
            case 0:  
                cout << "Exiting test harness.\n";  
                break;  
  
            default:  
                cout << "Invalid option. Please enter 0-4.\n";  
        }  
  
        if (choice != 0) {  
            cout << "\nPress Enter to return to menu...";  
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  
            sailingIn.clear();  
            sailingIn.seekg(0, ios::beg);  
        }  
    } while (choice != 0);  
  
    return 0;  
}

// int main(){
//     // Declare the stream objects
//     // Step 1: open for reading to check duplicates
//     ifstream myInputVesselFile("vessel.txt");
//     if (!myInputVesselFile) {
//         cerr << "Could not open vessel.txt for reading" << endl;
//         return 1;
//     }



//     ofstream myOutputVesselFile("vessel.txt", ios::app);
//     if (!myOutputVesselFile) {
//         cerr << "Could not open vessel.txt for appending" << endl;
//         return 1;
//     }

// // Now call your logic
//     createVessel(myInputVesselFile, myOutputVesselFile);



//     //testing vehicle
//     ifstream myInputVehicleFile("vehicle.txt");
//     if (!myInputVehicleFile) {
//         cerr << "Could not open vehicle.txt for reading" << endl;
//         return 1;
//     }



//     ofstream myOutputVehicleFile("vehicle.txt", ios::app);
//     if (!myOutputVesselFile) {
//         cerr << "Could not open vessel.txt for appending" << endl;
//         return 1;
//     }
//     Vehicle newVehicle("cmpt276", 2, 6);

//     newVehicle.writeVehicle(myOutputVehicleFile);
//     cout<<isVesselExist("cmpt276",myInputVehicleFile);


// // Clean up
//     myInputVesselFile.close();
//     myOutputVesselFile.close();

// }