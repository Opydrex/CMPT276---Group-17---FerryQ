//==========================================================================
//==========================================================================
//MODULE NAME: SailingFileIO.h - Low-level file I/O for Sailings
//Rev.1 - 24/07/2025 - Created for modular design separation
//==========================================================================

#ifndef SAILING_IO_H
#define SAILING_IO_H

#include "Sailing.h"
#include <fstream>
#include <string>
using namespace std;

//Fixed record size (sailingID = 12, vesselName = 25, 2 floats)
const int RECORD_SIZE = 12 + 25 + sizeof(float) * 2;

//----------------------------------------------------------------------------
int findSailingIndexByID(fstream& inFile, const string& id);
//Job: Scans the sailing file for a record matching the given SailingID.
//Usage: Used by delete/query/update operations to locate target.

//----------------------------------------------------------------------------
bool appendSailingRecord(fstream& outFile, const Sailing& record);
//Job: Appends a new Sailing record to the end of the binary file.
//Usage: Called during creation of new sailings.
//Restrictions: File must be open in binary write/append mode.

//----------------------------------------------------------------------------
bool loadSailingByIndex(fstream& inFile, int index, Sailing& result);
//Job: Loads a Sailing record from a specific index.
//Usage: Used for displaying or modifying an existing record.
//Restrictions: File must be open and index must be valid.

//----------------------------------------------------------------------------
bool writeSailingByIndex(fstream& ioFile, int index, const Sailing& data);
//Job: Overwrites a Sailing record at the specified index.
//Usage: Used to update sailing capacity or vessel assignment.
//Restrictions: File must be open and index must be valid.

//----------------------------------------------------------------------------
int countSailingRecords(fstream& inFile);
//Job: Counts the number of valid Sailing records in the file.
//Usage: Used for pagination, validation, and iteration.
//Restrictions: File must be open in binary read mode.

//----------------------------------------------------------------------------
bool deleteSailingByID(fstream& ioFile, const string& sailingID);
//Job: Deletes a Sailing by replacing it with the last record and truncating.
//Usage: Called by the interactive deleteSailing() workflow.
//Restrictions: File must be opened in binary read/write mode.

#endif //SAILING_IO_H
