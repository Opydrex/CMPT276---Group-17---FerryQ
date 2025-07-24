//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// VesselIO.cpp - Implementation of Vessel file I/O operations
// Rev.1 - 24/07/2025 - Added binary random-access I/O for Vessel records
//
// This module provides low-level functions to append, search, and read
// Vessel records in a fixed-length binary file. It assumes each record is
// sizeof(Vessel) bytes and uses fstream in binary mode for random-access.

#include "VesselIO.h"
#include "Vessel.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------
// writeVesselToFile
// Job: Append a Vessel record to the end of the already-open binary file.
// Implementation: Clears EOF flags, seeks to file end, writes raw bytes of
// sizeof(Vessel), then flushes to ensure data integrity.
// Usage: Called when creating a new Vessel in the Sailings menu.
// Restrictions: vesselFile must be open in ios::in|ios::out|ios::binary.
//----------------------------------------------------------------------------
bool writeVesselToFile(fstream& vesselFile, const Vessel& vessel) {
    // Reset any error flags and position write pointer at file end
    vesselFile.clear();
    vesselFile.seekp(0, ios::end);

    // Verify stream is valid for writing
    if (!vesselFile) {
        cerr << "Error: Vessel file stream is not available for writing.\n";
        return false;
    }

    // Write the raw bytes of the Vessel object
    vesselFile.write(reinterpret_cast<const char*>(&vessel), sizeof(Vessel));
    vesselFile.flush();  // ensure the record is actually written
    return true;
}

//----------------------------------------------------------------------------
// doesVesselExist
// Job: Check if a vessel with the given name exists in the binary file.
// Implementation: Clears flags, seeks to file start, then loops reading each
// sizeof(Vessel)-byte record and compares temp.getName() to vesselName.
// Usage: Called before creating a new Vessel to enforce uniqueness.
//----------------------------------------------------------------------------
bool doesVesselExist(fstream& vesselFile, const string& vesselName) {
    //Reset flags and position read pointer at file start
    vesselFile.clear();
    vesselFile.seekg(0, ios::beg);

    Vessel temp;  //temporary storage for each record
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) {
            //Found a matching vessel name
            return true;
        }
    }
    return false;  //no match found
}

//----------------------------------------------------------------------------
// readMaxRegularLength
// Job: Locate a vessel by name and return its max regular capacity.
// Implementation: Sequentially reads records, compares names, returns
// temp.getMaxSmall() on match; returns -1.0f if not found.
//----------------------------------------------------------------------------
float readMaxRegularLength(fstream& vesselFile, const string& vesselName) {
    vesselFile.clear();
    vesselFile.seekg(0, ios::beg);

    Vessel temp;
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) {
            return temp.getMaxSmall();  // return regular capacity
        }
    }
    return -1.0f;  // sentinel for "not found"
}

//----------------------------------------------------------------------------
// readMaxSpecialLength
// Job: Locate a vessel by name and return its max special capacity.
// Implementation: Same pattern as readMaxRegularLength, uses temp.getMaxBig().
//----------------------------------------------------------------------------
float readMaxSpecialLength(fstream& vesselFile, const string& vesselName) {
    vesselFile.clear();
    vesselFile.seekg(0, ios::beg);

    Vessel temp;
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) {
            return temp.getMaxBig();  // return special capacity
        }
    }
    return -1.0f;  // sentinel for "not found"
}
