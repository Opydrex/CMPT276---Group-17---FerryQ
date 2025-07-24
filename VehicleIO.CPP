//==========================================================================
// VehicleIO.cpp - Implementation of Vehicle file I/O operations
//==========================================================================
#include "VehicleIO.h"
#include <fstream>
#include <iostream>
using namespace std;

bool appendVehicle(const Vehicle& vehicle) {
    ofstream out(fileNameVehicle, ios::binary | ios::app);
    if (!out) return false;

    out.write(reinterpret_cast<const char*>(&vehicle), sizeof(Vehicle));
    return true;
}

bool isVehicleExist(const string& licensePlate) {
    ifstream in(fileNameVehicle, ios::binary);
    if (!in) return false;

    Vehicle temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))) {
        if (temp.getLicensePlate() == licensePlate) {
            return true;
        }
    }

    return false;
}

bool getVehicleDimensions(const string& licensePlate, float& length, float& height) {
    ifstream in(fileNameVehicle, ios::binary);
    if (!in) return false;

    Vehicle temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))) {
        if (temp.getLicensePlate() == licensePlate) {
            length = temp.getLength();
            height = temp.getHeight();
            return true;
        }
    }

    return false;
}