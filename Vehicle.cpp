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

//Constructor
Vehicle::Vehicle(const string& licensePlate, const float& height, const float& length) {
    // Store licensePlate in fixed-size char array (ensure null termination)
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';
    this->height = height;
    this->length = length;
}

//File I/O Functions
bool writeVehicle(fstream& vehicleFile, const Vehicle& vehicle) {
    vehicleFile.clear();
    vehicleFile.seekp(0, ios::end);
    if (!vehicleFile) {
        cerr << "Error: Vehicle file stream not available for writing.\n";
        return false;
    }
    vehicleFile.write(reinterpret_cast<const char*>(&vehicle), sizeof(Vehicle));
    vehicleFile.flush();
    return true;
}

bool isVehicleExist(fstream& vehicleFile, const string& licensePlate) {
    vehicleFile.clear(); // reset EOF state
    vehicleFile.seekg(0, ios::beg);
    Vehicle temp;
    while (vehicleFile.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))) {
        if (temp.getLicensePlate() == licensePlate) {
            return true;
        }
    }
    return false;
}

bool getVehicleDimensions(fstream& vehicleFile, const string& licensePlate, float& length, float& height) {
    vehicleFile.clear();
    vehicleFile.seekg(0, ios::beg);
    Vehicle temp;
    while (vehicleFile.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))) {
        if (temp.getLicensePlate() == licensePlate) {
            length = temp.getLength();
            height = temp.getHeight();
            return true;
        }
    }
    return false;
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