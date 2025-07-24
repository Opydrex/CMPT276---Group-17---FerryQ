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
const string fileNameVessel = "vessel.dat";//constant that stores file name with all vessels

class Vessel {
    public:
        Vessel() = default;
        Vessel(const string& name,             // input: vessel name (<=25 chars)
               const float& maxCapacitySmall,  // input: total deck length for regular vehicles
               const float& maxCapacityBig     // input: total deck length for oversize vehicles
        );
        // Job: Vessel constructor initializes vessel data.
        // Usage: Called when adding a new Vessel.
        // Restrictions: name <= 25 characters.

        void writeVessel(fstream& outFile);
        // Job: Writes this Vessel record to the open vessel file.
        // Usage: Called after creating a Vessel to save it persistently.
        // Restrictions: Assumes outFile is an open fstream in binary mode.

        // Setters
        void setName(const string& name);
        void setMaxCapacitySmall(float capSmall);
        void setMaxCapacityBig(float capBig);
        // Getters
        string getName() const;
        float getMaxSmall() const;
        float getMaxBig() const;
    private:
        char name[26];           // vessel name (25 chars + null terminator)
        float maxCapacitySmall;  // total regular vehicle lane length (meters)
        float maxCapacityBig;    // total oversize vehicle lane length (meters)
};

//----------------------------------------------------------------------------

void createVessel(fstream& inFile, fstream& outFile);
// Job: Prompts user for vessel details and saves a new Vessel to file.
// Usage: Called from Sailings menu to add a new vessel (vesselFile passed for I/O).
// Restrictions: Vessel name must be unique and <=25 chars; capacities within [0, maxLaneLength].

bool isVesselExist(const string& name, fstream& inFile);
// Job: Checks if a vessel with given name exists in the vessel data file.
// Usage: Used to validate uniqueness when creating new vessel or verifying vessel existence.
// Restrictions: name <=25 chars.

float getMaxRegularLength(const string& vesselName, fstream& inFile);
// Job: Retrieves the max regular vehicle deck length for a given vessel.
// Usage: Used when creating a Sailing to initialize capacities.
// Restrictions: Vessel must exist; returns -1.0 if not found.

float getMaxSpecialLength(const string& vesselName, fstream& inFile);
// Job: Retrieves the max oversize vehicle deck length for a given vessel.
// Usage: Used when creating a Sailing to initialize capacities.
// Restrictions: Vessel must exist; returns -1.0 if not found.

#endif // VESSEL_H