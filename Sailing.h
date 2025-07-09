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
const string fileNameSailing = "sailing.txt"; //constant that stores file name with all sailings

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

        void writeSailing(const string& filename//input
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


bool isSailingExist(const string& sailingId//input
                    );

//Job: Returns a boolean flag indicating if the Sailing exists.
//Usage: Call when need to find out if the Sailing exists
//Restrictions: SailingId must comply to domain requirements.

//----------------------------------------------------------------------------

bool deleteSailing(const string& sailingId,//input
                   );
//Job: Deletes a Sailing and all related bookings.
//Usage: Call when need to delete a sailing and all related bookings.
//Restrictions: Sailing must exist.

//----------------------------------------------------------------------------

void printReport();
//Job: Prints a Sailings report.
//Usage: Call when User wants to print Sailings report.
//Restrictions: At least 1 Sailing must exist.

//----------------------------------------------------------------------------

#endif //SAILING_H
