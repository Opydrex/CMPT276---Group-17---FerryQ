// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: testFileOps.cpp
// Rev.1 - 09/07/2025 - Implemented a test driver for vehicle file IO
//
// ----------------------------------------------------------------------------
// This module contains a test driver for vehicle file IO
// ----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "VehicleFileIO.h"

using namespace std;

//----------------------------------------------------------------------------
int main() {
//Description: This is a test driver! not the actual main function of the program
    // Open (and truncate) the vehicle file
    fstream file(fileNameVehicle, ios::binary | ios::in | ios::out | ios::trunc);
    if (!file) {
        cerr << "Error: Unable to open " << fileNameVehicle << endl;
        return 1;
    }

    // Create two vehicles
    Vehicle v1("ABC123", 1.5, 4.2);
    Vehicle v2("XYZ789", 2.8, 6.6);

    bool pass = true;

    // Write them to file
    if (!writeVehicle(file, v1)) {
        cerr << "Error: writeVehicle(v1) failed" << endl;
        pass = false;
    }
    if (!writeVehicle(file, v2)) {
        cerr << "Error: writeVehicle(v2) failed" << endl;
        pass = false;
    }

    // Test existence checks

    if (!isVehicleExist(file, "ABC123")) {
        cerr << "Error: A vehicle with the license plate ABC123 does not exist" << endl;
        pass = false;
    }
    if (!isVehicleExist(file, "XYZ789")) {
        cerr << "Error: A vehicle with the license plate XYZ789 does not exist" << endl;
        pass = false;
    }


    // Test dimension reads
    float length, height;

    if (!getVehicleDimensions(file, "ABC123", length, height) ||
        length != v1.getLength() || height != v1.getHeight()) {
        cerr << "Error: getVehicleDimensions failed for license plate ABC123" << endl;
        pass = false;
    } else {
        cout << "Read height  = " << height << " and length = " << length << " of the vehicle with the license plate ABC123" << endl;
    }

    if (!getVehicleDimensions(file, "XYZ789", length, height) ||
        length != v2.getLength() || height != v2.getHeight()) {
        cerr << "Error: getVehicleDimensions failed for license plate XYZ789" << endl;
        pass = false;
    }else {
        cout << "Read height  = " << height << " and length = " << length << " of the vehicle with the license plate XYZ789" << endl;
    }

    // Test that reading beyond end fails
    if (getVehicleDimensions(file, "MYSTERY", length, height)) {
        cerr << "Error: getVehicleDimensions found a license plate that should not exist!" << endl;
        pass = false;
    } else {
        cout << "getVehicleDimensions stopped successfully at EOF" << endl;
    }

    // Final result
    if(pass){
        cout << "Test passed!" << endl;
        return 0;
    } else {
        cout << "Test Failed!" << endl;
        return 1;
    }

}