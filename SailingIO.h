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

// Low-level sailing file operations:
int findSailingIndexByID(const string& id);
bool loadSailingByIndex(int index, Sailing& result);
bool appendSailingRecord(const Sailing& record);
bool writeSailingByIndex(int index, const Sailing& data);
int countSailingRecords();
bool deleteSailingByID(const string& sailingID, fstream& ioFile);

#endif // SAILING_IO_H