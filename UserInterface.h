//==========================================================================
//==========================================================================
/*
MODULE NAME: UserInterface.h
Rev.1 - 09/07/2025 - UserInterface class header created.
----------------------------------------------------------------------------
This module contains functions and implementations related to User Interface.
----------------------------------------------------------------------------
*/

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------

void userInterfaceLoop(fstream& vesselFile, fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile);
//Job: Handles the execution loop of the program, this function has the switch cases for the UI to function
//Usage: Called from main when the program starts up, and continues looping until the program is terminated
//Restriction: Must be called from main()

//----------------------------------------------------------------------------

#endif //USERINTERFACE_H