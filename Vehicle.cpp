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

Vehicle::Vehicle(const string& licensePlate, const float& height, const float& length) {
    // Store licensePlate in fixed-size char array (ensure null termination)
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';
    this->height = height;
    this->length = length;
}

void Vehicle::writeVehicle(fstream& outFile) {
    if (outFile.is_open()) {
        outFile.clear();
        outFile.seekp(0, ios::end);
        outFile.write(reinterpret_cast<const char*>(this), sizeof(Vehicle));
        outFile.flush();
    } else {
        cerr << "Error: Unable to open vehicle file for writing." << endl;
    }
}

// Setters
void Vehicle::setLicensePlate(const string& licensePlate) {
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[10] = '\0'; // ensure null-termination
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