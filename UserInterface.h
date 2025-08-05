// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: UserInterface.h
// Rev.2 - 05/08/2025 - Added the trim helper
// Rev.1 - 09/07/2025 - UserInterface class header created.
//
// --------------------------------------------------------------------------
// This module declares the public functions for the main user interface.
// The primary function, userInterfaceLoop, is the entry point for the
// application's interactive menu system and drives all user-facing workflows.
// ---------------------------------------------------------------------------

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------
void userInterfaceLoop(fstream& vesselFile, //input
                       fstream& vehicleFile,//input
                       fstream& bookingFile,//input
                       fstream& sailingFile //input
                       );
//Job: Runs the main menu loop for navigating the FerryQ system.
//Usage: Called from main() to route control to Bookings, Sailings, and Check-in.
//Restrictions: All file streams must be open and valid before calling.

//----------------------------------------------------------------------------
void SailingsMenu(fstream& vesselFile,    //input
                  fstream& sailingFile,   //input
                  fstream& bookingFile,   //input
                  fstream& vehicleFile    //input
                  );
//Job: Displays and handles the Sailings submenu (create/delete sailings, add vessels, view reports).
//Usage: Called from userInterfaceLoop when user selects Sailings.
//Restrictions: Requires valid, open file streams.

//----------------------------------------------------------------------------
void BookingsMenu(fstream& vehicleFile,//input
                  fstream& bookingFile,//input 
                  fstream& sailingFile //input
                  );
//Job: Displays and handles the Bookings submenu (create/delete bookings).
//Usage: Called from userInterfaceLoop when user selects Bookings.
//Restrictions: Requires valid, open file streams.

//----------------------------------------------------------------------------
string trim(const string& s //input
            );
//Job: Trims off whitespace characters from the input and returns it
//Usage: Called after any getline() to trim off whitespace for usage elsewhere
//Restrictions: Requires a string.

#endif //USERINTERFACE_H
