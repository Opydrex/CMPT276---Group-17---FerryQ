//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: VesselFileIO.cpp
Rev.1 - 24/07/2025 - Implementation of Vessel file I/O operations.

----------------------------------------------------------------------------
This module implements low-level binary file access for Vessel records.
Functions include writing, searching, and retrieving capacities from the
binary vessel file. These are utility functions called by higher-level
modules such as Vessel.cpp.
----------------------------------------------------------------------------
*/

#include "VesselIO.h"
#include "Vessel.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------
bool writeVesselToFile(fstream& vesselFile, const Vessel& vessel){
    //Description: Appends a new Vessel record to the end of the vessel file.
    //             Assumes file is already opened by caller.
    vesselFile.clear();                //Reset any fail/eof flags
    vesselFile.seekp(0, ios::end);     //Move to the end for appending

    if (!vesselFile){
        cerr << "Error: Vessel file stream is not available for writing.\n";
        return false;
    }

    vesselFile.write(reinterpret_cast<const char*>(&vessel), sizeof(Vessel));
    vesselFile.flush();                //Ensure write hits disk
    return true;
}

//----------------------------------------------------------------------------
bool doesVesselExist(fstream& vesselFile, const string& vesselName){
    //Description: Checks if a vessel with the given name exists in the file.
    //             Uses a linear search through the entire file.
    vesselFile.clear();                //Reset stream flags
    vesselFile.seekg(0, ios::beg);     //Start reading from beginning

    Vessel temp;
    //Linear search: read one record at a time
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))){
        if (temp.getName() == vesselName){
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------
float readMaxRegularLength(fstream& vesselFile, const string& vesselName){
    //Description: Retrieves the max regular (low vehicle) capacity for a given vessel name.
    //             Returns -1.0f if vessel not found.
    vesselFile.clear();                //Reset stream flags
    vesselFile.seekg(0, ios::beg);     //Start from beginning

    Vessel temp;
    //Linear search to find matching vessel
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))){
        if (temp.getName() == vesselName){
            return temp.getMaxSmall(); //Return regular capacity
        }
    }
    return -1.0f;  //Not found
}

//----------------------------------------------------------------------------
float readMaxSpecialLength(fstream& vesselFile, const string& vesselName){
    //Description: Retrieves the max special (oversize vehicle) capacity 
    //             for a given vessel. Returns -1.0f if vessel not found.
    vesselFile.clear();
    vesselFile.seekg(0, ios::beg);

    Vessel temp;
    //Linear search to find matching vessel
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))){
        if (temp.getName() == vesselName){
            return temp.getMaxBig();  //Return special capacity
        }
    }
    return -1.0f;
}

//----------------------------------------------------------------------------
