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
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

// Implementation of Vessel class functions:
Vessel::Vessel(const string& name, const float& maxCapacitySmall, const float& maxCapacityBig) {
    // Store name as fixed-length char array (ensure null-termination)
    strncpy(this->name, name.c_str(), sizeof(this->name) - 1);
    this->name[sizeof(this->name) - 1] = '\0';
    this->maxCapacitySmall = maxCapacitySmall;
    this->maxCapacityBig = maxCapacityBig;
}



// Setters
void Vessel::setName(const string& name) {
    strncpy(this->name, name.c_str(), sizeof(this->name) - 1);
    this->name[25] = '\0'; // ensure null-termination (name[25] is last char slot)
}
void Vessel::setMaxCapacitySmall(float capSmall) {
    this->maxCapacitySmall = capSmall;
}
void Vessel::setMaxCapacityBig(float capBig) {
    this->maxCapacityBig = capBig;
}

// Getters
string Vessel::getName() const {
    return string(this->name);
}
float Vessel::getMaxSmall() const {
    return this->maxCapacitySmall;
}
float Vessel::getMaxBig() const {
    return this->maxCapacityBig;
}

// Mid-level Vessel module functions:
void createVessel(fstream& inFile, fstream& outFile) {
    // Prompt user for a new vessel's details and add to file
    string name;
    float maxCapacitySmall;
    float maxCapacityBig;
    string another;
    string inputSmall, inputBig;
    while (true) {
        // Input Vessel name
        while (true) {
            cout << "Enter Vessel name (1 - 25 characters): ";
            getline(cin >> ws, name);
            if (name.empty()) return;  // allow cancel by empty input
            if (name.length() > 25) {
                cout << "Bad entry! Name is too long.\n";
                continue;
            }
                if (isVesselExist(name, inFile)) {
                    cout << "Error: A vessel named \"" << name << "\" already exists.\n";
                    continue;
            }
            break;
        }
        // Input capacity for regular vehicles
        while (true) {
            cout << "Enter vessel capacity for regular vehicles (0 - " << maxLaneLength << " meters): ";
            getline(cin >> ws, inputSmall);
            if (inputSmall.empty()) return; 
            stringstream ss(inputSmall);
            if (ss >> maxCapacitySmall && maxCapacitySmall >= 0 && maxCapacitySmall <= maxLaneLength) {
                break;
            } else {
                cout << "Bad entry! Please enter a number between 0 and " << maxLaneLength << ".\n";
            }
        }
        // Input capacity for oversize vehicles
        while (true) {
            cout << "Enter vessel capacity for oversize vehicles (0 - " << maxLaneLength << " meters): ";
            getline(cin >> ws, inputBig);
            if (inputBig.empty()) return;
            stringstream ss(inputBig);
            if (ss >> maxCapacityBig && maxCapacityBig >= 0 && maxCapacityBig <= maxLaneLength) {
                break;
            } else {
                cout << "Bad entry! Please enter a number between 0 and " << maxLaneLength << ".\n";
            }
        }
        // Create Vessel object and append to file
        Vessel v(name, maxCapacitySmall, maxCapacityBig);
        v.writeVessel(outFile);  // write new vessel record to file
        cout << "Vessel \"" << name << "\" created with capacities " 
             << maxCapacitySmall << " (regular) and " << maxCapacityBig << " (oversize).\n";
        cout << "Would you like to create another vessel? (Y/N): ";
        cin >> another;
        cin.ignore(1000, '\n');
        if (another != "Y" && another != "y") {
            return;
        }
    }
}

bool isVesselExist(const string& name, fstream& inFile) {
    // Search through open vessel file for matching vessel name
    inFile.clear();
    inFile.seekg(0, ios::beg);
    Vessel vesselRecord;
    while (inFile.read(reinterpret_cast<char*>(&vesselRecord), sizeof(Vessel))) {
        if (strncmp(vesselRecord.getName().c_str(), name.c_str(), 25) == 0) {
            return true;
        }
    }
    return false;
}

float getMaxRegularLength(const string& vesselName, fstream& inFile) {
    // Retrieve maxCapacitySmall for specified vessel by scanning file
    inFile.clear();
    inFile.seekg(0, ios::beg);
    Vessel vessel;
    while (inFile.read(reinterpret_cast<char*>(&vessel), sizeof(Vessel))) {
        if (vesselName == vessel.getName()) {
            return vessel.getMaxSmall();
        }
    }
    return -1.0f; // not found
}

float getMaxSpecialLength(const string& vesselName, fstream& inFile) {
    // Retrieve maxCapacityBig for specified vessel
    inFile.clear();
    inFile.seekg(0, ios::beg);
    Vessel vessel;
    while (inFile.read(reinterpret_cast<char*>(&vessel), sizeof(Vessel))) {
        if (vesselName == vessel.getName()) {
            return vessel.getMaxBig();
        }
    }
    return -1.0f;
}
