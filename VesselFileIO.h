// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: VesselFileIO.h
// Rev.1 - 24/07/2025 - Initial creation of low-level Vessel file I/O header.
// 
// ----------------------------------------------------------------------------
// This header declares the low-level functions for direct binary file access
// to vessel records. It serves as the data persistence layer for vessels,
// abstracting away the details of reading and writing fixed-length vessel
// records from the data file.
//
// These functions are called by the VesselUserIO and SailingUserIO modules.
// ----------------------------------------------------------------------------

#ifndef VESSEL_IO_H
#define VESSEL_IO_H

#include "VesselUserIO.h"
#include <string>
#include <fstream>
using namespace std;


//----------------------------------------------------------------------------
bool writeVesselToFile(fstream& vesselFile, const Vessel& vessel);
//Job: Appends a Vessel record to the end of the binary vessel file.
//Usage: Called by createVessel after validating input.
//Restrictions: File must be open in write mode. Record must be valid.

//----------------------------------------------------------------------------
bool doesVesselExist(fstream& vesselFile, const string& vesselName);
//Job: Checks if a vessel with the given name exists in the file.
//Usage: Called before writing a new vessel to prevent duplicates.
//Restrictions: File must be open in read mode.

//----------------------------------------------------------------------------
float getMaxRegularLength(fstream& vesselFile, const string& vesselName);
//Job: Retrieves the max regular vehicle capacity for a specific vessel.
//Usage: Used during sailing or booking validation.
//Restrictions: File must be open in read mode. Returns -1 if not found.

//----------------------------------------------------------------------------
float getMaxSpecialLength(fstream& vesselFile, const string& vesselName);
//Job: Retrieves the max special vehicle capacity for a specific vessel.
//Usage: Used during sailing or booking validation.
//Restrictions: File must be open in read mode. Returns -1 if not found.

#endif //VESSEL_IO_H
