// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: SailingUserIO.cpp
// Rev.1 - 9/07/2025 - Module created.
// Rev.2 - 24/07/2025 - Updated to match UI stream logic and core operations.
//
// ----------------------------------------------------------------------------
// This module handles all high-level, interactive logic for managing sailings.
//
// What it does:
// - Implements the user-facing workflows for creating a sailing, deleting
//   sailings, viewing the full sailing report, and querying a single sailing.
// - Contains all console prompts and data validation for sailing details.
// - Calls low-level functions from SailingFileIO.cpp and VesselFileIO.cpp
//   to retrieve data and persist changes.
//
// Used By: Called by UserInterface.cpp from the "Sailings" menu.
// ----------------------------------------------------------------------------

#include "SailingFileIO.h"
#include "SailingUserIO.h"
#include "VesselFileIO.h"
#include "BookingFileIO.h"
#include "VehicleFileIO.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <regex>

using namespace std;

//----------------------------------------------------------------------------
void createSailing(fstream& vesselFile, fstream& sailingFile){
//Description: Prompts the user for sailing info and appends a new Sailing record. 
//             Also, it Checks validity of each field and makes sure the vessel exits.
    string term, vesselName, dayStr, hourStr;

    while (true){
        //Prompt for 3-letter terminal code
        cout << "Enter departure terminal ID (ccc): ";
        getline(cin >> ws, term);
        if (term.empty()) return;
        if (term.size() != 3 || !isalpha(term[0]) || !isalpha(term[1]) || !isalpha(term[2])){
            cout << "Bad entry! Must be exactly three letters." << endl;
            continue;
        }

        //Prompt for vessel name
        cout << "Enter vessel name (1-25 characters): ";
        getline(cin >> ws, vesselName);
        if (vesselName.empty()) return;
        if (vesselName.size() > 25){
            cout << "Bad entry! Name too long." << endl;
            continue;
        }

        //Check that vessel exists and retrieve capacities
        vesselFile.clear(); vesselFile.seekg(0, ios::beg);
        float capSmall = getMaxRegularLength(vesselName, vesselFile);
        vesselFile.clear(); vesselFile.seekg(0, ios::beg);
        float capBig = getMaxSpecialLength(vesselName, vesselFile);
        if (capSmall < 0 || capBig < 0){
            cout << "Error: Vessel not found. Please re-enter." << endl;
            continue;
        }

        //Prompt for departure day
        cout << "Enter departure day (dd): ";
        getline(cin >> ws, dayStr);
        if (dayStr.empty()) return;
        if (dayStr.size() != 2 || !isdigit(dayStr[0]) || !isdigit(dayStr[1])){
            cout << "Bad entry! Must be two digits." << endl;
            continue;
        }
        int day = stoi(dayStr);
        if (day < 1 || day > maxSailingDay){
            cout << "Day out of range." << endl;
            continue;
        }

        //Prompt for departure hour
        cout << "Enter departure hour (hh): ";
        getline(cin >> ws, hourStr);
        if (hourStr.empty()) return;
        if (hourStr.size() != 2 || !isdigit(hourStr[0]) || !isdigit(hourStr[1])){
            cout << "Bad entry! Must be two digits." << endl;
            continue;
        }
        int hour = stoi(hourStr);
        if (hour < 0 || hour > maxSailingHour){
            cout << "Hour out of range." << endl;
            continue;
        }

        string sailingID = term + "-" + dayStr + "-" + hourStr;

        //Prevent duplicate sailings
        if (findSailingIndexByID(sailingFile, sailingID) != -1){
            cout << "A sailing with ID " << sailingID << " already exists. Try again? (Y/N): ";
        } else{
            //Append valid sailing to file
            Sailing s;
            s.setSailingID(sailingID);
            s.setVesselName(vesselName);
            s.setCurrentCapacitySmall(capSmall);
            s.setCurrentCapacityBig(capBig);
            if (appendSailingRecord(sailingFile, s)){
                cout << "Sailing successfully created. The SailingID is " << sailingID << ". Would you like to create another sailing? (Y/N): ";
            } else{
                cout << "Error writing sailing to file." << endl;
                return;
            }
        }

        string resp;
        getline(cin >> ws, resp);
        if (resp.empty() || (resp[0] != 'Y' && resp[0] != 'y')) break;
    }
}

//----------------------------------------------------------------------------
bool deleteSailing(fstream& sailingFile){
//Description: Prompts user for sailing ID and deletes it from the file if found.
    string sailingID;
    cout << "Enter SailingID (ccc-dd-dd): ";
    getline(cin >> ws, sailingID);
    if (sailingID.empty() || !isValidSailingID(sailingID)) return false;
    bool ok = deleteSailingByID(sailingFile, sailingID);
    if (ok)
        cout << "Sailing with SailingID " << sailingID << " deleted successfully." << endl;
    else
        cout << "No sailing with SailingID " << sailingID << " was found." << endl;
    return ok;
}

//----------------------------------------------------------------------------
bool isValidSailingID(const string& id){
//Description: Checks if a string matches the expected "ccc-dd-dd" sailing format.
    regex pattern("^[A-Za-z]{3}-\\d{2}-\\d{2}$");
    return regex_match(id, pattern);
}

