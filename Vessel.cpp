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
        cout << "Enter Vessel name (1-25 chars): ";
        getline(cin >> ws, name);
        if (name.empty()) return;
        if (name.size() > 25) {
            cout << "Name too long. Try again." << endl;
            continue;
        }
        if (doesVesselExist(vesselFile, name)) {
            cout << "Vessel exists. Enter a unique name." << endl;
            continue;
        }
        break;
    }

    float capSmall = 0.0f, capBig = 0.0f;
    string input;

    // Regular capacity
    while (true) {
        cout << "Enter regular capacity (0-" << maxLaneLength << "): ";
        getline(cin >> ws, input);
        if (input.empty()) return;
        stringstream ss(input);
        if (ss >> capSmall && capSmall >= 0 && capSmall <= maxLaneLength) break;
        cout << "Invalid. Try again." << endl;
    }
    // Special capacity
    while (true) {
        cout << "Enter special capacity (0-" << maxLaneLength << "): ";
        getline(cin >> ws, input);
        if (input.empty()) return;
        stringstream ss(input);
        if (ss >> capBig && capBig >= 0 && capBig <= maxLaneLength) break;
        cout << "Invalid. Try again." << endl;
    }

    Vessel v(name, capSmall, capBig);
    if (!writeVesselToFile(vesselFile, v)) {
        cerr << "Error writing vessel to file." << endl;
        return;
    }
    cout << "Created vessel " << name << "." << endl;
    cout << "Create another? (Y/N): ";
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
