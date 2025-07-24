//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//MODULE NAME: UserInterface.cpp
//Rev.1 - 21/07/2025 - UserInterface class implementation.
//Rev.2 - 22/07/2025 - Updated to match new module signatures and added Query Sailing.
//--------------------------------------------------------------------------
//This module contains the implementation for the user interface, including
//the main program loop and menu navigation, as specified in the architectural
//design and user manual. It dispatches user commands to the appropriate
//modules and coordinates file stream usage.
//==========================================================================

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
                checkIn(bookingFile, vehicleFile, sailingFile);
                break;
            case 2:
                BookingsMenu(vehicleFile, bookingFile, sailingFile);
                break;
            case 3:
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
void SailingsMenu(fstream& vesselFile, fstream& sailingFile){
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
        cout << "[0] Exit" << endl;
        cout << "Enter a number (0-5): ";
        getline(cin, inputLine);
        if (inputLine.empty()) return;

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
                deleteSailing(sailingFile);
                break;
            case 3:
                createVessel(vesselFile);
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
void BookingsMenu(fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile){
//Description: Displays the Bookings submenu and handles creating or deleting bookings.
    string inputLine;
    int choice = -1;
    bool inMenu = true;

    while (inMenu){
        cout << "==Bookings==" << endl;
        cout << "[1] Create a booking" << endl;
        cout << "[2] Delete a booking" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter a number (0-2): ";
        getline(cin, inputLine);
        if (inputLine.empty()) return;

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
                promptToDeleteBooking(bookingFile);
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
