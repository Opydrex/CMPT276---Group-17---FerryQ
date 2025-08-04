// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: SailingUserIO.h
// Rev.1 - 09/07/2025 - SailingUserIO module header created
// Rev.2 - 24/07/2025 - SailingUserIO module header edited
//
// ----------------------------------------------------------------------------
// This module defines the public interface for all high-level, user-facing
// sailing operations. It contains the Sailing class definition, which is
// structured for fixed-length binary record storage using fixed-size char
// arrays for string data.
//
// The functions declared here handle the interactive workflows for creating,
// deleting, and reporting on sailings, and are called from the UI.
// ----------------------------------------------------------------------------

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


//----------------------------------------------------------------------------
    void setSailingID(const string& id);
    //Job: Sets the unique Sailing ID.
    //Usage: Called during creation or editing of a sailing record.
    //Restrictions: Must follow the format ccc-dd-dd (e.g., YVR-08-13).

//----------------------------------------------------------------------------
    void setVesselName(const string& name);
    //Job: Sets the name of the vessel assigned to the sailing.
    //Usage: Used when linking a vessel to a sailing.
    //Restrictions: Name must match an existing vessel in the vessel file.

//----------------------------------------------------------------------------
    void setCurrentCapacitySmall(float cap);
    //Job: Sets the current deck usage for regular vehicles.
    //Usage: Called during check-in or booking cancellation.
    //Restrictions: Must be a non-negative float.

//----------------------------------------------------------------------------
    void setCurrentCapacityBig(float cap);
    //Job: Sets the current deck usage for special vehicles.
    //Usage: Called during check-in or booking cancellation.
    //Restrictions: Must be a non-negative float.

//----------------------------------------------------------------------------
    string getSailingID() const;
    //Job: Retrieves the Sailing ID.
    //Usage: Used in reports, lookups, or file operations.
    //Restrictions: None.

//----------------------------------------------------------------------------
    string getVesselName() const;
    //Job: Retrieves the name of the vessel assigned to this sailing.
    //Usage: Used for display, reporting, or file matching.
    //Restrictions: None.

//----------------------------------------------------------------------------
    float getCurrentCapacitySmall() const;
    //Job: Retrieves the current deck usage for regular vehicles.
    //Usage: Used for validation, capacity checks, or reporting.
    //Restrictions: None.

//----------------------------------------------------------------------------
    float getCurrentCapacityBig() const;
    //Job: Retrieves the current deck usage for special vehicles.
    //Usage: Used for validation, capacity checks, or reporting.
    //Restrictions: None.

private:
    char sailingID[16];         //Format: ccc-dd-hh (15 + null)
    char vesselName[26];        //Vessel name (25 + null)
    float currentCapacitySmall; //Remaining regular deck length (LHR)
    float currentCapacityBig;   //Remaining oversize deck length (HHR)
};

//----------------------------------------------------------------------------
bool isValidSailingID(const string& id);
//Job: Validates that the given ID is in correct format: "ccc-dd-dd".
//Usage: Called before creating, deleting, or querying a sailing.
//Returns: true if format is valid; false otherwise.

//----------------------------------------------------------------------------
void createSailing(fstream& vesselFile, fstream& sailingFile);
//Job: Prompts user to enter sailing details and saves it to the sailing file.
//Usage: Called from Sailings menu (option [1]).
//Requirements: Valid terminal, vessel must exist, proper day/hour formatting.

//----------------------------------------------------------------------------
bool deleteSailing(fstream& sailingFile);
//Job: Prompts user for SailingID and removes that sailing from file.
//Usage: Called from Sailings menu (option [2]).
//Returns: true if sailing was deleted; false otherwise.

//----------------------------------------------------------------------------
void printSailingReportHeader();
//Job: Prints column headers for use with sailing listings.
//Usage: Called from printReport and querySailing for consistent formatting.

//----------------------------------------------------------------------------
void printReport(fstream& sailingFile, fstream& bookingFile, fstream& vehicleFile, fstream& vesselFile);
//Job: Displays all sailing records in a paginated list (5 per page).
//Usage: Called from Sailings menu (option [4]).
//Requirements: sailingFile must be open; supports skip/continue on prompt.

//----------------------------------------------------------------------------
void querySailing(fstream& sailingFile);
//Job: Prompts for a SailingID and displays full details if found.
//Usage: Called from Sailings menu (option [5]).
//Requirements: SailingID must be valid; record must exist in file.

#endif // SAILING_H
