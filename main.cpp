//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//MODULE NAME: main.cpp
//Rev.1 - 09/07/2025 - Main module created
//--------------------------------------------------------------------------
//This module contains the entry point for the FerryQ system. It handles
//initialization and final shutdown of all file streams and launches the
//main user interface loop that drives the application.
//==========================================================================

#include "UserInterface.h"
#include "Vessel.h"
#include "Vehicle.h"
#include "Booking.h"
#include "Sailing.h"
#include <iostream>
#include <fstream>
using namespace std;

//----------------------------------------------------------------------------
//main
//----------------------------------------------------------------------------
//Job: Entry point of the FerryQ system. Initializes all binary file streams,
//     creates them if missing, then launches the main user interface loop.
//Usage: Called when the FerryQ program is executed. Ensures all required
//       system data files exist and are opened correctly.
//Restrictions: Files must be accessible for read/write in binary mode.
int main(){
    cout << "Welcome to the FerryQ!!!" << endl << endl;

    //Open all system files or create if missing
    fstream vesselFile(fileNameVessel, ios::in | ios::out | ios::binary);
    if (!vesselFile){
        ofstream tmp(fileNameVessel, ios::binary); tmp.close();
        vesselFile.open(fileNameVessel, ios::in | ios::out | ios::binary);
    }

    fstream vehicleFile(fileNameVehicle, ios::in | ios::out | ios::binary);
    if (!vehicleFile){
        ofstream tmp(fileNameVehicle, ios::binary); tmp.close();
        vehicleFile.open(fileNameVehicle, ios::in | ios::out | ios::binary);
    }

    fstream bookingFile(fileNameBooking, ios::in | ios::out | ios::binary);
    if (!bookingFile){
        ofstream tmp(fileNameBooking, ios::binary); tmp.close();
        bookingFile.open(fileNameBooking, ios::in | ios::out | ios::binary);
    }

    fstream sailingFile(fileNameSailing, ios::in | ios::out | ios::binary);
    if (!sailingFile){
        ofstream tmp(fileNameSailing, ios::binary); tmp.close();
        sailingFile.open(fileNameSailing, ios::in | ios::out | ios::binary);
    }

    if (!vesselFile || !vehicleFile || !bookingFile || !sailingFile){
        cerr << "Error: Could not open one or more data files." << endl;
        return 1;
    }

    //Launch main interface
    userInterfaceLoop(vesselFile, vehicleFile, bookingFile, sailingFile);

    //Final cleanup
    vesselFile.close();
    vehicleFile.close();
    bookingFile.close();
    sailingFile.close();

    return 0;
}
