// SailingIO.cpp
// Implements file I/O operations for Sailing records using provided streams

#include "SailingIO.h"
#include <cstdio>    // for truncate
#include <fstream>
extern "C" int truncate(const char* path, long long length);
using namespace std;

// Append a Sailing record to the end of the open binary file
bool appendSailingRecord(fstream& outFile, const Sailing& record) {
    if (!outFile.is_open()) return false;
    outFile.clear();
    outFile.seekp(0, ios::end);
    outFile.write(reinterpret_cast<const char*>(&record), sizeof(Sailing));
    outFile.flush();
    return outFile.good();
}

// Find index of a Sailing record by ID; returns -1 if not found
int findSailingIndexByID(fstream& inFile, const string& id) {
    if (!inFile.is_open()) return -1;
    inFile.clear();
    inFile.seekg(0, ios::beg);
    Sailing temp;
    int index = 0;
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        if (temp.getSailingID() == id) return index;
        ++index;
    }
    return -1;
}

// Load the Sailing record at the given index
bool loadSailingByIndex(fstream& inFile, int index, Sailing& result) {
    if (!inFile.is_open()) return false;
    inFile.clear();
    inFile.seekg(static_cast<streampos>(index) * sizeof(Sailing), ios::beg);
    inFile.read(reinterpret_cast<char*>(&result), sizeof(Sailing));
    return inFile.gcount() == sizeof(Sailing);
}

// Overwrite a Sailing record at the given index
bool writeSailingByIndex(fstream& ioFile, int index, const Sailing& data) {
    if (!ioFile.is_open()) return false;
    ioFile.clear();
    ioFile.seekp(static_cast<streampos>(index) * sizeof(Sailing), ios::beg);
    ioFile.write(reinterpret_cast<const char*>(&data), sizeof(Sailing));
    ioFile.flush();
    return ioFile.good();
}

// Return the number of Sailing records in the open binary file
int countSailingRecords(fstream& inFile) {
    if (!inFile.is_open()) return 0;
    inFile.clear();
    inFile.seekg(0, ios::end);
    streampos size = inFile.tellg();
    return static_cast<int>(size / sizeof(Sailing));
}

// Delete a Sailing record by ID by moving the last record into its place and truncating the file
bool deleteSailingByID(fstream& ioFile, const string& sailingID) {
    int target = findSailingIndexByID(ioFile, sailingID);
    if (target < 0) return false;

    int total = countSailingRecords(ioFile);
    int lastIndex = total - 1;
    Sailing lastRec;
    if (target != lastIndex) {
        if (!loadSailingByIndex(ioFile, lastIndex, lastRec)) return false;
        ioFile.clear();
        ioFile.seekp(static_cast<streampos>(target) * sizeof(Sailing), ios::beg);
        ioFile.write(reinterpret_cast<const char*>(&lastRec), sizeof(Sailing));
        ioFile.flush();
    }

    // Close the fstream so we can truncate on disk
    ioFile.close();

    long newSize = static_cast<long>(lastIndex) * static_cast<long>(sizeof(Sailing));
    if (truncate(fileNameSailing.c_str(), newSize) != 0) {
        return false;
    }

    // Now reopen exactly as you did in main():
    ioFile.open(fileNameSailing, ios::in | ios::out | ios::binary);
    return ioFile.is_open();
}
