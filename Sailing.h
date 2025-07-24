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

const int maxSailingDay = 31; //last day of the month - can't create a sailing on day 32.
const int maxSailingHour = 23; //last hour of the day in 24-hour format - can't create a sailing on hour 25.
const string fileNameSailing = "sailing.txt";

class Sailing {
    public:
        // Default constructor
        Sailing() = default;

        // File I/O
        void writeSailing(ofstream& outFile);

        // Add readSailing() later if needed
        // void readSailing(ifstream& inFile);
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
        char sailingID[16];       // Fixed-length ID: "ccc-dd-hh" (max 11 + null terminator)
        char vesselName[26];      // Vessel name (max 25 + null)
        float currentCapacitySmall;
        float currentCapacityBig;
};

//----------------------------------------------------------------------------

bool isValidSailingID(const string& id//input
                      );

//----------------------------------------------------------------------------

void createSailing(ifstream& vesselFile,//input
                   ofstream& sailingOutFile,//input
                   ifstream& sailingInFile//input
                   );

// Job: Prompts the user for appropriate data and returns a Sailing object
// Usage: Called by the UI to collect Sailing information from the user. The collected data can then be passed to writeSailing to save it to file.
// Restrictions: User input must conform to each variable's domain

//----------------------------------------------------------------------------

bool isSailingExist(const string& sailingId,//input
                    ifstream& sailingInFile//input
                    );

//Job: Returns a boolean flag indicating if the Sailing exists.
//Usage: Call when need to find out if the Sailing exists
//Restrictions: SailingId must comply to domain requirements.

//----------------------------------------------------------------------------

bool deleteSailing(fstream& sailingIO);

//Job: Deletes a Sailing and all related bookings.
//Usage: Call when need to delete a sailing and all related bookings.
//Restrictions: Sailing must exist.

//----------------------------------------------------------------------------

void printSailingReportHeader();
// Job: Prints the standardized header and underline rows for a sailing report.
// Usage: Call before displaying sailing rows in either printReport or querySailing.
// Restrictions: None. Just prints to standard output.

//----------------------------------------------------------------------------

void printReport(ifstream& sailingInFile//input
                 );
//Job: Prints a Sailings report.
//Usage: Call when User wants to print Sailings report.
//Restrictions: At least 1 Sailing must exist.

//----------------------------------------------------------------------------

void querySailing(ifstream& sailingInFile//input
                  );
// Job: Prompts user for a valid SailingID and prints its details.
// Usage: Call when user wants to view information for a specific sailing.
// Restrictions: sailingInFile must be open and readable.

#endif //SAILING_H