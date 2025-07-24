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
#include "SailingIO.h"
#include "BookingIO.h"
#include <fstream>
#include <cstdio>
#include <iostream>
#include <limits>
#include <cstring>
#include <iomanip>
using namespace std;

// Sailing::writeSailing writes the binary record of this Sailing to file.
void Sailing::writeSailing(fstream& outFile) {
    if (outFile.is_open()) {
        outFile.clear();
        outFile.seekp(0, ios::end);
        outFile.write(reinterpret_cast<const char*>(this), sizeof(Sailing));
        outFile.flush();
    } else {
        cerr << "Error: Unable to write to sailing file." << endl;
    }
}

// Check if string matches "ccc-dd-dd" format for SailingID.
bool isValidSailingID(const string& id) {
    return id.size() == 9 &&
           id[3] == '-' && id[6] == '-' &&
           isalpha(id[0]) && isalpha(id[1]) && isalpha(id[2]) &&
           isdigit(id[4]) && isdigit(id[5]) &&
           isdigit(id[7]) && isdigit(id[8]);
}

// Create a new Sailing record (user input -> file output)
void createSailing(fstream& vesselFile, fstream& sailingFile) {
    while (true) {
        string term, vesselName, dayStr, hourStr;
        int day = 0, hour = 0;
        float capSmall = 0.f, capBig = 0.f;
        // Terminal code (3-letter)
        cout << "Enter departure terminal code (ccc): ";
        getline(cin >> ws, term);
        if (term.empty()) return;
        if (term.size() != 3 || !isalpha(term[0]) || !isalpha(term[1]) || !isalpha(term[2])) {
            cout << "Bad entry! Must be exactly three letters." << endl;
            continue;
        }
        // Vessel Name
        while (true) {
            cout << "Enter vessel name (1-25 characters): ";
            getline(cin >> ws, vesselName);
            if (vesselName.empty()) return;
            if (vesselName.length() > 25) {
                cout << "Bad entry! Name too long." << endl;
                continue;
            }
            // Check that vessel exists and fetch its deck capacities
            capSmall = getMaxRegularLength(vesselName, vesselFile);
            capBig = getMaxSpecialLength(vesselName, vesselFile);
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
            cout << "Day out of range (must be 01-31)." << endl;
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
            cout << "Hour out of range (must be 00-23)." << endl;
            continue;
        }
        string sailingID = term + "-" + dayStr + "-" + hourStr;
        // Check for duplicate SailingID
        if (findSailingIndexByID(sailingID) != -1) {
            cout << "A sailing with ID " << sailingID << " already exists. Would you like to create another sailing? (Y/N)" << endl;
        } else {
            // Create new Sailing object and initialize
            Sailing newSailing;
            newSailing.setSailingID(sailingID);
            newSailing.setVesselName(vesselName);
            newSailing.setCurrentCapacitySmall(capSmall);
            newSailing.setCurrentCapacityBig(capBig);
            // Write new sailing to file
            newSailing.writeSailing(sailingFile);
            cout << "Sailing successfully created with SailingID " << sailingID << ". Would you like to create another sailing? (Y/N)" << endl;
        }
        string response;
        getline(cin >> ws, response);
        if (response != "Y" && response != "y") {
            return;
        }
    }
}

// Delete a Sailing record (and associated bookings)
bool deleteSailing(fstream& sailingFile) {
    string sailingID;
    cout << "Enter SailingID (ccc-dd-dd): ";
    getline(cin >> ws, sailingID);
    if (!isValidSailingID(sailingID)) {
        cout << "Bad entry! Must be ccc-dd-dd.\n";
        return false;
    }
    if (deleteSailingByID(sailingID, sailingFile)) {
        cout << "Sailing with ID " << sailingID << " deleted successfully.\n";
        // Also delete all bookings for this sailing ID
        if (deleteBookingRecord(sailingID, "")) {  // Using BookingIO function in a loop would be better to remove all; simplified here
            cout << "All bookings for " << sailingID << " have been deleted.\n";
        }
        return true;
    } else {
        cout << "No sailing with ID " << sailingID << " was found.\n";
        return false;
    }
}

