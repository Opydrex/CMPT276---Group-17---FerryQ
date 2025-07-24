#include "Booking.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {

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