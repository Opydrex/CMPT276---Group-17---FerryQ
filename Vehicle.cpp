//==========================================================================
//==========================================================================
/*
MODULE NAME: Vehicle.cpp
Rev.1 - 24/07/2025 - Vehicle class implementation.
----------------------------------------------------------------------------
This module contains Vehicle class implementation.
----------------------------------------------------------------------------
*/
#include "Vehicle.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

// Vehicle::Vehicle(const string& licensePlate,//input
//                const float& height,//input
//                const float& length//input
// )
// {
//     this->licensePlate = licensePlate;
//     this->height = height;
//     this->length = length;
// }

void Vehicle::writeVehicle(ofstream& outFile) {
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(this), sizeof(Vehicle));
        outFile.flush();
    } else {
        cerr << "Error: Unable to open file for writing. Check file path and permissions." << endl;
    }
}

bool isVehicleExist(const string& licensePlate, ifstream& inFile) {
    inFile.clear();
    inFile.seekg(0, ios::beg);

    Vehicle temp;
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))) {
        if (licensePlate == temp.getLicensePlate()) {
            return true;
        }
    }

    return false;
}


void getVehicleDimensions(string licensePlate, string* length, string* height, ifstream& inFile) {
    inFile.clear();
    inFile.seekg(0, ios::beg);

    Vehicle temp;
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))) {
        if (licensePlate == temp.getLicensePlate()) {
            *height = to_string(temp.getHeight());
            *length = to_string(temp.getLength());
            return;
        }
    }

    cout << "Vehicle not found." << endl;
}

// Setters
void Vehicle::setLicensePlate(const string& licensePlate) {
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[10] = '\0'; // Ensure null-termination
}

void Vehicle::setHeight(float height) {
    this->height = height;
}

void Vehicle::setLength(float length) {
    this->length = length;
}

// Getters
string Vehicle::getLicensePlate() const {
    return string(this->licensePlate);
}

float Vehicle::getHeight() const {
    return this->height;
}

float Vehicle::getLength() const {
    return this->length;
}