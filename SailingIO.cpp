//==========================================================================
// SailingIO.cpp - Handles file I/O operations for Sailing records
//==========================================================================

#include "SailingIO.h"
#include "Sailing.h"
#include <fstream>
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
using namespace std;

// (Low-level functions like appendSailingRecord, truncateSailingFile, loadSailingByIndex etc. 
// remain similar to original implementation, managing binary records via fstreams.)
bool appendSailingRecord(const Sailing& record) {
    ofstream outFile(fileNameSailing, ios::binary | ios::app);
    if (!outFile) return false;
    outFile.write(reinterpret_cast<const char*>(&record), sizeof(Sailing));
    return true;
}

// ... (Other functions not shown for brevity, but would include findSailingIndexByID, loadSailingByIndex, countSailingRecords, etc.)

bool deleteSailingByID(const string& sailingID, fstream& ioFile) {
    ioFile.clear();
    ioFile.seekg(0, ios::beg);
    Sailing temp;
    int index = 0;
    int targetIndex = -1;
    int lastIndex = -1;
    // Find target record index and track last index
    while (ioFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        if (temp.getSailingID() == sailingID) {
            targetIndex = index;
        }
        lastIndex = index;
        index++;
    }
    if (targetIndex == -1) {
        return false;  // not found
    }
    // If target isn't last record, replace it with last record
    if (targetIndex != lastIndex) {
        ioFile.clear();
        ioFile.seekg(lastIndex * sizeof(Sailing), ios::beg);
        ioFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing));
        ioFile.clear();
        ioFile.seekp(targetIndex * sizeof(Sailing), ios::beg);
        ioFile.write(reinterpret_cast<const char*>(&temp), sizeof(Sailing));
    }
    // Truncate file to remove last record
    ioFile.close();
    long newSize = lastIndex * sizeof(Sailing);
    truncate(fileNameSailing.c_str(), newSize);
    return true;
}