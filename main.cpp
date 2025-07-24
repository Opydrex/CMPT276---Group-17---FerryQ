//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
/*
MODULE NAME: main.cpp
Rev.1 - 09/07/2025 - Main module created
----------------------------------------------------------------------------
This module contains the main function, as well as functions and implementations related to startup and shutdown.
----------------------------------------------------------------------------
*/

#include "UserInterface.h"
#include <iostream>
#include "Vessel.h"
#include "Vehicle.h"
#include "Booking.h"
#include "Sailing.h"

int main() {
    std::cout << "Welcome to the FerryQ!!!" << std::endl << std::endl;
    // Open all data files once at program startup (for combined read/write in binary mode)
    fstream vesselFile(fileNameVessel, ios::in | ios::out | ios::binary);
    if (!vesselFile) {
        // Create file if it doesn't exist, then reopen in read/write mode
        ofstream tmp(fileNameVessel, ios::binary);
        tmp.close();
        vesselFile.open(fileNameVessel, ios::in | ios::out | ios::binary);
    }
    fstream vehicleFile(fileNameVehicle, ios::in | ios::out | ios::binary);
    if (!vehicleFile) {
        ofstream tmp(fileNameVehicle, ios::binary);
        tmp.close();
        vehicleFile.open(fileNameVehicle, ios::in | ios::out | ios::binary);
    }
    fstream bookingFile(fileNameBooking, ios::in | ios::out | ios::binary);
    if (!bookingFile) {
        ofstream tmp(fileNameBooking, ios::binary);
        tmp.close();
        bookingFile.open(fileNameBooking, ios::in | ios::out | ios::binary);
    }
    fstream sailingFile(fileNameSailing, ios::in | ios::out | ios::binary);
    if (!sailingFile) {
        ofstream tmp(fileNameSailing, ios::binary);
        tmp.close();
        sailingFile.open(fileNameSailing, ios::in | ios::out | ios::binary);
    }
    if (!vesselFile || !vehicleFile || !bookingFile || !sailingFile) {
        std::cerr << "Error: Could not open one or more data files." << std::endl;
        return 1;
    }
    // Launch the user interface loop, passing in all opened file streams
    userInterfaceLoop(vesselFile, vehicleFile, bookingFile, sailingFile);
    // Close all data files before exiting the program
    vesselFile.close();
    vehicleFile.close();
    bookingFile.close();
    sailingFile.close();
    return 0;
}

//Job: Initialises and opens every data file upon program launch.
//Usage: Used in main.cpp
//Restrictions: Data files must exist. They come prefilled with the executable of FerryQ.

//----------------------------------------------------------------------------

void shutdown();

//Job: Shutdowns every data file and frees all the used memory when User exits the program.
//Usage: Used in main.cpp
//Restrictions: Data files must exist. They come prefilled with the executable of FerryQ.

//----------------------------------------------------------------------------

/*
Modules will be separated by two double-dashed lines, example:
//==========================================================================
//==========================================================================

The header of each module will have this format:
MODULE NAME:
REVISION HISTORY. (Revisions are going to be in descending order)
Rev.2 - DD/MM/YYYY - [Description]
Rev.1 - DD/MM/YYYY - [Description]

At the end of the header, there will be an explanation of what the module does
and where it is used.

Each section of the header file and each function will be separated
by a single dashed line. Example:
----------------------------------------------------------------------------
C++ #include statements go here (after the header).
----------------------------------------------------------------------------
Global constants/type/variables go here.
----------------------------------------------------------------------------
Class declarations go here.
Public members first, protected and private go below that.
----------------------------------------------------------------------------
EXAMPLE FUNCTION DECLARATION:
Booking(const string& licensePlate, //first parameter comment example
        const string& SailingId     //second parameter comment example
        );
----------------------------------------------------------------------------
Function Declaration Commenting procedure:
    As shown above, each parameter is listed on a separate line, with a comment
    describing whether it's an input, output, or input and output parameter. 

    The description of what the function does comes right after the function
    declaration. This description should have the following form:
    //Job: [Actual description of the function]
    //Usage: [How the function should be used]
    //Restrictions: [Describe the restrictions of the function]
----------------------------------------------------------------------------
Private Variables in classes will not be separated by a line, but private functions
will.
*/