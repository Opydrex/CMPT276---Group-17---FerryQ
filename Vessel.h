//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: VesselUserIO.h
Rev.1 - 09/07/2025 - Vessel class header created.
Rev.2 - 24/07/2025 - Minor changes
----------------------------------------------------------------------------
This module contains function declarations and data structures related to
Vessels. It defines the Vessel class, which stores information about a
vessel’s name and lane capacities, as well as file-level functions used for
creating and querying vessels.
----------------------------------------------------------------------------
*/

#ifndef VESSEL_H
#define VESSEL_H

#include <string>
#include <fstream>

const float maxLaneLength = 3600.0f;               //Max lane length in meters
const std::string fileNameVessel = "vessel.txt";   //Vessel file name

class Vessel{
public:
    Vessel() = default;
    Vessel(const std::string& name,        //input
           float capSmall,                //input
           float capBig                   //input
    );
    //Job: Constructor that initializes the vessel with a name and max capacities.
    //Usage: Automatically called when creating a vessel record.
    //Restrictions: name length 1–25 characters, capacity values in valid range.

    void setName(const std::string& name);
    //Job: Sets the vessel’s name.
    //Usage: Used when creating or modifying a vessel.
    //Restrictions: Name should be 1–25 characters, no commas.

    void setMaxCapacitySmall(float capSmall);
    //Job: Sets the vessel’s regular vehicle deck capacity.
    //Usage: Called during vessel creation or updates.
    //Restrictions: Must be a non-negative float.

    void setMaxCapacityBig(float capBig);
    //Job: Sets the vessel’s special (oversized) vehicle deck capacity.
    //Usage: Called during vessel creation or updates.
    //Restrictions: Must be a non-negative float.

    std::string getName() const;
    //Job: Retrieves the vessel’s name.
    //Usage: Used for reports, searching, or I/O.
    //Restrictions: None.

    float getMaxSmall() const;
    //Job: Retrieves the regular deck capacity.
    //Usage: Used for validations, reports, or file I/O.
    //Restrictions: None.

    float getMaxBig() const;
    //Job: Retrieves the special vehicle deck capacity.
    //Usage: Used for validations, reports, or file I/O.
    //Restrictions: None.

private:
    char name[26];             //Vessel name (max 25 chars + null terminator)
    float maxCapacitySmall;    //Capacity for regular-height vehicles
    float maxCapacityBig;      //Capacity for special/taller vehicles
};

//Mid-level interface functions (called from UI or logic, not from file I/O layers)

void createVessel(std::fstream& vesselFile);
//Job: Interactively prompts user for vessel info, validates it, and writes to file.
//Usage: Called by UI module when user wants to create a vessel.
//Restrictions: File must be open in read/write mode.

bool isVesselExist(const std::string& name, std::fstream& vesselFile);
//Job: Checks if a vessel with given name exists in file.
//Usage: Called during vessel creation or sailing validation.
//Restrictions: File must be open. Name must be within valid format.

float getMaxRegularLength(const std::string& vesselName, std::fstream& vesselFile);
//Job: Returns the max capacity for regular vehicles for the given vessel.
//Usage: Used during sailing report or vehicle loading checks.
//Restrictions: Vessel must exist. File must be open.

float getMaxSpecialLength(const std::string& vesselName, std::fstream& vesselFile);
//Job: Returns the max capacity for special vehicles for the given vessel.
//Usage: Used during sailing report or vehicle loading checks.
//Restrictions: Vessel must exist. File must be open.

#endif //VESSEL_H
