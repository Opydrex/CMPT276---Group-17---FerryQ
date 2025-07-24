//======================== SailingIO.cpp ========================
// Implements file I/O operations for Sailing records using provided streams

#include "SailingIO.h"
#include <cstdio>    // for truncate
using namespace std;

int findSailingIndexByID(fstream& inFile, const string& id) {
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

bool appendSailingRecord(fstream& outFile, const Sailing& record) {
    if (!outFile) return false;
    outFile.clear();
    outFile.seekp(0, ios::end);
    outFile.write(reinterpret_cast<const char*>(&record), sizeof(Sailing));
    return outFile.good();
}

bool loadSailingByIndex(fstream& inFile, int index, Sailing& result) {
    if (!inFile) return false;
    inFile.clear();
    inFile.seekg(index * sizeof(Sailing), ios::beg);
    inFile.read(reinterpret_cast<char*>(&result), sizeof(Sailing));
    return inFile.gcount() == sizeof(Sailing);
}

bool writeSailingByIndex(fstream& ioFile, int index, const Sailing& data) {
    if (!ioFile) return false;
    ioFile.clear();
    ioFile.seekp(index * sizeof(Sailing), ios::beg);
    ioFile.write(reinterpret_cast<const char*>(&data), sizeof(Sailing));
    return ioFile.good();
}

int countSailingRecords(fstream& inFile) {
    if (!inFile) return 0;
    inFile.clear();
    inFile.seekg(0, ios::end);
    streampos size = inFile.tellg();
    return static_cast<int>(size / sizeof(Sailing));
}

bool deleteSailingByID(fstream& ioFile, const string& sailingID) {
    // find target index
    int target = findSailingIndexByID(ioFile, sailingID);
    if (target < 0) return false;

    int total = countSailingRecords(ioFile);
    int lastIndex = total - 1;

    Sailing lastRec;
    if (target != lastIndex) {
        // read last record
        ioFile.clear();
        ioFile.seekg(lastIndex * sizeof(Sailing), ios::beg);
        ioFile.read(reinterpret_cast<char*>(&lastRec), sizeof(Sailing));
        // overwrite target
        ioFile.clear();
        ioFile.seekp(target * sizeof(Sailing), ios::beg);
        ioFile.write(reinterpret_cast<const char*>(&lastRec), sizeof(Sailing));
    }
    // truncate file
    ioFile.close();
    long newSize = lastIndex * sizeof(Sailing);
    return truncate("sailing.dat", newSize) == 0;
}