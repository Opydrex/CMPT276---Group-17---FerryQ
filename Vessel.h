//==========================================================================
//==========================================================================
/*
MODULE NAME: Vessel.h
Rev.1 - 09/07/2025 - Vessel class header created.
----------------------------------------------------------------------------
This module contains functions and implementations related to Vessels.
----------------------------------------------------------------------------
*/

#ifndef VESSEL_H
#define VESSEL_H

#include <iostream>
#include <string>
using namespace std;

const float maxLaneLength = 3600.0; // max length in meters for both the regular-sized and special-sized vehicle lanes
const string fileNameVessel = "vessel.txt";//constant that stores file name with all vessels

class Vessel{
    public:
        Vessel(const string& name,//input
               const float& maxCapacitySmall,//input
               const float& maxCapacityBig//input
               );
        //Job: Vessel object constructor.
        //Usage: Used when a Vessel is being created.
        //Restrictions: Vessel name must be under 25 characters. Vessel name must be unique.

        //----------------------------------------------------------------------------

        void writeVessel(const string& filename//input
                         );
        //Job: Writes the Vessel data into the file.
        //Usage: Used when all data about Vessel is collected. It is then written into a data file.
        //Restrictions: Vessel mustn't already exist in the data file.

        //----------------------------------------------------------------------------




    private:
        string name;//Vessel's name.
        float maxCapacitySmall;//Maximum capacity for regular sized vehicles. Measured in meters.
        float maxCapacityBig;//Maximum capacity for special sized vehicles. Measured in meters.



};



bool isVesselExist(const string& name//input
                           );

    //Job: Returns a boolean flag indicating if the Vessel exists.
    //Usage: Call when need to find out if the Vessel exists when creating a Sailing.
    //Restrictions: Vessel name must be under 25 characters. Vessel name must be unique.

    //----------------------------------------------------------------------------

#endif //VESSEL_H
