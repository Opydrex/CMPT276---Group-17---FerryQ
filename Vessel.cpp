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

// Vessel::Vessel(const string& name,//input
//                const float& maxCapacitySmall,//input
//                const float& maxCapacityBig//input
//                )
// {
//     this->name = name;
//     this->maxCapacitySmall = maxCapacitySmall;
//     this->maxCapacityBig = maxCapacityBig;
// }



void Vessel::writeVessel(ofstream& outFile) {
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(this), sizeof(Vessel));
        outFile.flush();
    }
    else
    {
        cerr << "Error: Unable to open file for writing. Check file path and permissions." << endl;
    }
}



// CREATE QUEURY A SINGLE SAILING AND ALSO ADD IT TO THE MENU (EDIT THE USER MANUAL)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\




bool isVesselExist(const string& name, ifstream& inFile) {
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

void createVessel(ifstream& inFile, ofstream& outFile) {
    string name;
    float maxCapacitySmall;
    float maxCapacityBig;
    string anotherVessel;
    string inputForCapacities;
    string inputForBigCapacities;

    while (true) {
        // Prompt for vessel name
        while (true) {
            cout << "Enter Vessel name (1 - 25 characters): ";
            getline(cin >> ws, name);

            if (name.empty()) return;

            if (name.length() > 25) {
                cout << "Bad entry! Please try again.\n";
                continue;
            }

            if (isVesselExist(name, inFile)) {
                cout << "Error: Vessel with this name already exists. Please enter a unique name.\n";
                continue;
            }

            break;
        }

        // Prompt for regular vehicle capacity
        while (true) {
            cout << "Enter vessel capacity for low vehicles (0 - 3,600 meters): ";
            getline(cin >> ws, inputForCapacities);

            if (inputForCapacities.empty()) return;

            stringstream ss(inputForCapacities);
            if (ss >> maxCapacitySmall && maxCapacitySmall > 0 && maxCapacitySmall <= maxLaneLength) {
                break;
            } else {
                cout << "Bad entry! Please try again.\n";
            }
        }

        // Prompt for special vehicle capacity
        while (true) {
            cout << "Enter vessel capacity for special vehicles (0 - 3,600 meters): ";
            getline(cin >> ws, inputForBigCapacities);

            if (inputForBigCapacities.empty()) return;

            stringstream ss(inputForBigCapacities);
            if (ss >> maxCapacityBig && maxCapacityBig > 0 && maxCapacityBig <= maxLaneLength) {
                break;
            } else {
                cout << "Bad entry! Please try again.\n";
            }
        }

        // Write new vessel to file
        Vessel v;
        v.setName(name);
        v.setMaxCapacitySmall(maxCapacitySmall);
        v.setMaxCapacityBig(maxCapacityBig);

        outFile.write(reinterpret_cast<const char*>(&v), sizeof(Vessel));
        outFile.flush();

        cout << "A vessel called " << name
             << " with " << maxCapacitySmall
             << " low vehicle capacity and "
             << maxCapacityBig
             << " special vehicle capacity has been created."
             << " Would you like to create another vessel? (Y/N): ";
        
        cin >> anotherVessel;
        cin.ignore(1000, '\n');
        if (anotherVessel != "Y") return;
    }
}

float getMaxRegularLength(const string& vesselName, ifstream& inFile) {
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



float getMaxSpecialLength(const string& vesselName, ifstream& inFile) {
    inFile.clear();
    inFile.seekg(0, ios::beg);

    Vessel vessel;
    while (inFile.read(reinterpret_cast<char*>(&vessel), sizeof(Vessel))) {
        if (vesselName == vessel.getName()) {
            return vessel.getMaxBig();
        }
    }
    return -1.0f; // not found
}

// Setters
void Vessel::setName(const string& name) {
    strncpy(this->name, name.c_str(), sizeof(this->name) - 1);
    this->name[25] = '\0'; // ensure null-termination
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
