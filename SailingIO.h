//======================== SailingIO.h ========================
// Handles all file-level binary I/O for Sailings using provided streams

#ifndef SAILING_IO_H
#define SAILING_IO_H

#include "Sailing.h"
#include <fstream>
#include <string>
using namespace std;

// Finds index of a sailing record in the open binary stream
int findSailingIndexByID(fstream& inFile, const string& id);

// Appends a Sailing record to an already-open binary stream
bool appendSailingRecord(fstream& outFile, const Sailing& record);

// Loads a Sailing record by index from an open binary stream
bool loadSailingByIndex(fstream& inFile, int index, Sailing& result);

// Overwrites a Sailing record at given index in an open binary stream
bool writeSailingByIndex(fstream& ioFile, int index, const Sailing& data);

// Returns number of records in an open binary stream
int countSailingRecords(fstream& inFile);

// Deletes a Sailing by ID in an open fstream (in/out)
bool deleteSailingByID(fstream& ioFile, const string& sailingID);

#endif // SAILING_IO_H