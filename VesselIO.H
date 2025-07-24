//==========================================================================
// VesselIO.h - Header file for Vessel file I/O operations
//==========================================================================

#ifndef VESSEL_IO_H
#define VESSEL_IO_H

#include "Vessel.h"
#include <string>
using namespace std;

// Constants
const string fileNameVessel = "vessel.txt";  // Change to .txt if text-based

// Appends a new vessel record to the vessel file
bool writeVesselToFile(const Vessel& vessel);

// Returns true if a vessel with the given name exists in the file
bool doesVesselExist(const string& vesselName);

// Returns max regular deck length of the vessel (or -1 if not found)
float readMaxRegularLength(const string& vesselName);

// Returns max special deck length of the vessel (or -1 if not found)
float readMaxSpecialLength(const string& vesselName);

#endif
