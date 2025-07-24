//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================
//MODULE NAME: UserInterface.h
//Rev.1 - 09/07/2025 - UserInterface class header created.
//--------------------------------------------------------------------------
//This module contains functions and declarations related to the
//User Interface. It declares the main program loop entry point
//for interacting with all system modules.
//==========================================================================

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------
//userInterfaceLoop
//----------------------------------------------------------------------------
//Job: Entry point for executing the FerryQ main menu loop. Provides high-
//     level navigation between the Bookings, Sailings, and Check-in modules.
//Usage: Called from main() with all open file streams. This function loops
//       until the user chooses to exit the system.
//Restriction: Must be called from main. File streams must be open.
void userInterfaceLoop(fstream& vesselFile,
                       fstream& vehicleFile,
                       fstream& bookingFile,
                       fstream& sailingFile);

#endif //USERINTERFACE_H
