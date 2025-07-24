//==========================================================================
// VesselIO.h - Header file for Vessel file I/O operations
//==========================================================================

#ifndef VESSEL_IO_H
#define VESSEL_IO_H

#include "Vessel.h"
#include <string>
using namespace std;

const string fileNameVessel = "vessel.dat";  // vessel data file name (binary)

// Low-level Vessel file I/O operations:
bool writeVesselToFile(fstream& vesselFile, const Vessel& vessel);
// Appends a Vessel record to the vessel file (open fstream).

bool doesVesselExist(fstream& vesselFile, const string& vesselName);
// Returns true if a vessel with given name exists in the open vessel file.

float readMaxRegularLength(fstream& vesselFile, const string& vesselName);
// Gets the max regular deck length for the vessel (returns -1 if not found).

float readMaxSpecialLength(fstream& vesselFile, const string& vesselName);
// Gets the max oversize deck length for the vessel (returns -1 if not found).

#endif // VESSEL_IO_H