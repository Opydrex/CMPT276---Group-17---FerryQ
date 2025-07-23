//==========================================================================
//==========================================================================
/*
MODULE NAME: Sailing.cpp
Rev.1 - 24/07/2025 - Sailing class implementation.
----------------------------------------------------------------------------
This module contains implementation of the Sailing class and related functions.
Unsorted fixed-length text records are used. Linear searches and simple append/deletion
(in-memory streaming via a temp file) strategies keep file I/O straightforward.
----------------------------------------------------------------------------
*/

#include "Sailing.h"
#include "Booking.h" 
#include "Vessel.h"           
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>               
#include <limits>
#include <cstring>
using namespace std;

//----------------------------------------------------------------------------
// Sailing constructor
//----------------------------------------------------------------------------
//makes a sailing obj using the given infomation (sailingID, name, small & big caps)
// Sailing::Sailing(const string& sailingId,   //input
//                  const string& vesselName,  //input
//                  const float& currentCapacitySmall,  //input
//                  const float& currentCapacityBig     //input
//                 )
// {
//     this->sailingId = sailingId; //save the sailing sailingID
//     this->vesselName = vesselName; //save vessel name
//     this->currentCapacitySmall = currentCapacitySmall; //small cars
//     this->currentCapacityBig = currentCapacityBig; //big vehicals
// }

//----------------------------------------------------------------------------
// Writes an instance of the Sailing object to the file.
//----------------------------------------------------------------------------
//open file and stick data of sailing to it in csv like format
void Sailing::writeSailing(ofstream& outFile) {
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(this), sizeof(Sailing));
    } else {
        cerr << "Error: Unable to write to the output stream." << endl;
    }
}

void Sailing::setSailingID(const string& id) {
    strncpy(sailingID, id.c_str(), sizeof(sailingID) - 1);
    sailingID[sizeof(sailingID) - 1] = '\0';
}

void Sailing::setVesselName(const string& name) {
    strncpy(vesselName, name.c_str(), sizeof(vesselName) - 1);
    vesselName[sizeof(vesselName) - 1] = '\0';
}

void Sailing::setCurrentCapacitySmall(float cap) {
    currentCapacitySmall = cap;
}

void Sailing::setCurrentCapacityBig(float cap) {
    currentCapacityBig = cap;
}

string Sailing::getSailingID() const {
    return string(sailingID);
}

string Sailing::getVesselName() const {
    return string(vesselName);
}

float Sailing::getCurrentCapacitySmall() const {
    return currentCapacitySmall;
}

float Sailing::getCurrentCapacityBig() const {
    return currentCapacityBig;
}

//----------------------------------------------------------------------------
// Check if a string is a valid SailingID in ccc-dd-dd format
//----------------------------------------------------------------------------
bool isValidSailingID(const string& id) {
    return id.size() == 9 &&
           id[3] == '-' && id[6] == '-' &&
           isalpha(id[0]) && isalpha(id[1]) && isalpha(id[2]) &&
           isdigit(id[4]) && isdigit(id[5]) &&
           isdigit(id[7]) && isdigit(id[8]);
}

