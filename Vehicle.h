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
const string fileNameVehicle = "vehicle.txt"; //constant that stores file name with all vehicles.

class Vehicle{
    public:
        Vehicle(const string& licensePlate,//input
                const float& height,//input
                const float& length //input
                );
        //Job: Vehicle object constructor.
        //Usage: Used when a Vehicle is being created. Created automatically if the vehicle is not found in the file.
        //Restrictions: Must comply to the domain restrictions and height/length restrictions.

        //----------------------------------------------------------------------------

        void writeVehicle(ofstream& outFile//input
                                );
        //Job: Writes an instance of the Vehicle object to the file.
        //Usage: Used automatically when a Vehicle is registered for the Booking for the first time ever.
        //Restrictions: Isn't called when Vehicle already exists in the file.

        //----------------------------------------------------------------------------


    private:
            string licensePlate; //License plate of the vehicle.
            float height; //Vehicle's height. 0 if it is not a special-sized vehicle.
            float length; //Vehicle's length. 0 if it is not a special-sized vehicle.



};

//----------------------------------------------------------------------------

bool isVehicleExist(const string& licensePlate//input
                           );

//Job: Returns a boolean flag indicating if the Vehicle exists.
//Usage: Call when need to find out if the Vehicle exists
//Restrictions: licensePlate must comply to domain.

//----------------------------------------------------------------------------



#endif //VEHICLE_H
