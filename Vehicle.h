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
const float maxLength = 99.9; //constant that stores maximum allowed vehicle length.
const float maxHeight = 9.9; //constant that stores maximum allowed vehicle height.
const string fileNameVehicle = "vehicle.txt";
class Vehicle {
    public:
        Vehicle() = default;

        Vehicle(const string& licensePlate, // input
                const float& height,       // input
                const float& length        // input
        );
        // Job: Vehicle object constructor.
        // Usage: Used when a Vehicle is being created. Created automatically if the vehicle is not found in the file.
        // Restrictions: Must comply to the domain restrictions and height/length restrictions.

        void writeVehicle(ofstream& outFile); // input
        // Job: Writes an instance of the Vehicle object to the file.
        // Usage: Used automatically when a Vehicle is registered for the Booking for the first time ever.
        // Restrictions: Isn't called when Vehicle already exists in the file.

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
        float height; // Vehicle's height
        float length; // Vehicle's length
};

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

// bool isVehicleExist(const string& licensePlate, // input
//                     ifstream& inFile            // input
// );
// // Job: Returns a boolean flag indicating if the Vehicle exists.
// // Usage: Call when need to find out if the Vehicle exists
// // Restrictions: licensePlate must comply to domain.

// //----------------------------------------------------------------------------

// void getVehicleDimensions(string licensePlate, // input
//                           string* length,      // output
//                           string* height,      // output
//                           ifstream& inFile     // input
// );
// // Job: Assigns vehicle length and height to two pointers.
// // Usage: acts as a getter for a vehicle's height and length.
// // Restrictions: licensePlate must comply to domain. Vehicle must exist.

#endif // VEHICLE_H
