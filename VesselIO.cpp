//==========================================================================
// VesselIO.cpp - Implementation of Vessel file I/O operations
//==========================================================================

#include "VesselIO.h"
#include "Vessel.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

bool writeVesselToFile(fstream& vesselFile, const Vessel& vessel) {
    // Append a Vessel record to the end of vessel file
    vesselFile.clear();
    vesselFile.seekp(0, ios::end);
    if (!vesselFile) {
        cerr << "Error: Vessel file stream is not available for writing.\n";
        return false;
    }
    vesselFile.write(reinterpret_cast<const char*>(&vessel), sizeof(Vessel));
    vesselFile.flush();
    return true;
}

bool doesVesselExist(fstream& vesselFile, const string& vesselName) {
    vesselFile.clear();  // reset any EOF flags
    vesselFile.seekg(0, ios::beg);
    Vessel temp;
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) {
            return true;
        }
    }
    return false;
}

float readMaxRegularLength(fstream& vesselFile, const string& vesselName) {
    vesselFile.clear(); // reset flags
    vesselFile.seekg(0, ios::beg);
    Vessel temp;
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) {
            return temp.getMaxSmall();
        }
    }
    return -1.0f;
}

float readMaxSpecialLength(fstream& vesselFile, const string& vesselName) {
    vesselFile.clear();
    vesselFile.seekg(0, ios::beg);
    Vessel temp;
    while (vesselFile.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) {
            return temp.getMaxBig();
        }
    }
    return -1.0f;
}