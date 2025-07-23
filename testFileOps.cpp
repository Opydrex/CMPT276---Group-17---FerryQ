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
    {
        ofstream outV(fileNameVessel, ios::app);
        if (!outV) {
            cerr << "Could not open " << fileNameVessel << " for writing" << endl;
            return 1;
        }
        Vessel v1("SpiritBC", 20.0f, 10.0f);
        v1.writeVessel(outV);
    }

    int choice = -1;
    do {
        cout << "\n=== Sailing Module Test Harness ===" << endl;
        cout << "1) Create a Sailing" << endl;
        cout << "2) Delete a Sailing" << endl;
        cout << "3) View Sailings Report" << endl;
        cout << "0) Exit" << endl;
        cout << "Enter choice (0-3): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "\n--- Create Sailing ---" << endl;
                createSailing();
                break;

            case 2:
                cout << "\n--- Delete Sailing ---" << endl;
                deleteSailing();
                break;

            case 3:
                cout << "\n--- Sailings Report ---" << endl;
                printReport();
                break;

            case 0:
                cout << "Exiting test harness." << endl;
                break;

            default:
                cout << "Invalid option. Please enter 0-3." << endl;
        }

        if (choice != 0) {
            cout << "\nPress Enter to return to menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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