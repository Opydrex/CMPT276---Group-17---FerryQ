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
using namespace std;

const int maxSailingDay = 31; //last day of the month - can't create a sailing on day 32.
const int maxSailingHour = 23; //last hour of the day in 24-hour format - can't create a sailing on hour 25.
const std::string fileNameSailing = "sailing.txt";

class Sailing{

    public:
        Sailing(const string& sailingId,//input
                const string& vesselName,//input
                const float& currentCapacitySmall,//input
                const float& currentCapacityBig //input
                );
        //Job: Sailing object constructor.
        //Usage: Used when a Sailing is being created.
        //Restrictions: vesselName and sailingId must be unique. sailingId and vesselName must comply to domain.

        //----------------------------------------------------------------------------

        void writeSailing(ofstream& outFile//input
                          );
        //Job: Writes an instance of the Sailing object to the file.
        //Usage: Used when a Sailing is written to the file.
        //Restrictions: File exists and found.

        //----------------------------------------------------------------------------

    private:
        string sailingId;//SailingId.
        string vesselName;//Vessel that is going to complete a Sailing.
        float currentCapacitySmall;//Remaning length for regular-sized vehicles.
        float currentCapacityBig;//Remaining length for special-sized vehicles.
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

bool deleteSailing(ofstream& outFile,//input
                   ifstream& inFile//input
                   );

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