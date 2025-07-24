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

#include <string>
#include <fstream>

const float maxLaneLength = 3600.0f;
const std::string fileNameVessel = "vessel.txt";

class Vessel {
public:
    Vessel() = default;
    Vessel(const std::string& name, float capSmall, float capBig);

    void setName(const std::string& name);
    void setMaxCapacitySmall(float capSmall);
    void setMaxCapacityBig(float capBig);

    std::string getName() const;
    float getMaxSmall() const;
    float getMaxBig() const;

private:
    char name[26];
    float maxCapacitySmall;
    float maxCapacityBig;
};

// Mid-level interface functions (no direct file open/close)
void createVessel(std::fstream& vesselFile);
bool isVesselExist(const std::string& name, std::fstream& vesselFile);
float getMaxRegularLength(const std::string& vesselName, std::fstream& vesselFile);
float getMaxSpecialLength(const std::string& vesselName, std::fstream& vesselFile);

#endif // VESSEL_H
