//==========================================================================
// SailingIO.h
// Rev.1 - 24/07/2025 - Handles all file-level binary I/O for Sailings
//==========================================================================

#ifndef SAILING_IO_H
#define SAILING_IO_H

#include <fstream>
#include <string>
#include <io.h>
#include "Sailing.h"
using namespace std;

// Finds a sailing by ID and returns true if found, result is filled in
bool findSailingByID(ifstream& inFile, const string& id, Sailing& result);

// Returns index of sailing in file, -1 if not found
int findSailingIndexByID(const string& id);

bool loadSailingByIndex(int index, Sailing& result);

int countSailingRecords();

// Overwrites sailing at specific index
bool overwriteSailing(fstream& ioFile, int index, const Sailing& sailing);

// Truncates the file to the given number of records
bool truncateSailingFile(const string& fileName, int recordCount);

bool appendSailingRecord(const Sailing& record);

bool deleteSailingByID(const string& sailingID, fstream& ioFile);

#endif // SAILING_IO_H