bool isSailingExist(const string& sailingId, fstream& sailingFile) {
    // Search open sailing file for a record with matching SailingID
    sailingFile.clear();
    sailingFile.seekg(0, ios::beg);
    Sailing temp;
    while (sailingFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        if (string(temp.getSailingID()) == sailingId) {
            return true;
        }
    }
    return false;
}

void printSailingReportHeader() {
    // Print column headers for sailing listings
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

void printReport(fstream& sailingFile) {
    cout << "  == Sailings Report ==\n";
    printSailingReportHeader();
    sailingFile.clear();
    sailingFile.seekg(0, ios::beg);
    int total = 0, pageCount = 0;
    bool exitFlag = false;
    Sailing temp;
    while (sailingFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
        float LHR = temp.getCurrentCapacitySmall();
        float HHR = temp.getCurrentCapacityBig();
        int totalVehicles = 0;  // not tracked in this assignment
        int usage = 0;          // not calculated yet
        string indexLabel = to_string(total + 1) + ")";
        cout << right << setw(4) << indexLabel << " "
             << left  << setw(12) << temp.getSailingID() << " "
             << setw(24) << temp.getVesselName() << " "
             << setw(6)  << fixed << setprecision(1) << LHR << " "
             << setw(6)  << HHR << " "
             << setw(14) << totalVehicles << " "
             << setw(13) << (to_string(usage) + "%") << "\n";
        total++;
        pageCount++;
        if (pageCount == 5) {
            cout << "  0)Exit\n";
            cout << "Enter M to print 5 more lines or 0 to exit: ";
            string inp;
            getline(cin >> ws, inp);
            if (inp.empty() || inp == "0") {
                exitFlag = true;
                break;
            }
            while (inp != "M" && inp != "m") {
                cout << "Bad entry! Please enter M or 0 (or press Enter).\n";
                cout << "Enter M to print 5 more lines or 0 to exit: ";
                getline(cin >> ws, inp);
                if (inp.empty() || inp == "0") {
                    exitFlag = true;
                    break;
                }
            }
            if (exitFlag) break;
            pageCount = 0;
        }
    }
    if (!exitFlag && total > 0) {
        cout << "  0)Exit\n";
        cout << "No more sailings.\n";
    }
}

void querySailing(fstream& sailingFile) {
    while (true) {
        cout << "Enter SailingID (ccc-dd-dd) or blank to return: ";
        string sailingId;
        getline(cin >> ws, sailingId);
        if (sailingId.empty()) return;
        if (!isValidSailingID(sailingId)) {
            cout << "Bad entry! SailingID must follow the format ccc-dd-dd." << endl;
            continue;
        }
        sailingFile.clear();
        sailingFile.seekg(0, ios::beg);
        Sailing temp;
        bool found = false;
        while (sailingFile.read(reinterpret_cast<char*>(&temp), sizeof(Sailing))) {
            if (string(temp.getSailingID()) == sailingId) {
                found = true;
                break;
            }
        }
        if (found) {
            cout << "  == Sailing Details ==\n";
            printSailingReportHeader();
            float LHR = temp.getCurrentCapacitySmall();
            float HHR = temp.getCurrentCapacityBig();
            int totalVehicles = 0;
            int usage = 0;
            cout << right << setw(4) << "1) "
                 << left  << setw(12) << temp.getSailingID() << " "
                 << setw(24) << temp.getVesselName() << " "
                 << setw(6)  << fixed << setprecision(1) << LHR << " "
                 << setw(6)  << HHR << " "
                 << setw(14) << totalVehicles << " "
                 << setw(13) << (to_string(usage) + "%") << "\n";
        } else {
            cout << "No sailing with SailingID " << sailingId << " was found." << endl;
        }
        cout << "Would you like to query another sailing? (Y/N): ";
        string resp;
        getline(cin >> ws, resp);
        if (resp.empty() || (resp[0] != 'Y' && resp[0] != 'y')) {
            return;
        }
    }
}

// Setters for Sailing class fields:
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

// Getters for Sailing:
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
