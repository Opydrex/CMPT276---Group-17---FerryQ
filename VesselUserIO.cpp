// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: VesselUserIO.cpp
// Rev.2 - 05/08/2025 - Updated user input logic to correctly check for blank inputs.
//                    - Functions now clear the terminal before outputting their result.
// Rev.1 - 24/07/2025 - Vessel class implementation.
//
// ----------------------------------------------------------------------------
// This module handles the user interaction workflow for creating a new vessel.
//
// What it does:
// - Prompts the user for all necessary vessel details (name, capacities).
// - Performs input validation, including checking for duplicate vessel names.
// - Calls the VesselFileIO module to write the new, validated vessel
//   record to the binary data file.
//
// Used By: Called by UserInterface.cpp from the "Sailings" menu.
// ----------------------------------------------------------------------------
#include "VesselUserIO.h"
#include "VesselFileIO.h"
#include "UserInterface.h"
#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

//----------------------------------------------------------------------------
void createVessel(fstream& vesselFile){
//Description: Prompts the user to create a new vessel and saves 
//             it to file if valid. Recursively asks if the user 
//             wants to create another. Assumes vesselFile is already open.
    string name;
    while (true){
        cout << endl << "Enter Vessel name (1-25 characters) or blank to cancel: ";
        getline(cin, name);
        name = trim(name);
        if (name.empty()) {
            system("cls");
            cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
            return;
        }
        if (name.size() > 25){
            cout << "Vessel name is too long. Try again." << endl;
            continue;
        }
        if (doesVesselExist(vesselFile, name)){
            cout << "Vessel already exists. Enter a unique name." << endl;
            continue;
        }
        break;
    }

    float capSmall = 0.0f, capBig = 0.0f;
    string inputForLow, inputForSpecial;

    //Ask for regular (low) vehicle lane capacity
    while (true){
        cout << "Enter vessel capacity for low vehicles (0 - " << maxLaneLength << " meters) or blank to cancel: ";
        getline(cin, inputForLow);
        inputForLow = trim(inputForLow);
        if (inputForLow.empty()) {
            system("cls");
            cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
            return;
        }
        stringstream ss(inputForLow);
        if (ss >> capSmall && capSmall >= 0 && capSmall <= maxLaneLength) break;
        cout << "Invalid. Try again." << endl;
    }

    //Ask for special (tall or wide) vehicle lane capacity
    while (true){
        cout << "Enter vessel capacity for special vehicles (0 - " << maxLaneLength << " meters) or blank to cancel: ";
        getline(cin, inputForSpecial);
        inputForSpecial = trim(inputForSpecial);
        if (inputForSpecial.empty()) {
            system("cls");
            cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
            return;
        }
        stringstream ss(inputForSpecial);
        if (ss >> capBig && capBig >= 0 && capBig <= maxLaneLength) break;
        cout << "Invalid. Try again." << endl;
    }

    system("cls");
    Vessel v(name, capSmall, capBig);
    if (!writeVesselToFile(vesselFile, v)){
        cerr << "Error writing vessel to file." << endl;
        return;
    }

    //Confirm and ask to make another
    cout << "A vessel called " << name
         << " with " << inputForLow << " low vehicle capacity and "
         << inputForSpecial << " special vehicle capacity has been created."
         << " Would you like to create another vessel? (Y/N): ";
    string resp;
    getline(cin, resp);
    resp = trim(resp);
    if (!resp.empty() && (resp[0]=='Y'||resp[0]=='y'))
        createVessel(vesselFile);
}

//----------------------------------------------------------------------------
Vessel::Vessel(const string& nameStr, float s, float b){
//Description: Basic constructor that sets all member values using setters.
    setName(nameStr);
    setMaxCapacitySmall(s);
    setMaxCapacityBig(b);
}

//----------------------------------------------------------------------------
void Vessel::setName(const string& n){
//Description: Copies the name into the internal char array (C-style string).
    strncpy(name, n.c_str(), sizeof(name)-1);
    name[sizeof(name)-1] = '\0'; //Force null termination
}

//----------------------------------------------------------------------------
void Vessel::setMaxCapacitySmall(float c){
//Description: Sets the capacity for regular vehicles (low height).
    maxCapacitySmall = c;
}

//----------------------------------------------------------------------------
void Vessel::setMaxCapacityBig(float c){
//Description: Sets the capacity for special vehicles (taller or larger).
    //Description: Sets the capacity for special vehicles (taller or larger).
    maxCapacityBig = c;
}

//----------------------------------------------------------------------------
string Vessel::getName() const{
//Description: Returns the vessel name as a string.
    return string(name);
}

//----------------------------------------------------------------------------
float Vessel::getMaxSmall() const{
//Description: Returns how many regular vehicles this vessel can carry.
    return maxCapacitySmall;
}

//----------------------------------------------------------------------------
float Vessel::getMaxBig() const{
//Description: Returns how many special vehicles this vessel can carry.
    return maxCapacityBig;
}


