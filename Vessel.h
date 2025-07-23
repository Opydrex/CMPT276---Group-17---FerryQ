//==========================================================================
//==========================================================================
/*
MODULE NAME: Vessel.h
Rev.1 - 09/07/2025 - Vessel class header created.
Rev.2 - 24/07/2025 - Minor changes
----------------------------------------------------------------------------
This module contains functions and implementations related to Vessels.
----------------------------------------------------------------------------
*/

#ifndef VESSEL_H
#define VESSEL_H

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

const float maxLaneLength = 3600.0; // max length in meters for both the regular-sized and special-sized vehicle lanes
const string fileNameVessel = "vessel.txt";//constant that stores file name with all vessels

class Vessel {
    public:
        Vessel() = default;

        Vessel(const string& name,             // input
               const float& maxCapacitySmall,  // input
               const float& maxCapacityBig     // input
        );
        // Job: Vessel object constructor.
        // Usage: Used when a Vessel is being created.
        // Restrictions: Vessel name must be under 25 characters. Vessel name must be unique.

        void writeVessel(ofstream& outFile); // input
        // Job: Writes the Vessel data into the file.
        // Usage: Used when all data about Vessel is collected. It is then written into a data file.
        // Restrictions: Vessel mustn't already exist in the data file.

        // Setters
        void setName(const string& name);
        void setMaxCapacitySmall(float capSmall);
        void setMaxCapacityBig(float capBig);
        // Getters
        string getName() const;
        float getMaxSmall() const;
        float getMaxBig() const;

    private:
        char name[26]; // Vessel's name, up to 25 characters + null terminator
        float maxCapacitySmall; // Maximum capacity for regular-sized vehicles. Measured in meters.
        float maxCapacityBig;   // Maximum capacity for special-sized vehicles. Measured in meters.
};

//----------------------------------------------------------------------------

void createVessel(ifstream& inFile,  // input file stream
                  ofstream& outFile  // output file stream
);
// Job: Prompts the user for appropriate data and returns a Vessel object
// Usage: Called by the UI to collect vessel information from the user. The collected data can then be passed to writeVessel to save it to file.
// Restrictions: User input must conform to each variable's domain

//----------------------------------------------------------------------------

bool isVesselExist(const string& name,  // input
                   ifstream& inFile     // input file stream
);
// Job: Returns a boolean flag indicating if the Vessel exists.
// Usage: Call when need to find out if the Vessel exists when creating a Sailing.
// Restrictions: Vessel name must be under 25 characters. Vessel name must be unique.

//----------------------------------------------------------------------------

float getMaxRegularLength(const string& vesselName, ifstream& inFile);
// Job: Getter for initial regular-sized vehicles on the vessel.
// Usage: Called when creating a Sailing.
// Restrictions: Vessel name must be under 25 characters. Vessel name must exist.

//----------------------------------------------------------------------------

float getMaxSpecialLength(const string& vesselName, ifstream& inFile);
// Job: Getter for initial special-sized vehicles on the vessel.
// Usage: Called when creating a Sailing.
// Restrictions: Vessel name must be under 25 characters. Vessel name must exist.

//----------------------------------------------------------------------------

#endif // VESSEL_H
