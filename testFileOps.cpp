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

int main() {  
    // ensure vessel data exists  
    {  
        ofstream tmpV("vessel.txt", ios::app);  
    }  
    // ensure sailing file exists so ifstream won’t fail  
    {  
        ofstream tmpS("sailing.txt", ios::app);  
    }  
  
    // open vessel lookup file (read‑only)  
    ifstream vesselIn("vessel.txt");  
    if (!vesselIn) {  
        cerr << "Could not open vessel.txt for reading" << endl;  
        return 1;  
    }  
  
    // open sailing file for appending new sailings  
    ofstream sailingOut("sailing.txt", ios::app);  
    if (!sailingOut) {  
        cerr << "Could not open sailing.txt for appending" << endl;  
        return 1;  
    }  
  
    // open sailing file for reading/existence checks  
    ifstream sailingIn("sailing.txt");  
    if (!sailingIn) {  
        cerr << "Could not open sailing.txt for reading" << endl;  
        return 1;  
    }  
  
    int choice = -1;  
    do {  
        cout << "\n=== Sailing Module Test Harness ===" << endl;  
        cout << "1) Create a Sailing" << endl;  
        cout << "2) Delete a Sailing" << endl;  
        cout << "3) View Sailings Report" << endl;  
        cout << "4) Query a Sailing" << endl;  
        cout << "0) Exit" << endl;  
        cout << "Enter choice (0-4): ";  
        cin >> choice;  
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  
  
        switch (choice) {  
            case 1:  
                cout << "\n--- Create Sailing ---" << endl;  
                createSailing(vesselIn, sailingOut, sailingIn);  
                break;  
  
        case 2: {
            cout << "\n--- Delete Sailing ---" << endl;

            // 1) open original for reading
            sailingIn.clear();
            sailingIn.seekg(0, ios::beg);

            // 2) open temp for writing (trunc)
            ofstream tempOut("sailing_tmp.txt");
            if (!tempOut) {
                cerr << "Error: could not open sailing_tmp.txt for writing" << endl;
                break;
            }

            // 3) prompt and delete using streams
            promptToDeleteSailing(sailingIn, tempOut);

            // 4) close both
            sailingIn.close();
            tempOut.close();

            // 5) replace original
            remove("sailing.txt");
            if (rename("sailing_tmp.txt", "sailing.txt") != 0) {
                perror("Error renaming temp file");
                break;
            }

            // 6) reopen streams for next operations
            sailingIn.open("sailing.txt");
            if (!sailingIn) {
                cerr << "Error: could not reopen sailing.txt for reading" << endl;
                return 1;
            }
            sailingOut.close();
            sailingOut.open("sailing.txt", ios::app);
            if (!sailingOut) {
                cerr << "Error: could not reopen sailing.txt for appending" << endl;
                return 1;
            }
        }
        break; 
        
            case 3:  
                cout << "\n--- Sailings Report ---" << endl;  
                printReport(sailingIn);  
                break;  
  
            case 4:  
                cout << "\n--- Query a Single Sailing ---" << endl;  
                querySailing(sailingIn);  
                break;  
  
            case 0:  
                cout << "Exiting test harness." << endl;  
                break;  
  
            default:  
                cout << "Invalid option. Please enter 0-4." << endl;  
        }  
  
        if (choice != 0) {  
            cout << "\nPress Enter to return to menu...";  
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  
            // rewind sailingIn so next operation starts at the top  
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