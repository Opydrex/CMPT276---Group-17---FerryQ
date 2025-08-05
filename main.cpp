// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: main.cpp
// Rev.3 - 05/08/2025 - FerryQ now clears the terminal before launching
// Rev.2 - 24/07/2025 - main now opens all the files and calls the main UI loop.
// Rev.1 - 09/07/2025 - Main module created
//
// --------------------------------------------------------------------------
// This module serves as the entry point for the FerryQ application.
//
// What it does:
// - Initializes all fstream objects for binary file I/O.
// - Creates the data files (.txt) if they do not already exist.
// - Launches the main user interface loop, passing the open file streams.
// - Handles the final closing of all file streams upon program termination.
//
// Used By: This module is called by the operating system to start the program.
// --------------------------------------------------------------------------

#include "UserInterface.h"
#include "VesselUserIO.h"
#include "VehicleFileIO.h"
#include "BookingUserIO.h"
#include "SailingUserIO.h"
#include <iostream>
#include <fstream>
using namespace std;


//--------------------------------------------------------------------------
int main(){
//Job: Entry point of the FerryQ system. Initializes all binary file streams,
//     creates them if missing, then launches the main user interface loop.
//Usage: Called when the FerryQ program is executed. Ensures all required
//       system data files exist and are opened correctly.
//Restrictions: Files must be accessible for read/write in binary mode.
    system("cls");
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

/*                      CODING CONVENTIONS:

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
