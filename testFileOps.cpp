#include "Vehicle.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    // Create a vehicle object
    Vehicle v("CMPT276", 2.5f, 4.8f);

    // Print out the fields to check constructor + getters
    cout << "License Plate: " << v.getLicensePlate() << endl;
    cout << "Height: " << v.getHeight() << endl;
    cout << "Length: " << v.getLength() << endl;

    // Open a binary file to test writeVehicle
    fstream outFile("vehicle.txt", ios::out | ios::binary);
    writeVehicle(outFile, v);
    outFile.close();

    // Confirm that file was created
    ifstream check("vehicle.txt", ios::binary);
    if (check.good()) {
        cout << "Vehicle written to vehicle.txt successfully." << endl;
    } else {
        cerr << "Failed to write vehicle to file." << endl;
    }
    check.close();

    return 0;
}