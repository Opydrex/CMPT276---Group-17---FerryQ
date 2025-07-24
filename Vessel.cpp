//==========================================================================
//==========================================================================
/*
MODULE NAME: Vessel.cpp
Rev.1 - 24/07/2025 - Vessel class implementation.
----------------------------------------------------------------------------
This module contains Vessel class implementation.
----------------------------------------------------------------------------
*/
#include "Vessel.h"
#include "VesselIO.h"
#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;

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
    string inputForLow;
    string inputForSpecial;
    // Regular capacity
    while (true) {
        cout << "Enter vessel capacity for low vehicles (0-" << maxLaneLength << "): ";
        getline(cin >> ws, inputForLow);
        if (inputForLow.empty()) return;
        stringstream ss(inputForLow);
        if (ss >> capSmall && capSmall >= 0 && capSmall <= maxLaneLength) break;
        cout << "Invalid. Try again." << endl;
    }
    // Special capacity
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
    cout << "A vessel called " << name
         << " with " << inputForLow
         << " low vehicle capacity and "
         << inputForSpecial
         << " special vehicle capacity has been created."
         << " Would you like to create another vessel? (Y/N): ";
    string resp;
    getline(cin >> ws, resp);
    if (!resp.empty() && (resp[0]=='Y'||resp[0]=='y'))
        createVessel(vesselFile);
}



// Setters/Getters
Vessel::Vessel(const string& nameStr, float s, float b) {
    setName(nameStr);
    setMaxCapacitySmall(s);
    setMaxCapacityBig(b);
}
void Vessel::setName(const string& n) {
    strncpy(name, n.c_str(), sizeof(name)-1);
    name[sizeof(name)-1] = '\0';
}

void Vessel::setMaxCapacitySmall(float c) { maxCapacitySmall = c; }
void Vessel::setMaxCapacityBig(float c)   { maxCapacityBig   = c; }
string Vessel::getName()      const { return string(name); }
float  Vessel::getMaxSmall()  const { return maxCapacitySmall; }
float  Vessel::getMaxBig()    const { return maxCapacityBig; }
