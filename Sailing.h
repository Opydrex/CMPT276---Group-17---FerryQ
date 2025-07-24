//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: SailingUserIO.h
Rev.1 - 09/07/2025 - SailingUserIO module header created
Rev.2 - 24/07/2025 - SailingUserIO module header edited
----------------------------------------------------------------------------
This module contains the Sailing class and high-level operations related to
creating, deleting, and querying sailing records using binary file I/O.
----------------------------------------------------------------------------
*/

#ifndef SAILING_H
#define SAILING_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//Constants used for sailing validation and file name
const int maxSailingDay = 31;              //Max valid day (1–31)
const int maxSailingHour = 23;             //Max valid hour (0–23)
const string fileNameSailing = "sailing.txt"; //Path to sailing file

//Fixed-length binary record representing a sailing
class Sailing{
public:
    Sailing() : sailingID{0}, vesselName{0}, currentCapacitySmall(0), currentCapacityBig(0){}

    //File I/O (not used under current modular design)
    void writeSailing(fstream& outFile);
    //Job: Writes this Sailing record to the given open file stream.
    //Usage: Legacy only. Prefer calling low-level I/O from SailingIO.cpp.

    //Setters
    void setSailingID(const string& id);
    void setVesselName(const string& name);
    void setCurrentCapacitySmall(float cap);
    void setCurrentCapacityBig(float cap);

    //Getters
    string getSailingID() const;
    string getVesselName() const;
    float getCurrentCapacitySmall() const;
    float getCurrentCapacityBig() const;

private:
    char sailingID[16];         //Format: ccc-dd-hh (15 + null)
    char vesselName[26];        //Vessel name (25 + null)
    float currentCapacitySmall; //Remaining regular deck length (LHR)
    float currentCapacityBig;   //Remaining oversize deck length (HHR)
};

//============================================================================
//High-level UI-driven Sailing functions (require open file streams)
//============================================================================

bool isValidSailingID(const string& id);
//Job: Validates that the given ID is in correct format: "ccc-dd-dd".
//Usage: Called before creating, deleting, or querying a sailing.
//Returns: true if format is valid; false otherwise.

void createSailing(fstream& vesselFile, fstream& sailingFile);
//Job: Prompts user to enter sailing details and saves it to the sailing file.
//Usage: Called from Sailings menu (option [1]).
//Requirements: Valid terminal, vessel must exist, proper day/hour formatting.

bool deleteSailing(fstream& sailingFile);
//Job: Prompts user for SailingID and removes that sailing from file.
//Usage: Called from Sailings menu (option [2]).
//Returns: true if sailing was deleted; false otherwise.

void printSailingReportHeader();
//Job: Prints column headers for use with sailing listings.
//Usage: Called from printReport and querySailing for consistent formatting.

void printReport(fstream& sailingFile);
//Job: Displays all sailing records in a paginated list (5 per page).
//Usage: Called from Sailings menu (option [4]).
//Requirements: sailingFile must be open; supports skip/continue on prompt.

void querySailing(fstream& sailingFile);
//Job: Prompts for a SailingID and displays full details if found.
//Usage: Called from Sailings menu (option [5]).
//Requirements: SailingID must be valid; record must exist in file.

#endif //SAILING_H
