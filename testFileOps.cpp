//==========================================================================
//==========================================================================
/*
MODULE NAME: testFileOps.cpp
Rev.1 - 17/07/2025 - test file operations module created
----------------------------------------------------------------------------
This module contains the operations and functions that will test the program
----------------------------------------------------------------------------
*/

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Vessel.h"
#include "Vehicle.h"
using namespace std;

int main(){
    // Declare the stream objects
    // Step 1: open for reading to check duplicates
    ifstream myInputVesselFile("vessel.txt");
    if (!myInputVesselFile) {
        cerr << "Could not open vessel.txt for reading" << endl;
        return 1;
    }



    ofstream myOutputVesselFile("vessel.txt", ios::app);
    if (!myOutputVesselFile) {
        cerr << "Could not open vessel.txt for appending" << endl;
        return 1;
    }

// Now call your logic
    createVessel(myInputVesselFile, myOutputVesselFile);



    //testing vehicle
    ifstream myInputVehicleFile("vehicle.txt");
    if (!myInputVehicleFile) {
        cerr << "Could not open vehicle.txt for reading" << endl;
        return 1;
    }



    ofstream myOutputVehicleFile("vehicle.txt", ios::app);
    if (!myOutputVesselFile) {
        cerr << "Could not open vessel.txt for appending" << endl;
        return 1;
    }
    Vehicle newVehicle("cmpt276", 2, 6);

    newVehicle.writeVehicle(myOutputVehicleFile);
    cout<<isVesselExist("cmpt276",myInputVehicleFile);


// Clean up
    myInputVesselFile.close();
    myOutputVesselFile.close();

}