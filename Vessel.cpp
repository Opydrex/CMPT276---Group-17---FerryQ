//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: Vessel.cpp
Rev.1 - 24/07/2025 - Vessel class implementation.

----------------------------------------------------------------------------
This file handles vessel creation and implements the Vessel class. It prompts
the user for vessel details, checks for duplicates, and saves valid entries
to a binary file via the VesselIO module.

Design notes:
- All records are stored as fixed-length binary entries.
- We rely on simple linear search and append-style file writes.
- No virtual functions — keeps the object layout clean for file I/O.
- UI handles file opening/closing; this module assumes file is open.
*/

//----------------------------------------------------------------------------
#include "Vessel.h"
#include "VesselIO.h"
#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

//----------------------------------------------------------------------------
// Prompts the user to create a new vessel and saves it to file if valid.
// Recursively asks if the user wants to create another.
// Assumes vesselFile is already open.
void createVessel(fstream& vesselFile) {
    string name;
    while (true) {
        cout << "Enter Vessel name (1-25 characters): ";
        getline(cin >> ws, name);
        if (name.empty()) return;
        if (name.size() > 25) {
            cout << "Vessel name is too long. Try again." << endl;
            continue;
        }
        if (doesVesselExist(vesselFile, name)) {
            cout << "Vessel already exists. Enter a unique name." << endl;
            continue;
        }
        break;
    }

    float capSmall = 0.0f, capBig = 0.0f;
    string inputForLow, inputForSpecial;

    // Ask for regular (low) vehicle lane capacity
    while (true) {
        cout << "Enter vessel capacity for low vehicles (0-" << maxLaneLength << "): ";
        getline(cin >> ws, inputForLow);
        if (inputForLow.empty()) return;
        stringstream ss(inputForLow);
        if (ss >> capSmall && capSmall >= 0 && capSmall <= maxLaneLength) break;
        cout << "Invalid. Try again." << endl;
    }

    // Ask for special (tall or wide) vehicle lane capacity
    while (true) {
        cout << "Enter vessel capacity for special vehicles (0-" << maxLaneLength << "): ";
        getline(cin >> ws, inputForSpecial);
        if (inputForSpecial.empty()) return;
        stringstream ss(inputForSpecial);
        if (ss >> capBig && capBig >= 0 && capBig <= maxLaneLength) break;
        cout << "Invalid. Try again." << endl;
    }

    Vessel v(name, capSmall, capBig);
    if (!writeVesselToFile(vesselFile, v)) {
        cerr << "Error writing vessel to file." << endl;
        return;
    }

    // Confirm and ask to make another
    cout << "A vessel called " << name
         << " with " << inputForLow << " low vehicle capacity and "
         << inputForSpecial << " special vehicle capacity has been created."
         << " Would you like to create another vessel? (Y/N): ";
    string resp;
    getline(cin >> ws, resp);
    if (!resp.empty() && (resp[0]=='Y'||resp[0]=='y'))
        createVessel(vesselFile);
}

//----------------------------------------------------------------------------
// Basic constructor that sets all member values using setters.
Vessel::Vessel(const string& nameStr, float s, float b) {
    setName(nameStr);
    setMaxCapacitySmall(s);
    setMaxCapacityBig(b);
}

//----------------------------------------------------------------------------
// Copies the name into the internal char array (C-style string).
void Vessel::setName(const string& n) {
    strncpy(name, n.c_str(), sizeof(name)-1);
    name[sizeof(name)-1] = '\0'; // Force null termination
}

//----------------------------------------------------------------------------
// Sets the capacity for regular vehicles (low height).
void Vessel::setMaxCapacitySmall(float c) {
    maxCapacitySmall = c;
}

//----------------------------------------------------------------------------
// Sets the capacity for special vehicles (taller or larger).
void Vessel::setMaxCapacityBig(float c) {
    maxCapacityBig = c;
}

//----------------------------------------------------------------------------
// Returns the vessel name as a string.
string Vessel::getName() const {
    return string(name);
}

//----------------------------------------------------------------------------
// Returns how many regular vehicles this vessel can carry.
float Vessel::getMaxSmall() const {
    return maxCapacitySmall;
}

//----------------------------------------------------------------------------
// Returns how many special vehicles this vessel can carry.
float Vessel::getMaxBig() const {
    return maxCapacityBig;
}

//----------------------------------------------------------------------------
