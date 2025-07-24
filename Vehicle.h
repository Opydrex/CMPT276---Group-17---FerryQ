//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
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

const float maxLength = 99.9;  //maximum allowed vehicle length (meters)
const float maxHeight = 9.9;   //maximum allowed vehicle height (meters)
const string fileNameVehicle = "vehicle.txt";

class Vehicle{
    public:
        Vehicle() = default;
        Vehicle(const string& licensePlate, //input
                const float& height,       //input
                const float& length        //input
        );
        //Job: Vehicle constructor initializes a vehicle with given attributes.
        //Usage: Automatically called when creating a Vehicle record.
        //Restrictions: licensePlate 3-10 characters; height/length within allowed domain.



        //Setters
        void setLicensePlate(const string& licensePlate//input
                             );
        //Job: Sets the license plate of this vehicle.
        //Usage: Used when modifying or initializing a vehicle’s ID.
        //Restrictions: License plate must follow proper format and length.

        void setHeight(float height//input
                       );
        //Job: Sets the height of the vehicle.
        //Usage: Used during vehicle creation or editing.
        //Restrictions: Height must be non-negative and within vessel limits.

        void setLength(float length//input
                        );
        //Job: Sets the length of the vehicle.
        //Usage: Used during vehicle creation or editing.
        //Restrictions: Length must be non-negative and within vessel limits.

        //Getters
        string getLicensePlate() const;
        //Job: Retrieves the vehicle's license plate.
        //Usage: Used in display, search, or when writing to file.
        //Restrictions: None.

        float getHeight() const;
        //Job: Retrieves the height of the vehicle.
        //Usage: Used for file I/O, calculations, or reports.
        //Restrictions: None.

        float getLength() const;
        //Job: Retrieves the length of the vehicle.
        //Usage: Used for file I/O, calculations, or reports.
        //Restrictions: None.
    private:
        char licensePlate[11]; //License plate (max 10 chars + null terminator)
        float height;//Height of vehicle
        float length;//Length of vehicle
};

bool writeVehicle(fstream& vehicleFile, //input
                  const Vehicle& vehicle//input
                  );
//Job: Writes this Vehicle to the open vehicle file (binary).
//Usage: Called when adding a new vehicle to file.
//Restrictions: Only call if vehicle does not already exist in file.

bool isVehicleExist(fstream& vehicleFile,     //input
                    const string& licensePlate//input
                    );
//Job: Checks if the vehicle exists in the text file.
//Usage: Called when adding a new vehicle to file.
//Restrictions: vehicle file must be open. license plate has to be in range

bool getVehicleDimensions(fstream& vehicleFile,       //input
                          const string& licensePlate, //input
                          float& length, float& height//input
                          );
//Job: Getter for vehicle dimensions.
//Usage: Called while creating a Sailings report. Or when fare is calculated
//Restrictions: vehicle file must be open. license plate has to be in range, vehicle must exist

#endif //VEHICLE_H
