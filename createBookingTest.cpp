// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// 
// MODULE NAME: createBookingTest.cpp
// Rev.1 - 09/07/2025 - Implemented a test driver for vehicle file IO
//
// ----------------------------------------------------------------------------
// This module contains a test driver for creating a vehicle
// ----------------------------------------------------------------------------

#include "Booking.h"
#include <iostream>
#include <fstream>
using namespace std;

//----------------------------------------------------------------------------
int main() {
//Description: This is a test driver! not the actual main function of the program

    // Open a binary file to test writeVehicle
    fstream vehicleFile("vehicle.txt", ios::in |ios::out | ios::binary);
    fstream bookingFile("booking.txt", ios::in |ios::out | ios::binary);
    fstream sailingFile("sailing.txt", ios::in |ios::out | ios::binary);
    createBooking(vehicleFile, bookingFile, sailingFile);
    vehicleFile.close();
    bookingFile.close();
    sailingFile.close();

    // Confirm that file was created
    ifstream check("booking.txt", ios::binary);
    if (check.good()) {
        cout << "Vehicle written to vehicle.txt successfully." << endl;
    } else {
        cerr << "Failed to write vehicle to file." << endl;
    }
    check.close();

    return 0;
}