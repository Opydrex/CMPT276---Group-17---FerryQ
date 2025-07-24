//==========================================================================
//==========================================================================
/*
MODULE NAME: Sailing.h
Rev.1 - 09/07/2025 - Sailing class header created
----------------------------------------------------------------------------
This module contains functions and implementations related to Sailings.
----------------------------------------------------------------------------
*/

#ifndef SAILING_H
#define SAILING_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int maxSailingDay = 31;
const int maxSailingHour = 23;
const string fileNameSailing = "sailing.dat";

class Sailing {
    public:
        Sailing() = default;

        // File I/O
        void writeSailing(fstream& outFile);
        // Job: Writes this Sailing record to the open sailing file (binary).
        // Usage: Called when adding or updating a Sailing record.

        // (Potential readSailing function could be added if needed)

        // Setters
        void setSailingID(const string& id);
        void setVesselName(const string& name);
        void setCurrentCapacitySmall(float cap);
        void setCurrentCapacityBig(float cap);
        // Getters
        string getSailingID() const;
        string getVesselName() const;
        float getCurrentCapacitySmall() const;
        float getCurrentCapacityBig() const;
    private:
        char sailingID[16];     // format "ccc-dd-hh" + null terminator
        char vesselName[26];    // vessel name (25 + null)
        float currentCapacitySmall;
        float currentCapacityBig;
};

// High-level Sailing functions using open files:
bool isValidSailingID(const string& id);
// Checks if a string is in valid "ccc-dd-dd" format.

void createSailing(fstream& vesselFile, fstream& sailingFile);
// Job: Prompts user for new Sailing details and appends a new Sailing record.
// Usage: Called from Sailings menu (requires vesselFile to verify vessel and sailingFile to write new record).
// Restrictions: Terminal code 3 letters, day 01-31, hour 00-23; vessel must exist.

bool deleteSailing(fstream& sailingFile);
// Job: Deletes a Sailing record (and associated bookings) given a SailingID from user input.
// Usage: Called from Sailings menu. Uses open sailing file and calls booking deletion functions.
// Returns: true if a record was deleted, false if not found.

bool isSailingExist(const string& sailingId, fstream& sailingFile);
// Job: Checks if a given SailingID exists in the open sailing file.
// Usage: Used to validate Sailing existence (e.g., when creating booking or check-in).
// Returns: true if found, false if not.

void printSailingReportHeader();
// Prints the header line for sailing reports (column titles, underlines).

void printReport(fstream& sailingFile);
// Job: Displays a paginated report of up to 5 sailings per page with key metrics (LHR, HHR, etc).
// Usage: Called from Sailings menu (View Sailings Report).
// Restriction: At least one Sailing should exist to produce output.

void querySailing(fstream& sailingFile);
// Job: Prompts for a SailingID and displays detailed info for that sailing (or error if not found).
// Usage: Called from Sailings menu (Query a Sailing).
// Restriction: SailingID must be valid format; sailing must exist.

#endif // SAILING_H