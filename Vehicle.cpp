//==========================================================================
//==========================================================================
/*
MODULE NAME: Vehicle.cpp
Rev.1 - 24/07/2025 - Vehicle class implementation.
----------------------------------------------------------------------------
This module contains Vehicle class implementation.
----------------------------------------------------------------------------
*/
#include "Vehicle.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Vehicle::Vehicle(const string& licensePlate,//input
               const float& height,//input
               const float& length//input
)
{
    this->licensePlate = licensePlate;
    this->height = height;
    this->length = length;
}

void Vehicle::writeVehicle(ofstream& outFile) {
    if(outFile.is_open()) {
        //cout << "A vessel called " << this->name << " with "<< this->maxCapacitySmall << " low vehicle capacity and "<< maxCapacityBig << " special v"<< endl;//edit this according to user manual
        //all messages to user will be handled from the createVessel
        outFile << this->licensePlate << "," << this->height << "," << this->length << endl;
        outFile.flush();
    } else {
        cerr << "Error: Unable to open file for writing. Check file path and permissions." << endl;
    }

}

bool isVehicleExist(const string& licensePlate, ifstream& inFile){
    inFile.clear(); // clear any error flags
    inFile.seekg(0, ios::beg); // move read pointer to the beginning of the file

    string line;
    bool exists = false;

    while(getline(inFile, line)){
        stringstream ss(line);
        string licensePlateFromFile;

        if(getline(ss,licensePlateFromFile, ',')){
            if(licensePlateFromFile == licensePlate){
                exists = true;
                break;
            }
        }
    }

    return exists;

}