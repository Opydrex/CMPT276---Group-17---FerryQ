// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: UserInterface.cpp
// Rev.3 - 05/08/2025 - Updated user input logic to correctly check for blank inputs.
//                    - Added and Implemented the trim() helper function
//                    - Functions now clear the terminal before outputting their result.
// Rev.2 - 22/07/2025 - Updated to match new module signatures and added Query Sailing.
// Rev.1 - 21/07/2025 - UserInterface class implementation.
//
//--------------------------------------------------------------------------
// This module contains the main user interface loop and all menu navigation logic.
//
// What it does:
// - Displays the main menu and sub-menus for Sailings and Bookings.
// - Uses a robust getline/stoi pattern to handle user input safely, preventing
//   crashes from invalid (non-numeric) entries.
// - Acts as a controller, dispatching user commands to the appropriate
//   mid-level UserIO modules (e.g., createSailing, promptToDeleteBooking).
//
// Used By: Called by main.cpp to run the application's primary event loop.
//          Most UI modules also utilize the trim() helper function
// ---------------------------------------------------------------------------

#include "UserInterface.h"
#include "VesselUserIO.h"
#include "VehicleFileIO.h"
#include "BookingUserIO.h"
#include "SailingUserIO.h"
#include <iostream>
#include <string>
#include <limits> 
#include <fstream>
using namespace std;


//----------------------------------------------------------------------------
void userInterfaceLoop(fstream& vesselFile, fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile){
//Description: Runs the main program menu and dispatches user input to check-in, booking, or sailing modules.
    bool running = true;
    while (running){
        cout << "=== Main Menu ===" << endl;
        cout << "[1] Check-in" << endl;
        cout << "[2] Bookings" << endl;
        cout << "[3] Sailings" << endl;
        cout << "[0] Quit" << endl;
        cout << "Enter a number (0-3): ";

        string inputLine;
        int choice = -1;
        getline(cin, inputLine);
        if (!inputLine.empty()){
            try{
                choice = stoi(inputLine);
            } catch (...){
                choice = -1;
            }
        } else{
            cout << "Bad Entry! Please try again." << endl;
            continue;
        }

        switch (choice){
            case 1:
                system("cls");
                checkIn(bookingFile, vehicleFile, sailingFile);
                break;
            case 2:
                system("cls");
                BookingsMenu(vehicleFile, bookingFile, sailingFile);
                break;
            case 3:
                system("cls");
                SailingsMenu(vesselFile, sailingFile, bookingFile, vehicleFile);
                break;
            case 0:
                running = false;
                cout << endl << "Shutting down FerryQ. Goodbye!" << endl;
                break;
            default:
                cout << "Bad Entry! Please try again." << endl;
                break;
        }

        cout << "\n";
    }
}

//----------------------------------------------------------------------------
void SailingsMenu(fstream& vesselFile, fstream& sailingFile, fstream& bookingFile, fstream& vehicleFile){
//Description: Displays the Sailings submenu and handles create, delete, report, and query actions.
    string inputLine;
    int choice = -1;
    bool inMenu = true;

    while (inMenu){
        cout << "==Sailings==" << endl;
        cout << "[1] Create a Sailing" << endl;
        cout << "[2] Delete a Sailing" << endl;
        cout << "[3] Create a Vessel" << endl;
        cout << "[4] View Sailings Report" << endl;
        cout << "[5] Query a Sailing" << endl;
        cout << "[0] Back" << endl;
        cout << "Enter a number (0-5): ";
        getline(cin, inputLine);
        if (inputLine.empty()) {
            system("cls"); 
            return;
        }
        try{
            choice = stoi(inputLine);
        } catch (...){
            cout << "Bad Entry! Please enter a valid number." << endl;
            choice = -1;
            continue;
        }

        switch (choice){
            case 1:
                createSailing(vesselFile, sailingFile);
                break;
            case 2:
                deleteSailing(sailingFile, bookingFile);
                break;
            case 3:
                createVessel(vesselFile);
                break;
            case 4:
                printReport(sailingFile, bookingFile, vehicleFile, vesselFile);
                break;
            case 5:
                querySailing(sailingFile);
                break;
            case 0:
                inMenu = false;
                system("cls");
                break;
            default:
                cout << "Bad Entry! Please try again." << endl;
                break;
        }

        cout << "\n";
    }
}

//----------------------------------------------------------------------------
void BookingsMenu(fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile){
//Description: Displays the Bookings submenu and handles creating or deleting bookings.
    string inputLine;
    int choice = -1;
    bool inMenu = true;

    while (inMenu){
        cout << "==Bookings==" << endl;
        cout << "[1] Create a booking" << endl;
        cout << "[2] Delete a booking" << endl;
        cout << "[0] Back" << endl;
        cout << "Enter a number (0-2): ";
        getline(cin, inputLine);
        if (inputLine.empty()) {
            system("cls");
            return;
        }
        try{
            choice = stoi(inputLine);
        } catch (...){
            cout << "Bad Entry! Please enter a valid number." << endl;
            choice = -1;
            continue;
        }

        switch (choice){
            case 1:
                createBooking(vehicleFile, bookingFile, sailingFile);
                break;
            case 2:
                promptToDeleteBooking(bookingFile, vehicleFile, sailingFile);
                break;
            case 0:
                inMenu = false;
                system("cls");
                break;
            default:
                cout << "Bad Entry! Please try again." << endl;
                break;
        }

        cout << "\n";
    }
}

string trim(const string& s){
    //Description: takes a string and returns a substring with trimmed
    //off whitespace characters
    size_t start = s.find_first_not_of(" \t\n\r");
    if(start == string::npos) return ""; //The string only has whitespace characters, return an empty string
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}
