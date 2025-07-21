//==========================================================================
//==========================================================================
/*
MODULE NAME: booking.cpp
Rev.1 - 24/07/2025 - Booking class implementation.
----------------------------------------------------------------------------
This module contains the Booking class implementation, as well as supporting
functions.
----------------------------------------------------------------------------
*/
#include "Booking.h"
#include "Sailing.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Booking::Booking(const string& licensePlate,//Input
                 const string& sailingId,//Input
                 const string& phoneNumber
)
{
    this->licensePlate = licensePlate;
    this->sailingId = sailingId;
    this->phoneNumber = phoneNumber;

}

bool isCheckedIn(const string& sailingId,//input
                 const string& licensePlate,//input
                 ifstream& inFile//Input
)
{
    string line;
    while (getline(inFile, line)) {//This loop searches for
    stringstream ss(line);
    string sailingIDFromFile, licensePlateFromFile;

    if (getline(ss, sailingIDFromFile, ',') && getline(ss, licensePlateFromFile, ',')) {
        if (sailingIDFromFile == sailingId && licensePlateFromFile == licensePlate) {
            return true;
        }
    }
    return false;
    }
}

void checkIn(ifstream& inFile//Input
)
{
    inFile.clear(); // clear any error flags
    inFile.seekg(0, ios::beg); // move read pointer to the beginning of the file

    string licensePlate;
    string sailingID;
    bool exitFlag = false;

    while(true){// This loop controls the whole checkIn process
        while(true){//This loop gets the licensePlate from the user
            cout << "Enter  a SailingID (ccc-dd-dd): ";
            getline(cin >> ws, sailingID);

            if(sailingID.empty()){ //Return to main-menu if nothing is entered
                exitFlag = true;
                break;
            }

            //If the sailing ID doesn't match the correct format, let them try again
            else if (sailingID.length() != 9 || sailingID[3] != '-' || sailingID[6] != '-' ||
            !isalpha(sailingID[0]) || !isalpha(sailingID[1]) || !isalpha(sailingID[2]) ||
            !isdigit(sailingID[4]) || !isdigit(sailingID[5]) ||
            !isdigit(sailingID[7]) || !isdigit(sailingID[8])){ 
                cout << "SailingID format is incorrect. Please try again, or press Enter to exit\n";
                continue;
            }
            
            //If the sailing ID doesn't exist, let them try again
            else if(!isSailingExist(sailingID)){
                cout << "SailingID does not exist! Please try again, or press Enter to exit\n";
                continue;
            }

        }

        if(exitFlag){
            break;
        }

        while(true){//This loop gets the licensePlate from the user
            cout << "Enter the vehicle's license plate (3 - 10 characters): ";
            getline(cin >> ws, licensePlate);

            if(licensePlate.empty()){ //
                exitFlag = true;
                break;
            }
            else if(licensePlate.length() < 3 || licensePlate.length() > 10){

            }


        }

        if(exitFlag){
            break;
        }

        bool exists = isCheckedIn(sailingID, licensePlate, inFile);

        if(exists){
        //Update the checkedIn status to true.
        }
        else {
            cout << "No booking found for this license plate " << licensePlate << " and sailing " << sailingID << ".\n";
        }
}
}