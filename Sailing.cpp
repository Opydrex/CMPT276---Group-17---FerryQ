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
using namespace std;

//----------------------------------------------------------------------------
// Sailing constructor
//----------------------------------------------------------------------------
//makes a sailing obj using the given infomation (id, name, small & big caps)
Sailing::Sailing(const string& sailingId,   //input
                 const string& vesselName,  //input
                 const float& currentCapacitySmall,  //input
                 const float& currentCapacityBig     //input
                )
{
    this->sailingId = sailingId; //save the sailing id
    this->vesselName = vesselName; //save vessel name
    this->currentCapacitySmall = currentCapacitySmall; //small cars
    this->currentCapacityBig = currentCapacityBig; //big vehicals
}

//----------------------------------------------------------------------------
// Writes an instance of the Sailing object to the file.
//----------------------------------------------------------------------------
//open file and stick data of sailing to it in csv like format
void Sailing::writeSailing(ofstream& outFile) {
    if(outFile.is_open()){
        outFile << sailingId << ","
                << vesselName << ","
                << currentCapacitySmall << ","
                << currentCapacityBig << endl;
        outFile.flush();
    } else {
        cerr << "Error: Unable to write to the output stream." << endl;
    }
}

//----------------------------------------------------------------------------
// Prompts the user for appropriate data and returns a Sailing object
//----------------------------------------------------------------------------
void createSailing(ofstream& sailingOutFile, ifstream& sailingInFile) {
    while (true) {
        string term, vessel, dayStr, hourStr;
        int day = 0, hour = 0;
        float capSmall = 0.f, capBig = 0.f;

        // Step 2: Enter terminal ID
        cout << "Enter departure terminal ID (ccc): ";
        getline(cin >> ws, term);
        if (term.empty()) return;
        if (term.size() != 3 || !isalpha(term[0]) || !isalpha(term[1]) || !isalpha(term[2])) {
            cout << "Bad entry! Must be exactly three letters." << endl;
            continue;
        }

        // Step 3: Enter vessel name
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

        // Step 5: Departure Day
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

        // Step 6: Departure Hour
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

        string id = term + "-" + dayStr + "-" + hourStr;

        // Check if SailingID exists already
        sailingInFile.clear();
        sailingInFile.seekg(0, ios::beg);
        if (isSailingExist(id, sailingInFile)) {
            cout << "A sailing with SailingID " << id << " already exists. Would you like to create another sailing? (Y/N)" << endl;
        } else {
            Sailing newSailing(id, vessel, capSmall, capBig);
            newSailing.writeSailing(sailingOutFile);
            cout << "Sailing successfully created. The SailingID is " << id << ". Would you like to create another sailing? (Y/N)" << endl;
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
bool deleteSailing(const string& sailingId, ofstream& outFile, ifstream& inFile){
    inFile.clear();
    inFile.seekg(0, ios::beg);

    string line;
    bool deleted = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string currentId;
        getline(ss, currentId, ',');

        if (currentId == sailingId) {
            deleted = true;
            continue;
        }

        outFile << line << '\n';
    }

    outFile.flush();
    return deleted;
}

//----------------------------------------------------------------------------
// promptToDeleteSailing: prompt user and call deleteSailing
//----------------------------------------------------------------------------
void promptToDeleteSailing(ifstream& inFile, ofstream& outFile){
    while (true) {
        cout << "Enter SailingID (ccc-dd-dd) or blank to return: ";
        string sailingId;
        getline(cin, sailingId);
        if (sailingId.empty())
            break;

        // format check
        if (sailingId.size() != 9 || sailingId[3] != '-' || sailingId[6] != '-' ||
            !isalpha(sailingId[0]) || !isalpha(sailingId[1]) || !isalpha(sailingId[2]) ||
            !isdigit(sailingId[4]) || !isdigit(sailingId[5]) ||
            !isdigit(sailingId[7]) || !isdigit(sailingId[8]))
        {
            cout << "Bad entry! SailingID must follow the format ccc-dd-dd.";
            continue;  // retry
        }

        bool ok = deleteSailing(sailingId, outFile, inFile);
        if (ok) {
            cout << "Sailing with SailingID " << sailingId << " deleted successfully.";
        } else {
            cout << "No sailing with SailingID " << sailingId << " was found.";
        }

        // ask to delete another
        cout << "Would you like to try deleting another sailing? (Y/N): ";
        string resp;
        getline(cin, resp);
        if (resp.empty() || (resp[0] != 'Y' && resp[0] != 'y'))
            break;
    }
}


//----------------------------------------------------------------------------
// Check if sailing already in file
//----------------------------------------------------------------------------
bool isSailingExist(const string& sailingId, ifstream& sailingInFile) {
    sailingInFile.clear();
    sailingInFile.seekg(0, ios::beg);
    
    string line;
    while(getline(sailingInFile, line)) {
        stringstream ss(line);
        string currentId;
        if(getline(ss, currentId, ',')) {
            if (currentId == sailingId)
                return true;
        }
    }
    return false;
}
//----------------------------------------------------------------------------
// Print paged report with exit-on-enter and error messages
//----------------------------------------------------------------------------
//shows all the sailingss in a nice list, 5 per page
void printReport(ifstream& sailingInFile) {
    sailingInFile.clear();
    sailingInFile.seekg(0, ios::beg);

    cout << "== Sailings Report ==" << endl;
    cout << left << setw(4) << "No." << "  " << setw(10) << "SailingID" << "  " 
         << setw(24) << "Vessel Name" << "  " << right << setw(5) << "LHR" 
         << setw(7) << "HHR" << "  " << setw(14) << "Total Vehicles" 
         << "  " << setw(14) << "Deck Usage(%)" << "\n";
    cout << left << string(4, '-') << "  " << string(10, '-') << "  " 
         << string(24, '-') << "  " << right << string(5, '-') 
         << string(7, '-') << "  " << string(14, '-') << "  " 
         << string(14, '-') << "\n";

    string line;
    int total = 0, page = 0;
    bool exit = false;

    while(getline(sailingInFile, line)) {
        stringstream ss(line);
        string id, vesName, freeSpaceStr, bookedSpaceStr;
        getline(ss, id, ','); 
        getline(ss, vesName, ','); 
        getline(ss, freeSpaceStr, ','); 
        getline(ss, bookedSpaceStr, ',');

        float vehicleSpace = stof(freeSpaceStr);
        float bookedSpace = stof(bookedSpaceStr);
        int totalCapacity = int(vehicleSpace + bookedSpace);
        int usage = totalCapacity ? int((bookedSpace / totalCapacity) * 100) : 0;
        string idx = to_string(total + 1) + ")";

        cout << right << setw(4) << idx << "  " << left << setw(10) << id 
             << "  " << setw(24) << vesName << "  " << right << setw(5) 
             << fixed << setprecision(1) << vehicleSpace << setw(7) 
             << bookedSpace << "  " << setw(14) << totalCapacity << "  " 
             << setw(14) << (to_string(usage) + "%") << "\n";

        total++;
        page++;

        if(page == 5) {
            string inp;
            while(true) {
                cout << "Enter M for more, 0 or Enter to exit: ";
                getline(cin, inp);
                if(inp.empty() || inp == "0") {
                    exit = true;
                    break;
                }
                if(inp == "M" || inp == "m")
                    break;
                cout << "Bad entry! Please enter M or 0 (or press Enter).\n";
            }
            if(exit) break;
            page = 0;
        }
    }
    if(!exit && total > 0)
        cout << "No more sailings.\n";
}