//----------------------------------------------------------------------------
// Prompts the user for appropriate data and returns a Sailing object
//----------------------------------------------------------------------------
void createSailing(ifstream& vesselFile, ofstream& sailingOutFile, ifstream& sailingInFile) {
    while (true) {
        string term, vessel, dayStr, hourStr;
        int day = 0, hour = 0;
        float capSmall = 0.f, capBig = 0.f;

        // Terminal ID
        cout << "Enter departure terminal sailingID (ccc): ";
        getline(cin >> ws, term);
        if (term.empty()) return;
        if (term.size() != 3 || !isalpha(term[0]) || !isalpha(term[1]) || !isalpha(term[2])) {
            cout << "Bad entry! Must be exactly three letters." << endl;
            continue;
        }

        // Vessel Name
        while (true) {
            cout << "Enter vessel name (1-25 characters): ";
            getline(cin >> ws, vessel);
            if (vessel.empty()) return;
            if (vessel.length() > 25) {
                cout << "Bad entry! Name too long." << endl;
                continue;
            }

            capSmall = getMaxRegularLength(vessel, vesselFile);
            capBig = getMaxSpecialLength(vessel, vesselFile);

            if (capSmall == -1 || capBig == -1) {
                cout << "Error: Vessel not found. Please enter a valid vessel name." << endl;
                continue;
            }

            break;
        }

        // Day
        cout << "Enter departure day (dd): ";
        getline(cin >> ws, dayStr);
        if (dayStr.empty()) return;
        if (dayStr.size() != 2 || !isdigit(dayStr[0]) || !isdigit(dayStr[1])) {
            cout << "Bad entry! Must be two digits (01-31)." << endl;
            continue;
        }
        day = stoi(dayStr);
        if (day < 1 || day > maxSailingDay) {
            cout << "Day out of range." << endl;
            continue;
        }

        // Hour
        cout << "Enter departure hour (hh): ";
        getline(cin >> ws, hourStr);
        if (hourStr.empty()) return;
        if (hourStr.size() != 2 || !isdigit(hourStr[0]) || !isdigit(hourStr[1])) {
            cout << "Bad entry! Must be two digits (00-23)." << endl;
            continue;
        }
        hour = stoi(hourStr);
        if (hour < 0 || hour > maxSailingHour) {
            cout << "Hour out of range." << endl;
            continue;
        }

        string sailingIDStr = term + "-" + dayStr + "-" + hourStr;

        // Check for duplicate
        sailingInFile.clear();
        sailingInFile.seekg(0, ios::beg);
        if (isSailingExist(sailingIDStr, sailingInFile)) {
            cout << "A sailing with SailingID " << sailingIDStr << " already exists. Would you like to create another sailing? (Y/N)" << endl;
        } else {
            Sailing newSailing;
            newSailing.setSailingID(sailingIDStr);
            newSailing.setVesselName(vessel);
            newSailing.setCurrentCapacitySmall(capSmall);
            newSailing.setCurrentCapacityBig(capBig);

            newSailing.writeSailing(sailingOutFile);
            cout << "Sailing successfully created. The SailingID is " << sailingIDStr << ". Would you like to create another sailing? (Y/N)" << endl;
        }

        string response;
        getline(cin >> ws, response);
        if (response != "Y" && response != "y") {
            return;
        }
    }
}


//----------------------------------------------------------------------------
// Delete a sailing record
//----------------------------------------------------------------------------
bool deleteSailing(ofstream& outFile, ifstream& inFile) {
    string sailingID;
    cout << "Enter SailingID (ccc-dd-dd): ";
    getline(cin >> ws, sailingID);
    if (!isValidSailingID(sailingID)) {
        cout << "Bad entry! Must be ccc‑dd‑dd.\n";
        return false;
    }

    // Rewind input
    inFile.clear();
    inFile.seekg(0, ios::beg);

    // Filter and write only non-matching records
    bool deleted = false;
    Sailing temp;

    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        if (sailingID == temp.getSailingID()) {
            deleted = true; // Skip this one
        } else {
            outFile.write(reinterpret_cast<const char*>(&temp), sizeof(Sailing));
        }
    }

    cout << (deleted
             ? "Sailing with SailingID " + sailingID + " deleted successfully.\n"
             : "No sailing with SailingID " + sailingID + " was found.\n");

    return deleted;
}


//----------------------------------------------------------------------------
// Check if sailing already in file
//----------------------------------------------------------------------------
bool isSailingExist(const string& sailingId, ifstream& sailingInFile) {
    sailingInFile.clear();
    sailingInFile.seekg(0, ios::beg);

    Sailing temp;
    while (sailingInFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        if (sailingId == temp.getSailingID()) {
            return true;
        }
    }

    return false;
}
//----------------------------------------------------------------------------
// Print header of paged report
//----------------------------------------------------------------------------

