//==========================================================================
//==========================================================================
/*
MODULE NAME: UserInterface.cpp
Rev.2 - 22/07/2025 - Updated to match new module signatures and added Query Sailing.
Rev.1 - 21/07/2025 - UserInterface class implementation.
----------------------------------------------------------------------------
This module contains the implementation for the user interface, including the
main program loop and menu navigation, as specified in the architectural
design and user manual. It dispatches user commands to the appropriate
modules.
----------------------------------------------------------------------------
*/

#include "UserInterface.h"
#include "Booking.h"
#include "Sailing.h"
#include "Vessel.h"
#include "Vehicle.h"
#include <iostream>
#include <string>
#include <limits> 
#include <fstream>

using namespace std;

//----------------------------------------------------------------------------
// Function Prototypes for menus
//----------------------------------------------------------------------------
void SailingsMenu(fstream& vesselFile, fstream& sailingFile);
void BookingsMenu(fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile);


//----------------------------------------------------------------------------
// userInterfaceLoop
//----------------------------------------------------------------------------
void userInterfaceLoop(fstream& vesselFile, fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile) {
    bool running = true;
    while (running) {
        cout << "=== Main Menu ===" << endl;
        cout << "[1] Check-in" << endl;
        cout << "[2] Bookings" << endl;
        cout << "[3] Sailings" << endl;
        cout << "[0] Quit" << endl;
        cout << "Enter a number (0-3): ";
        string inputLine;
        int choice = -1;
        getline(cin, inputLine);
        if (!inputLine.empty()) {
            try {
                choice = stoi(inputLine);
            } catch (...) {
                choice = -1;
            }
        } else {
            cout << "Bad Entry! Please try again." << endl;
            continue;
        }
        switch (choice) {
            case 1:
                // Call checkIn with open booking, vehicle, and sailing file streams
                checkIn(bookingFile, vehicleFile, sailingFile);
                break;
            case 2:
                // Enter the Bookings submenu
                BookingsMenu(vehicleFile, bookingFile, sailingFile);
                break;
            case 3:
                // Enter the Sailings submenu
                SailingsMenu(vesselFile, sailingFile);
                break;
            case 0:
                running = false;
                cout << "Shutting down FerryQ. Goodbye!" << endl;
                break;
            default:
                cout << "Bad Entry! Please try again." << endl;
                break;
        }
        cout << "\n";
    }
}

//----------------------------------------------------------------------------
// Sailings Menu
//----------------------------------------------------------------------------
// Job: Handles Sailings-related commands (create/delete sailing, create vessel, 
// print report, query sailing) using open file streams.
void SailingsMenu(fstream& vesselFile, fstream& sailingFile) {
    string inputLine;
    int choice = -1;
    bool inMenu = true;
    while (inMenu) {
        cout << "==Sailings==" << endl;
        cout << "[1] Create a Sailing" << endl;
        cout << "[2] Delete a Sailing" << endl;
        cout << "[3] Create a Vessel" << endl;
        cout << "[4] View Sailings Report" << endl;
        cout << "[5] Query a Sailing" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter a number (0-5): ";
        getline(cin, inputLine);
        if (inputLine.empty()) return;
        // We ignore any leftover input with newline
        // (No explicit ignore needed if reading line-by-line)
        try {
            choice = stoi(inputLine);
        } catch (...) {
            cout << "Bad Entry! Please enter a valid number." << endl;
            choice = -1;
            continue;
        }
        switch (choice) {
            case 1:
                createSailing(vesselFile, sailingFile);
                break;
            case 2:
                deleteSailing(sailingFile);
                break;
            case 3:
                createVessel(vesselFile, vesselFile);
                break;
            case 4:
                printReport(sailingFile);
                break;
            case 5:
                querySailing(sailingFile);
                break;
            case 0:
                inMenu = false;
                break;
            default:
                cout << "Bad Entry! Please try again." << endl;
                break;
        }
        cout << "\n";
    }
}

//----------------------------------------------------------------------------
// Bookings Menu
//----------------------------------------------------------------------------
// Job: Handles Bookings-related commands (create/delete booking) using open file streams.
void BookingsMenu(fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile) {
    string inputLine;
    int choice = -1;
    bool inMenu = true;
    while (inMenu) {
        cout << "==Bookings==" << endl;
        cout << "[1] Create a booking" << endl;
        cout << "[2] Delete a booking" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter a number (0-2): ";
        getline(cin, inputLine);
        if (inputLine.empty()) return;
        try {
            choice = stoi(inputLine);
        } catch (...) {
            cout << "Bad Entry! Please enter a valid number." << endl;
            choice = -1;
            continue;
        }
        switch (choice) {
            case 1:
                createBooking(vehicleFile, bookingFile, sailingFile);
                break;
            case 2:
                promptToDeleteBooking();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                cout << "Bad Entry! Please try again." << endl;
                break;
        }
        cout << "\n";
    }
}