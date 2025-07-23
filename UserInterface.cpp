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
void SailingsMenu(ifstream& vesselInFile, ofstream& vesselOutFile, ifstream& sailingInFile, ofstream& sailingOutFile);
void BookingsMenu(ifstream& vehicleInFile, ofstream& vehicleOutFile, ifstream& bookingInFile, ofstream& bookingOutFile, ifstream& sailingInFile);


//----------------------------------------------------------------------------
// userInterfaceLoop
//----------------------------------------------------------------------------
void userInterfaceLoop() {
    // Open all necessary files at the start of the session.
    // These streams will be passed to the functions that need them.
    ofstream vesselOutFile(fileNameVessel, ios::app);
    ifstream vesselInFile(fileNameVessel);
    ofstream vehicleOutFile(fileNameVehicle, ios::app);
    ifstream vehicleInFile(fileNameVehicle);
    ofstream bookingOutFile(fileNameBooking, ios::app);
    ifstream bookingInFile(fileNameBooking);
    ofstream sailingOutFile(fileNameSailing, ios::app);
    ifstream sailingInFile(fileNameSailing);

    // Check if all files opened successfully
    if (!vesselOutFile || !vesselInFile || !vehicleOutFile || !vehicleInFile || !bookingOutFile || !bookingInFile || !sailingOutFile || !sailingInFile) {
        cerr << "Error: Could not open one or more data files. Please ensure they exist and have correct permissions." << endl;
        return;
    }

    int choice;
    bool running = true;

    while (running) {
        cout << "=== Main Menu ===" << endl;
        cout << "[1] Check-in" << endl;
        cout << "[2] Bookings" << endl;
        cout << "[3] Sailings" << endl;
        cout << "[0] Quit" << endl;
        cout << "Enter a number (0-3): ";

        cin >> choice;
        cin.ignore();

        // Input validation
        if (cin.fail()) {
            cout << "Bad Entry! Please enter a valid number." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                // Call checkIn with the required file streams
                checkIn(bookingInFile, vehicleInFile, bookingOutFile, sailingInFile);
                break;
            case 2:
                // Go to the Bookings Menu
                BookingsMenu(vehicleInFile, vehicleOutFile, bookingInFile, bookingOutFile, sailingInFile);
                break;
            case 3:
                // Go to the Sailings Menu
                SailingsMenu(vesselInFile, vesselOutFile, sailingInFile, sailingOutFile);
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

    // Close all file streams before exiting
    vesselOutFile.close();
    vesselInFile.close();
    vehicleOutFile.close();
    vehicleInFile.close();
    bookingOutFile.close();
    bookingInFile.close();
    sailingOutFile.close();
    sailingInFile.close();
}

//----------------------------------------------------------------------------
// Sailings Menu
//----------------------------------------------------------------------------
void SailingsMenu(ifstream& vesselInFile, ofstream& vesselOutFile, ifstream& sailingInFile, ofstream& sailingOutFile) {
    int choice;
    bool inMenu = true;

    while (inMenu) {
        cout << "==Sailings==" << endl;
        cout << "[1] Create a Sailing" << endl;
        cout << "[2] Delete a Sailing" << endl;
        cout << "[3] Create a Vessel" << endl;
        cout << "[4] View Sailings Report" << endl;
        cout << "[5] Query a Sailing" << endl; // New option
        cout << "[0] Exit" << endl;
        cout << "Enter a number (0-5): ";

        cin >> choice;
        cin.ignore();

        if (cin.fail()) {
            cout << "Bad Entry! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                createSailing(vesselInFile, sailingOutFile, sailingInFile);
                break;
            case 2:
                promptToDeleteSailing(sailingInFile, sailingOutFile);
                break;
            case 3:
                createVessel(vesselInFile, vesselOutFile);
                break;
            case 4:
                printReport(sailingInFile);
                break;
            case 5: // New case for Query Sailing
                querySailing(sailingInFile);
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
void BookingsMenu(ifstream& vehicleInFile, ofstream& vehicleOutFile, ifstream& bookingInFile, ofstream& bookingOutFile, ifstream& sailingInFile) {
    int choice;
    bool inMenu = true;

    while (inMenu) {
        cout << "==Bookings==" << endl;
        cout << "[1] Create a booking" << endl;
        cout << "[2] Delete a booking" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter a number (0-2): ";

        cin >> choice;

        if (cin.fail()) {
            cout << "Bad Entry! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                // Call createBooking with its required file streams
                createBooking(vehicleInFile, vehicleOutFile, bookingOutFile, sailingInFile);
                break;
            case 2:
                // Call the self-contained prompt function for deleting a booking
                promptToDeleteBooking(bookingInFile, bookingOutFile);
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