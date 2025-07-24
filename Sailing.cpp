//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: SailingUserIO.cpp
Rev.1 - 9/07/2025 - Module created.
Rev.2 - 24/07/2025 - Updated to match UI stream logic and core operations.
----------------------------------------------------------------------------
This module implements all mid-level functions related to sailings,
including creating, deleting, printing, and querying sailing records.

It works closely with SailingIO.cpp for file access and with VesselIO.cpp
for retrieving deck capacities of vessels. Input validation is interactive.
----------------------------------------------------------------------------
*/

#include "Sailing.h"
#include "SailingIO.h"
#include "VesselIO.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <regex>

using namespace std;

//----------------------------------------------------------------------------
//createSailing
//Prompts the user for sailing info and appends a new Sailing record.
//Checks validity of each field and ensures vessel exists.
void createSailing(fstream& vesselFile, fstream& sailingFile){
    string term, vesselName, dayStr, hourStr;

    while (true){
        //Prompt for 3-letter terminal code
        cout << "Enter departure terminal ID (ccc) or blank to cancel: ";
        getline(cin >> ws, term);
        if (term.empty()) return;
        if (term.size() != 3 || !isalpha(term[0]) || !isalpha(term[1]) || !isalpha(term[2])){
            cout << "Bad entry! Must be exactly three letters." << endl;
            continue;
        }

        //Prompt for vessel name
        cout << "Enter vessel name (1-25 chars) or blank to cancel: ";
        getline(cin >> ws, vesselName);
        if (vesselName.empty()) return;
        if (vesselName.size() > 25){
            cout << "Bad entry! Name too long." << endl;
            continue;
        }

        //Check that vessel exists and retrieve capacities
        vesselFile.clear(); vesselFile.seekg(0, ios::beg);
        float capSmall = readMaxRegularLength(vesselFile, vesselName);
        vesselFile.clear(); vesselFile.seekg(0, ios::beg);
        float capBig = readMaxSpecialLength(vesselFile, vesselName);
        if (capSmall < 0 || capBig < 0){
            cout << "Error: Vessel not found. Please re-enter." << endl;
            continue;
        }

        //Prompt for departure day
        cout << "Enter departure day (dd) or blank to cancel: ";
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
        cout << "Enter departure hour (hh) or blank to cancel: ";
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
                cout << "Created sailing " << sailingID << ". Create another? (Y/N): ";
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
//deleteSailing
//Prompts user for sailing ID and deletes it from the file if found.
bool deleteSailing(fstream& sailingFile){
    string sailingID;
    cout << "Enter SailingID (ccc-dd-dd) or blank to cancel: ";
    getline(cin >> ws, sailingID);
    if (sailingID.empty() || !isValidSailingID(sailingID)) return false;
    bool ok = deleteSailingByID(sailingFile, sailingID);
    cout << (ok ? "Sailing deleted." : "Sailing not found.") << endl;
    return ok;
}

//----------------------------------------------------------------------------
//Checks if a string matches the expected "ccc-dd-dd" sailing format.
bool isValidSailingID(const string& id){
    regex pattern("^[A-Za-z]{3}-\\d{2}-\\d{2}$");
    return regex_match(id, pattern);
}

//----------------------------------------------------------------------------
//Prints column headers for sailing reports (used in multiple functions).
void printSailingReportHeader(){
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
//Displays all sailings from file, 5 per screen with pagination.
void printReport(fstream& sailingFile){
    cout << "== Sailings Report ==" << endl;
    printSailingReportHeader();

    int count = countSailingRecords(sailingFile);
    int shown = 0;

    for (int i = 0; i < count; ++i){
        Sailing s;
        if (!loadSailingByIndex(sailingFile, i, s)) continue;

        cout << setw(4) << (i+1) << ") "
             << left << setw(12) << s.getSailingID() << " "
             << setw(24) << s.getVesselName() << " "
             << setw(6)  << fixed << setprecision(1) << s.getCurrentCapacitySmall() << " "
             << setw(6)  << s.getCurrentCapacityBig() << " "
             << setw(14) << 0 << " "  //Placeholder for total vehicles
             << setw(13) << "0%" << endl; //Placeholder for deck usage

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
//Asks for one SailingID and shows its detailed info.
void querySailing(fstream& sailingFile){
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
//Sailing class setters
void Sailing::setSailingID(const string& id){
    strncpy(sailingID, id.c_str(), sizeof(sailingID) - 1);
    sailingID[sizeof(sailingID) - 1] = '\0';
}

void Sailing::setVesselName(const string& name){
    strncpy(vesselName, name.c_str(), sizeof(vesselName) - 1);
    vesselName[sizeof(vesselName) - 1] = '\0';
}

void Sailing::setCurrentCapacitySmall(float cap){
    currentCapacitySmall = cap;
}

void Sailing::setCurrentCapacityBig(float cap){
    currentCapacityBig = cap;
}

//----------------------------------------------------------------------------
//Sailing class getters
string Sailing::getSailingID() const{
    return string(sailingID);
}

string Sailing::getVesselName() const{
    return string(vesselName);
}

float Sailing::getCurrentCapacitySmall() const{
    return currentCapacitySmall;
}

float Sailing::getCurrentCapacityBig() const{
    return currentCapacityBig;
}
