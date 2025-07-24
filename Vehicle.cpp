//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: VehicleFileIO.cpp
Rev.1 - 24/07/2025 - Vehicle class implementation.

----------------------------------------------------------------------------
This module contains the implementation of the Vehicle class and basic
functions for reading/writing vehicle records to a binary file.

Design notes:
- Vehicles are stored as fixed-length binary records.
- All I/O is done one record at a time.
- Linear search is used for lookup.
- License plate is stored in a fixed-size char array to ensure consistent record size.
*/

//----------------------------------------------------------------------------
#include "Vehicle.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

//----------------------------------------------------------------------------
//Description: This is a constructor that initializes a Vehicle object with license, height, and length.
Vehicle::Vehicle(const string& licensePlate, const float& height, const float& length){
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0'; //Null-terminate
    this->height = height;
    this->length = length;
}

//----------------------------------------------------------------------------
//Description: Appends a vehicle record to the end of the vehicle file.
//             Returns true if successful.
bool writeVehicle(fstream& vehicleFile, const Vehicle& vehicle){
    vehicleFile.clear();                       //Clear EOF or fail flags
    vehicleFile.seekp(0, ios::end);            //Move to end to append

    if (!vehicleFile){
        cerr << "Error: Vehicle file stream not available for writing.\n";
        return false;
    }

    vehicleFile.write(reinterpret_cast<const char*>(&vehicle), sizeof(Vehicle));
    vehicleFile.flush();                       //Ensure it's written to disk
    return true;
}

//----------------------------------------------------------------------------
//Description: Checks if a vehicle with the given license plate exists in the file.
//             Returns true if found.
bool isVehicleExist(fstream& vehicleFile, const string& licensePlate){
    vehicleFile.clear();
    vehicleFile.seekg(0, ios::beg);
    Vehicle temp;

    //Linear search through all records
    while (vehicleFile.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))){
        if (temp.getLicensePlate() == licensePlate){
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------
bool getVehicleDimensions(fstream& vehicleFile, const string& licensePlate, float& length, float& height){
    //Description: Retrieves the dimensions of a vehicle by license plate.
    //             Stores the length and height in output parameters and returns true if found.
    vehicleFile.clear();
    vehicleFile.seekg(0, ios::beg);
    Vehicle temp;

    //Search each record for a match
    while (vehicleFile.read(reinterpret_cast<char*>(&temp), sizeof(Vehicle))){
        if (temp.getLicensePlate() == licensePlate){
            length = temp.getLength();
            height = temp.getHeight();
            return true;
        }
    }
    return false;
}


//----------------------------------------------------------------------------
void Vehicle::setLicensePlate(const string& licensePlate){
    //Description: Sets the license plate string (fixed-size char array).
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[10] = '\0'; //Null-terminate
}
//----------------------------------------------------------------------------
void Vehicle::setHeight(float height){
    //Description: Sets the height of the vehicle.
    this->height = height;
}
//----------------------------------------------------------------------------
void Vehicle::setLength(float length){
    //Description: Sets the length of the vehicle.
    this->length = length;
}

//----------------------------------------------------------------------------
string Vehicle::getLicensePlate() const{
    //Description: Returns the license plate as a std::string.
    return string(this->licensePlate);
}

//----------------------------------------------------------------------------
float Vehicle::getHeight() const{
    //Description: Returns the height of the vehicle.
    return this->height;
}

//----------------------------------------------------------------------------
float Vehicle::getLength() const{
    //Description: Returns the length of the vehicle.
    return this->length;
}

//----------------------------------------------------------------------------
