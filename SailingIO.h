//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
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
//findSailingIndexByID
//----------------------------------------------------------------------------
//Job: Scans through the open sailing file and returns the index of the 
//     first record that matches the given SailingID.
//Usage: Used by delete/query/update functions.
//Returns: Zero-based index if found, -1 otherwise.
int findSailingIndexByID(fstream& inFile, const string& id);

//----------------------------------------------------------------------------
//appendSailingRecord
//----------------------------------------------------------------------------
//Job: Appends a new Sailing record to the end of the open sailing file.
//Usage: Called by createSailing.
//Returns: true if written successfully; false if stream is invalid.
bool appendSailingRecord(fstream& outFile, const Sailing& record);

//----------------------------------------------------------------------------
//loadSailingByIndex
//----------------------------------------------------------------------------
//Job: Reads the Sailing record at the specified index from the file.
//Usage: Used by printReport and querySailing.
//Returns: true if read successfully; false if stream is invalid.
bool loadSailingByIndex(fstream& inFile, int index, Sailing& result);

//----------------------------------------------------------------------------
//writeSailingByIndex
//----------------------------------------------------------------------------
//Job: Overwrites the Sailing record at the given index in the file.
//Usage: Used when updating deck capacity or other in-place edits.
//Returns: true if written successfully; false otherwise.
bool writeSailingByIndex(fstream& ioFile, int index, const Sailing& data);

//----------------------------------------------------------------------------
//countSailingRecords
//----------------------------------------------------------------------------
//Job: Returns the number of complete records currently stored in the file.
//Usage: Used for pagination and data validation.
//Returns: Record count; 0 if file is empty or unreadable.
int countSailingRecords(fstream& inFile);

//----------------------------------------------------------------------------
//deleteSailingByID
//----------------------------------------------------------------------------
//Job: Removes a Sailing record by replacing it with the last record in
//     the file and then truncating the file.
//Usage: Called by deleteSailing() in Sailing.cpp.
//Returns: true if deletion and truncation succeeded; false otherwise.
bool deleteSailingByID(fstream& ioFile, const string& sailingID);

#endif //SAILING_IO_H
