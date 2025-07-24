//==========================================================================
// SailingIO.cpp - Handles file I/O operations for Sailing records
//==========================================================================

#include "SailingIO.h"
#include "Sailing.h"
#include <fstream>
#include <cstdio>    // for remove, rename
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
using namespace std;

//----------------------------------------------------------------------------
// Append a Sailing record to the end of the file
//----------------------------------------------------------------------------
bool appendSailingRecord(const Sailing& record) {
    ofstream outFile(fileNameSailing, ios::binary | ios::app);
    if (!outFile) return false;

    outFile.write(reinterpret_cast<const char*>(&record), sizeof(Sailing));
    return true;
}

//----------------------------------------------------------------------------
// Truncate the sailing.txt file by 1 record
//----------------------------------------------------------------------------
void truncateSailingFile() {
    fstream file(fileNameSailing, ios::in | ios::out | ios::binary);
    if (!file) return;

    file.seekg(0, ios::end);
    streampos size = file.tellg();
    if (size < static_cast<streampos>(sizeof(Sailing))) return;

    file.close();
#ifdef _WIN32
    _chsize_s(fileno(fopen(fileNameSailing.c_str(), "rb+")), size - sizeof(Sailing));
#else
    truncate(fileNameSailing.c_str(), size - sizeof(Sailing));
#endif
}

//----------------------------------------------------------------------------
// Load a Sailing record by index
//----------------------------------------------------------------------------
bool loadSailingByIndex(int index, Sailing& result) {
    ifstream inFile(fileNameSailing, ios::binary);
    if (!inFile) return false;

    inFile.seekg(index * sizeof(Sailing), ios::beg);
    inFile.read(reinterpret_cast<char*>(&result), sizeof(Sailing));
    return inFile.gcount() == sizeof(Sailing);
}

//----------------------------------------------------------------------------
// Write a Sailing record by index (overwrite)
//----------------------------------------------------------------------------
bool writeSailingByIndex(int index, const Sailing& data) {
    fstream file(fileNameSailing, ios::in | ios::out | ios::binary);
    if (!file) return false;

    file.seekp(index * sizeof(Sailing), ios::beg);
    file.write(reinterpret_cast<const char*>(&data), sizeof(Sailing));
    file.flush();
    return true;
}

//----------------------------------------------------------------------------
// Find a Sailing record index by ID
//----------------------------------------------------------------------------
int findSailingIndexByID(const string& id) {
    ifstream inFile(fileNameSailing, ios::binary);
    if (!inFile) return -1;

    Sailing temp;
    int index = 0;
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        if (id == temp.getSailingID())
            return index;
        index++;
    }
    return -1;
}

//----------------------------------------------------------------------------
// Get number of sailing records in file
//----------------------------------------------------------------------------
int countSailingRecords() {
    ifstream file(fileNameSailing, ios::binary | ios::ate);
    if (!file) return 0;

    streampos size = file.tellg();
    return static_cast<int>(size / sizeof(Sailing));
}

bool deleteSailingByID(const string& sailingID, fstream& ioFile) {
    ioFile.clear();
    ioFile.seekg(0, ios::beg);

    Sailing temp;
    int index = 0;
    int targetIndex = -1;
    int lastIndex = -1;

    // Step 1: Find target index and total records
    while (ioFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        if (temp.getSailingID() == sailingID) {
            targetIndex = index;
        }
        lastIndex = index;
        index++;
    }

    if (targetIndex == -1) {
        return false;
    }

    // If target is last record, no overwrite needed
    if (targetIndex != lastIndex) {
        // Step 2: Read the last record
        ioFile.clear();
        ioFile.seekg(lastIndex * sizeof(Sailing), ios::beg);
        ioFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing));

        // Step 3: Overwrite target with last record
        ioFile.clear();
        ioFile.seekp(targetIndex * sizeof(Sailing), ios::beg);
        ioFile.write(reinterpret_cast<const char*>(&temp), sizeof(Sailing));
    }

    // Step 4: Truncate the file to remove the last record
    ioFile.close();

    // Truncate using standard C function
    long newSize = lastIndex * sizeof(Sailing);
    if (truncate("sailing.dat", newSize) != 0) {
        cerr << "Error truncating sailing.dat\n";
        return false;
    }

    return true;
}