//----------------------------------------------------------------------------
void printSailingReportHeader(){
//Description: Prints column headers for sailing reports (used in more than one function).
    cout << "     "
         << left << setw(12) << "SailingID" << " "
         << setw(24) << "Vessel Name" << " "
         << setw(6)  << "LHR" << " "
         << setw(6)  << "HHR" << " "
         << setw(14) << "Total Vehicles" << " "
         << setw(13) << "Deck Usage(%)" << "\n";

    cout << "     "
         << string(12, '-') << " "
         << string(24, '-') << " "
         << string(6, '-')  << " "
         << string(6, '-')  << " "
         << string(14, '-') << " "
         << string(13, '-') << "\n";
}

//----------------------------------------------------------------------------
void printReport(fstream& sailingFile, fstream& bookingFile, fstream& vehicleFile, fstream& vesselFile){
//Description: Displays all sailings from file, 5 per screen.
    cout << "== Sailings Report ==" << endl;
    printSailingReportHeader();

    int count = countSailingRecords(sailingFile);
    int shown = 0;

    for (int i = 0; i < count; ++i){
        Sailing s;
        if (!loadSailingByIndex(sailingFile, i, s)) continue;
        
        string sailingID = s.getSailingID();
        string vesselName = s.getVesselName();

        bookingFile.clear();
        bookingFile.seekg(0, ios::beg);
        int vehicleCount = countBookingsForSailing(sailingID, bookingFile);

        float initialCapSmall = getMaxRegularLength(vesselName, vesselFile);
        float initialCapBig = getMaxSpecialLength(vesselName, vesselFile);

        float totalInitialCapacity = initialCapSmall + initialCapBig;
        float totalRemainingCapacity = s.getCurrentCapacitySmall() + s.getCurrentCapacityBig();
        float deckUsagePercentage = 0.0f;
        if (totalInitialCapacity > 0) {
            deckUsagePercentage = ((totalInitialCapacity - totalRemainingCapacity) / totalInitialCapacity) * 100;
        }

        cout << setw(4) << (i+1) << ") "
             << left << setw(12) << sailingID << " "
             << setw(24) << vesselName << " "
             << setw(6)  << fixed << setprecision(1) << s.getCurrentCapacitySmall() << " "
             << setw(6)  << s.getCurrentCapacityBig() << " "
             << setw(14) << vehicleCount << " "
             << setw(6) << fixed << setprecision(2) << deckUsagePercentage << "%" << endl;

        //Paginate every 5 rows
        if (++shown == 5){
            cout << "0) Exit or M for more: ";
            string in; getline(cin >> ws, in);
            if (in.empty() || in[0] == '0') break;
            shown = 0;
            printSailingReportHeader();
        }
    }
}

//----------------------------------------------------------------------------
void querySailing(fstream& sailingFile){
//Description: Asks for one SailingID and shows its detailed info.
    while (true){
        cout << "Enter SailingID (ccc-dd-dd) or blank to return: ";
        string sid; 
        getline(cin >> ws, sid);
        if (sid.empty() || !isValidSailingID(sid)) return;

        int idx = findSailingIndexByID(sailingFile, sid);
        if (idx >= 0){
            Sailing s;
            if (!loadSailingByIndex(sailingFile, idx, s)){
                cout << "Error reading sailing data.\n";
                return;
            }
            cout << "== Sailing Details ==\n";
            printSailingReportHeader();
            cout << setw(4) << "1) "
                 << left << setw(12) << s.getSailingID() << " "
                 << setw(24) << s.getVesselName() << " "
                 << setw(6)  << fixed << setprecision(1) << s.getCurrentCapacitySmall() << " "
                 << setw(6)  << s.getCurrentCapacityBig() << " "
                 << setw(14) << 0 << " "
                 << setw(13) << "0%" << "\n";
        } else{
            cout << "No sailing " << sid << " found.\n";
        }

        cout << "Query another? (Y/N): ";
        string r; 
        getline(cin >> ws, r);
        if (r.empty() || (r[0] != 'Y' && r[0] != 'y')) break;
    }
}

//----------------------------------------------------------------------------
void Sailing::setSailingID(const string& id){
//Description: Sets the sailingID in the Sailing object
    strncpy(sailingID, id.c_str(), sizeof(sailingID) - 1);
    sailingID[sizeof(sailingID) - 1] = '\0';
}

//----------------------------------------------------------------------------
void Sailing::setVesselName(const string& name){
//Description: Sets the vesselName in the Sailing object
    strncpy(vesselName, name.c_str(), sizeof(vesselName) - 1);
    vesselName[sizeof(vesselName) - 1] = '\0';
}

//----------------------------------------------------------------------------
void Sailing::setCurrentCapacitySmall(float cap){
//Description: Sets the currentCapacitySmall in the Sailing object
    currentCapacitySmall = cap;
}

//----------------------------------------------------------------------------
void Sailing::setCurrentCapacityBig(float cap){
//Description: Sets the currentCapacityBig in the Sailing object 
    currentCapacityBig = cap;
}

//----------------------------------------------------------------------------
string Sailing::getSailingID() const{
//Description: Gets the sailingID from the Sailing object  
    return string(sailingID);
}

//----------------------------------------------------------------------------
string Sailing::getVesselName() const{
//Description: Gets the vesselName from the Sailing object  
    return string(vesselName);
}

//----------------------------------------------------------------------------
float Sailing::getCurrentCapacitySmall() const{
//Description: Gets the currentCapacitySmall from the Sailing object   
    return currentCapacitySmall;
}

//----------------------------------------------------------------------------
float Sailing::getCurrentCapacityBig() const{
//Description: Gets the currentCapacityBig from the Sailing object 
    return currentCapacityBig;
}
