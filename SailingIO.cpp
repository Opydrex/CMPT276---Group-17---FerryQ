//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: SailingIO.cpp
Rev.1 – 24/07/2025 – Implements low-level file I/O for Sailing records.
----------------------------------------------------------------------------
This module provides binary file access functions for reading, writing,
searching, deleting, and counting Sailing records. It assumes the calling
code opens and manages the file stream, while these functions perform 
record-level operations.

Deletion is done by swapping with the last record and truncating the file.
Binary fixed-length records ensure direct access using indexing.
----------------------------------------------------------------------------
*/

#include "SailingIO.h"
#include <cstdio>    //for truncate()
#include <fstream>

extern "C" int truncate(const char* path, long long length);  //Needed on some systems for file truncation

using namespace std;

//----------------------------------------------------------------------------
bool appendSailingRecord(fstream& outFile, const Sailing& record){
    //Description: Appends a new Sailing record to the end of an open file.
    if (!outFile.is_open()) return false;

    outFile.clear();
    outFile.seekp(0, ios::end);  //Go to end of file
    outFile.write(reinterpret_cast<const char*>(&record), sizeof(Sailing));
    outFile.flush();
    return outFile.good();
}

//----------------------------------------------------------------------------
int findSailingIndexByID(fstream& inFile, const string& id){
    //Description: Searches for a Sailing record by ID and returns its index, 
    //             or -1 if not found.
    if (!inFile.is_open()) return -1;

    inFile.clear();
    inFile.seekg(0, ios::beg);  //Start from beginning

    Sailing temp;
    int index = 0;
    //Read and compare each record
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))){
        if (temp.getSailingID() == id) return index;
        ++index;
    }
    return -1;  //Not found
}

//----------------------------------------------------------------------------
bool loadSailingByIndex(fstream& inFile, int index, Sailing& result){
    //Description: Loads the Sailing record at a given index (zero-based).
    //             Returns true if read succeeded.
    if (!inFile.is_open()) return false;

    inFile.clear();
    inFile.seekg(static_cast<streampos>(index) * sizeof(Sailing), ios::beg);
    inFile.read(reinterpret_cast<char*>(&result), sizeof(Sailing));
    return inFile.gcount() == sizeof(Sailing);
}

//----------------------------------------------------------------------------
bool writeSailingByIndex(fstream& ioFile, int index, const Sailing& data){
    //Description: Overwrites a Sailing record at a specific index.
    //             Returns true if the write was successful.
    if (!ioFile.is_open()) return false;

    ioFile.clear();
    ioFile.seekp(static_cast<streampos>(index) * sizeof(Sailing), ios::beg);
    ioFile.write(reinterpret_cast<const char*>(&data), sizeof(Sailing));
    ioFile.flush();
    return ioFile.good();
}

//----------------------------------------------------------------------------
int countSailingRecords(fstream& inFile){
    //Description: Returns the number of Sailing records in the file.
    //             Assumes file is binary and uses fixed-length records.
    if (!inFile.is_open()) return 0;

    inFile.clear();
    inFile.seekg(0, ios::end);
    streampos size = inFile.tellg();
    return static_cast<int>(size / sizeof(Sailing));
}

//----------------------------------------------------------------------------
bool deleteSailingByID(fstream& ioFile, const string& sailingID){
    //Description: Deletes a Sailing record by its ID by swapping with the last record
    //             and truncating the file by one record size. Reopens the file
    //             after truncation to restore original state.
    int target = findSailingIndexByID(ioFile, sailingID);
    if (target < 0) return false;

    int total = countSailingRecords(ioFile);
    int lastIndex = total - 1;

    //If not last record, overwrite target with the last
    if (target != lastIndex){
        Sailing lastRec;
        if (!loadSailingByIndex(ioFile, lastIndex, lastRec)) return false;

        ioFile.clear();
        ioFile.seekp(static_cast<streampos>(target) * sizeof(Sailing), ios::beg);
        ioFile.write(reinterpret_cast<const char*>(&lastRec), sizeof(Sailing));
        ioFile.flush();
    }

    //Truncate the file to remove the last record
    ioFile.close();
    long newSize = static_cast<long>(lastIndex) * static_cast<long>(sizeof(Sailing));
    if (truncate(fileNameSailing.c_str(), newSize) != 0){
        return false;
    }

    //Reopen file for further operations
    ioFile.open(fileNameSailing, ios::in | ios::out | ios::binary);
    return ioFile.is_open();
}
