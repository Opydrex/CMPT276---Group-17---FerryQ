//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: VesselIO.h
Rev.1 - 24/07/2025 - Initial creation of low-level Vessel file I/O header.
----------------------------------------------------------------------------
This module provides low-level binary file operations for handling Vessel
records. These functions are called by mid-level modules like Vessel.cpp
to append, read, or search vessel data in the vessel file.
----------------------------------------------------------------------------
*/

#ifndef VESSEL_IO_H
#define VESSEL_IO_H

#include "Vessel.h"
#include <string>
#include <fstream>
using namespace std;

//Low-level file I/O operations for binary vessel file access.
//File must be opened before calling any of these functions.

bool writeVesselToFile(fstream& vesselFile, const Vessel& vessel);
//Job: Appends a Vessel record to the end of the binary vessel file.
//Usage: Called by createVessel after validating input.
//Restrictions: File must be open in write mode. Record must be valid.

bool doesVesselExist(fstream& vesselFile, const string& vesselName);
//Job: Checks if a vessel with the given name exists in the file.
//Usage: Called before writing a new vessel to prevent duplicates.
//Restrictions: File must be open in read mode.

float readMaxRegularLength(fstream& vesselFile, const string& vesselName);
//Job: Retrieves the max regular vehicle capacity for a specific vessel.
//Usage: Used during sailing or booking validation.
//Restrictions: File must be open in read mode. Returns -1 if not found.

float readMaxSpecialLength(fstream& vesselFile, const string& vesselName);
//Job: Retrieves the max special vehicle capacity for a specific vessel.
//Usage: Used during sailing or booking validation.
//Restrictions: File must be open in read mode. Returns -1 if not found.

#endif //VESSEL_IO_H
