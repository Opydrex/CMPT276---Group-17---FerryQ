//==========================================================================
//==========================================================================
/*
MODULE NAME: Vehicle.h
Rev.1 - 09/07/2025 - Vehicle class header created
Rev.2 - 24/07/2025 - Minor changes
----------------------------------------------------------------------------
This module contains functions and implementations related to Vehicles.
----------------------------------------------------------------------------
*/

#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
using namespace std;

const float maxLength = 99.9;  // maximum allowed vehicle length (meters)
const float maxHeight = 9.9;   // maximum allowed vehicle height (meters)
const string fileNameVehicle = "vehicle.txt";

class Vehicle {
    public:
        Vehicle() = default;
        Vehicle(const string& licensePlate, // input
                const float& height,       // input
                const float& length        // input
        );
        // Job: Vehicle constructor initializes a vehicle with given attributes.
        // Usage: Automatically called when creating a Vehicle record.
        // Restrictions: licensePlate 3-10 characters; height/length within allowed domain.

        void writeVehicle(fstream& outFile);
        // Job: Writes this Vehicle to the open vehicle file (binary).
        // Usage: Called when adding a new vehicle to persistent storage.
        // Restrictions: Only call if vehicle does not already exist in file.

        // Setters
        void setLicensePlate(const string& licensePlate);
        void setHeight(float height);
        void setLength(float length);
        // Getters
        string getLicensePlate() const;
        float getHeight() const;
        float getLength() const;
    private:
        char licensePlate[11]; // License plate (max 10 chars + null terminator)
        float height;
        float length;
};

#endif // VEHICLE_H
