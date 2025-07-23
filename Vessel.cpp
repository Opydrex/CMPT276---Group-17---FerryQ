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

using namespace std;

Vessel::Vessel(const string &name,            // input
               const float &maxCapacitySmall, // input
               const float &maxCapacityBig    // input
)
{
    this->name = name;
    this->maxCapacitySmall = maxCapacitySmall;
    this->maxCapacityBig = maxCapacityBig;
}

void Vessel::writeVessel(ofstream &outFile)
{

    // isVehicleExist is handled by the createVessel caller
    if (outFile.is_open())
    {
        // cout << "A vessel called " << this->name << " with "<< this->maxCapacitySmall << " low vehicle capacity and "<< maxCapacityBig << " special v"<< endl;//edit this according to user manual
        // all messages to user will be handled from the createVessel
        outFile << this->name << "," << this->maxCapacitySmall << "," << this->maxCapacityBig << endl;
        outFile.flush();
    }
    else
    {
        cerr << "Error: Unable to open file for writing. Check file path and permissions." << endl;
    }
}

// CREATE QUEURY A SINGLE SAILING AND ALSO ADD IT TO THE MENU (EDIT THE USER MANUAL)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\


bool isVesselExist(const string &name, ifstream &inFile)
{
    inFile.clear();            // clear any error flags
    inFile.seekg(0, ios::beg); // move read pointer to the beginning of the file

    string line;
    bool exists = false;

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string vesselNameFromFile;

        if (getline(ss, vesselNameFromFile, ','))
        {
            if (vesselNameFromFile == name)
            {
                exists = true;
                break;
            }
        }
    }

    return exists;
}

void createVessel(ifstream &inFile, ofstream &outFile)
{
    string name;
    inFile.seekg(0, ios::end);
    float maxCapacitySmall;
    float maxCapacityBig;
    string anotherVessel;
    string inputForCapacities;
    string inputForBigCapacities;
    while (true)
    {
        while (true)
        {
            cout << "Enter Vessel name (1 - 25 characters): ";
            getline(cin >> ws, name);

            if (name.empty())
            {
                // if just hit enter
                return; // return empty vessel object
            }

            if (name.length() > 25)
            {
                cout << "Bad entry! Please try again. " << endl;
                continue;
            }

            if (isVesselExist(name, inFile))
            {
                cout << "Error: Vessel with this name already exists. Please enter a unique name." << endl;
                continue;
            }

            // name is valid and unique
            break;
        }

        while (true)
        {
            cout << "Enter vessel capacity for low vehicles (0 - 3,600 meters): ";
            getline(cin >> ws, inputForCapacities);

            if (inputForCapacities.empty())
            {
                // if just hit enter
                return;
            }

            stringstream ss(inputForCapacities);
            if (ss >> maxCapacitySmall && maxCapacitySmall > 0 && maxCapacitySmall <= 3600.0)
            {
                break;
            }
            else
            {
                cout << "Bad entry! Please try again." << endl;
            }
        }

        while (true)
        {
            cout << "Enter vessel capacity for special vehicles (0 - 3,600 meters): ";
            getline(cin >> ws, inputForBigCapacities);

            if (inputForBigCapacities.empty())
            {
                // if just hit enter
                return;
            }

            stringstream ss(inputForBigCapacities);
            if (ss >> maxCapacityBig && maxCapacityBig > 0 && maxCapacityBig <= 3600.0)
            {
                break;
            }
            else
            {
                cout << "Bad Entry! Please try again. " << endl;
            }
        }

        Vessel newVessel(name, maxCapacitySmall, maxCapacityBig);

        newVessel.writeVessel(outFile);

        cout << "A vessel called " << name
             << " with " << maxCapacitySmall
             << " low vehicle capacity and "
             << maxCapacityBig
             << " special vehicle capacity has been created."
             << " Would you like to create another vessel? (Y/N): ";
        cin >> anotherVessel;
        if (anotherVessel == "Y")
        {
            createVessel(inFile, outFile);
        }
        else
        {
            return;
        }
    }
}

float getMaxRegularLength(const string &vesselName, ifstream &inFile)
{
    inFile.clear();
    inFile.seekg(0, ios::beg);

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string vesselNameFromFile;
        string regularLengthStr;

        if (getline(ss, vesselNameFromFile, ',') &&
            getline(ss, regularLengthStr, ','))
        {

            if (vesselNameFromFile == vesselName)
            {
                return stof(regularLengthStr);
            }
        }
    }
    return -1;
}

float getMaxSpecialLength(const string &vesselName, ifstream &inFile)
{
    inFile.clear();
    inFile.seekg(0, ios::beg);

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string vesselNameFromFile;
        string skipRegularLengthStr;
        string specialLengthStr;

        if (getline(ss, vesselNameFromFile, ',') &&
            getline(ss, skipRegularLengthStr, ',') &&
            getline(ss, specialLengthStr, ','))
        {

            if (vesselNameFromFile == vesselName)
            {
                return stof(specialLengthStr);
            }
        }
    }
    return -1;
}
