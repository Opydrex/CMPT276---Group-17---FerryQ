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

bool writeVesselToFile(const Vessel& vessel) {
    ofstream out(fileNameVessel, ios::binary | ios::app);
    if (!out) return false;

    out.write(reinterpret_cast<const char*>(&vessel), sizeof(Vessel));
    return true;
}

bool doesVesselExist(const string& vesselName) {
    ifstream in(fileNameVessel, ios::binary);
    if (!in) return false;

    Vessel temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) return true;
    }
    return false;
}

float readMaxRegularLength(const string& vesselName) {
    ifstream in(fileNameVessel, ios::binary);
    if (!in) return -1.0f;

    Vessel temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) return temp.getMaxSmall();
    }
    return -1.0f;
}

float readMaxSpecialLength(const string& vesselName) {
    ifstream in(fileNameVessel, ios::binary);
    if (!in) return -1.0f;

    Vessel temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Vessel))) {
        if (temp.getName() == vesselName) return temp.getMaxBig();
    }
    return -1.0f;
}