void printSailingReportHeader() {
    //Header
    cout << "     "
         << left << setw(12) << "SailingID" << " "
         << setw(24) << "Vessel Name" << " "
         << setw(6)  << "LHR" << " "
         << setw(6)  << "HHR" << " "
         << setw(14) << "Total Vehicles" << " "
         << setw(13) << "Deck Usage(%)" << "\n";

    // Dash line spacing
    cout << "     "
         << string(12, '-') << " "
         << string(24, '-') << " "
         << string(6, '-')  << " "
         << string(6, '-')  << " "
         << string(14, '-') << " "
         << string(13, '-') << "\n";
}

//----------------------------------------------------------------------------
// Print paged report with exit-on-enter and error messages
//----------------------------------------------------------------------------
//shows all the sailingss in a nice list, 5 per page
void printReport(ifstream& sailingInFile) {
    sailingInFile.clear();
    sailingInFile.seekg(0, ios::beg);

    cout << "  " << "== Sailings Report ==" << endl;
    printSailingReportHeader();

    Sailing temp;
    int total = 0, page = 0;
    bool exit = false;

    while (sailingInFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        float LHR = temp.getCurrentCapacitySmall();
        float HHR = temp.getCurrentCapacityBig();

        int totalVehicles = 0;
        int usage = 0;

        string idx = to_string(total + 1) + ")";
        cout << right << setw(4) << idx << " "
             << left  << setw(12) << temp.getSailingID() << " "
             << setw(24) << temp.getVesselName() << " "
             << setw(6)  << fixed << setprecision(1) << LHR << " "
             << setw(6)  << HHR << " "
             << setw(14) << totalVehicles << " "
             << setw(13) << (to_string(usage) + "%") << "\n";

        total++;
        page++;

        if (page == 5) {
            cout << "  0)Exit\n";
            cout << "Enter M to print 5 more lines or 0 to exit: ";

            string inp;
            getline(cin >> ws, inp);
            if (inp.empty() || inp == "0") {
                exit = true;
                break;
            }

            while (inp != "M" && inp != "m") {
                cout << "Bad entry! Please enter M or 0 (or press Enter).\n";
                cout << "Enter M to print 5 more lines or 0 to exit: ";
                getline(cin >> ws, inp);
                if (inp.empty() || inp == "0") {
                    exit = true;
                    break;
                }
            }

            if (exit) break;
            page = 0;
        }
    }

    if (!exit && total > 0) {
        cout << "  0)Exit\n";
        cout << "No more sailings.\n";
    }
}



//----------------------------------------------------------------------------
// Prompt and print a single sailing record
//----------------------------------------------------------------------------
// prompts user for a valid SailingID, reads file, and prints corresponding information
void querySailing(ifstream& sailingInFile) {
    while (true) {
        cout << "Enter SailingID (ccc-dd-dd) or blank to return: ";
        string sailingId;

        getline(cin >> ws, sailingId);
        if (sailingId.empty())
            return;

        // format check
        if (!isValidSailingID(sailingId)) {
            cout << "Bad entry! SailingID must follow the format ccc-dd-dd." << endl;
            continue;
        }

        sailingInFile.clear();
        sailingInFile.seekg(0, ios::beg);

        Sailing temp;
        bool found = false;

        printSailingReportHeader();

        while (sailingInFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
            if (sailingId == temp.getSailingID()) {
                float freeSpace   = temp.getCurrentCapacitySmall();
                float bookedSpace = temp.getCurrentCapacityBig();
                int totalCap = static_cast<int>(freeSpace + bookedSpace);
                int usage    = totalCap ? static_cast<int>((bookedSpace / totalCap) * 100) : 0;

                cout << left
                     << setw(10) << temp.getSailingID() << "  "
                     << setw(24) << temp.getVesselName() << "  "
                     << right
                     << setw(5) << fixed << setprecision(1) << freeSpace
                     << setw(7) << bookedSpace << "  "
                     << setw(14) << totalCap << "  "
                     << setw(14) << (to_string(usage) + "%") << "\n";

                found = true;
                break;
            }
        }

        if (!found) {
            cout << "No sailing with SailingID " << sailingId << " was found." << endl;
        }

        cout << "Would you like to query another sailing? (Y/N): ";
        string resp;
        getline(cin >> ws, resp);
        if (resp.empty() || (resp[0] != 'Y' && resp[0] != 'y'))
            return;
    }
}