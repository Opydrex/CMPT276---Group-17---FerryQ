//==========================================================================
//==========================================================================
/*
MODULE NAME: Vehicle.h
Rev.1 - 09/07/2025 - Vehicle class header created
----------------------------------------------------------------------------
This module is used to store data about vehicle.
----------------------------------------------------------------------------
*/

#ifndef VEHICLE_H
#define VEHICLE_H


#include <iostream>
#include <string>
using namespace std;

const string fileNameVehicle = "vehicle.txt";

class Vehicle{
    public:
        Vehicle(const string& licensePlate,//input
                const string& phoneNumber,//input
                const float& height,//input
                const float& length //input
                );
        //Job: Vehicle object constructor.
        //Usage: Used when a Vehicle is being created. Created automatically if the vehicle is not found in the file.
        //Restrictions: Must comply to the domain restrictions and height/length restrictions.

        //----------------------------------------------------------------------------

        void writeVehicle(const string& filename//input
                                );
        //Job: Writes an instance of the Vehicle object to the file.
        //Usage: Used automatically when a Vehicle is registered for the Booking for the first time ever.
        //Restrictions: Isn't called when Vehicle already exists in the file.

        //----------------------------------------------------------------------------


    private:
            string licensePlate; //License plate of the vehicle.
            string phoneNumber; //Phone number associated with a vehicle.
            float height; //Vehicle's height. 0 if it is not a special-sized vehicle.
            float length; //Vehicle's length. 0 if it is not a special-sized vehicle.



};

bool isVehicleExist(const string& licensePlate//input
                           );

//Job: Returns a boolean flag indicating if the Vehicle exists.
//Usage: Call when need to find out if the Vehicle exists
//Restrictions: licensePlate must comply to domain.

//----------------------------------------------------------------------------



#endif //VEHICLE_H
