//==========================================================================
// Sailing.cpp - Implementation of Sailing-related functionality
// Rev.2 - 2025-07-24 - Updated to match UI stream logic and core operations
//==========================================================================
#include "Sailing.h"
#include "SailingIO.h"
#include "VesselIO.cpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

//----------------------------------------------------------------------------
// createSailing
//----------------------------------------------------------------------------
// Prompts the user for a new Sailing and appends it to the already-open sailingFile.
void createSailing(fstream& vesselFile, fstream& sailingFile) {
    string term, vesselName, dayStr, hourStr;
    while (true) {
        // Terminal code
        cout << "Enter departure terminal ID (ccc) or blank to cancel: ";
        getline(cin >> ws, term);
        if (term.empty()) return;
        if (term.size() != 3 || !isalpha(term[0]) || !isalpha(term[1]) || !isalpha(term[2])) {
            cout << "Bad entry! Must be exactly three letters." << endl;
            continue;
        }

        // Vessel name
        cout << "Enter vessel name (1-25 chars) or blank to cancel: ";
        getline(cin >> ws, vesselName);
        if (vesselName.empty()) return;
        if (vesselName.size() > 25) {
            cout << "Bad entry! Name too long." << endl;
            continue;
        }
        // check vessel exists and get capacities
        vesselFile.clear(); vesselFile.seekg(0, ios::beg);
        float capSmall = readMaxRegularLength(vesselFile, vesselName);
        vesselFile.clear(); vesselFile.seekg(0, ios::beg);
        float capBig   = readMaxSpecialLength(vesselFile, vesselName);
        if (capSmall < 0 || capBig < 0) {
            cout << "Error: Vessel not found. Please re-enter." << endl;
            continue;
        }

        // Day
        cout << "Enter departure day (dd) or blank to cancel: ";
        getline(cin >> ws, dayStr);
        if (dayStr.empty()) return;
        if (dayStr.size() != 2 || !isdigit(dayStr[0]) || !isdigit(dayStr[1])) {
            cout << "Bad entry! Must be two digits." << endl;
            continue;
        }
        int day = stoi(dayStr);
        if (day < 1 || day > maxSailingDay) {
            cout << "Day out of range." << endl;
            continue;
        }

        // Hour
        cout << "Enter departure hour (hh) or blank to cancel: ";
        getline(cin >> ws, hourStr);
        if (hourStr.empty()) return;
        if (hourStr.size() != 2 || !isdigit(hourStr[0]) || !isdigit(hourStr[1])) {
            cout << "Bad entry! Must be two digits." << endl;
            continue;
        }
        int hour = stoi(hourStr);
        if (hour < 0 || hour > maxSailingHour) {
            cout << "Hour out of range." << endl;
            continue;
        }

        string sailingID = term + "-" + dayStr + "-" + hourStr;
        // check duplicate
        if (findSailingIndexByID(sailingFile, sailingID) != -1) {
            cout << "A sailing with ID " << sailingID << " already exists. Try again? (Y/N): ";
        } else {
            // append
            Sailing s;
            s.setSailingID(sailingID);
            s.setVesselName(vesselName);
            s.setCurrentCapacitySmall(capSmall);
            s.setCurrentCapacityBig(capBig);
            if (appendSailingRecord(sailingFile, s)) {
                cout << "Created sailing " << sailingID << ". Create another? (Y/N): ";
            } else {
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
// deleteSailing
//----------------------------------------------------------------------------
// Prompts for SailingID and deletes it from the already-open sailingFile.
bool deleteSailing(fstream& sailingFile) {
    string sailingID;
    cout << "Enter SailingID (ccc-dd-dd) or blank to cancel: ";
    getline(cin >> ws, sailingID);
    if (sailingID.empty() || !isValidSailingID(sailingID)) return false;
    bool ok = deleteSailingByID(sailingFile, sailingID);
    cout << (ok ? "Sailing deleted." : "Sailing not found.") << endl;
    return ok;
}

//----------------------------------------------------------------------------
// printReport
//----------------------------------------------------------------------------
// Displays all sails, 5 per page, with pagination prompts.
void printReport(fstream& sailingFile) {
    cout << "== Sailings Report ==" << endl;
    printSailingReportHeader();
    int count = countSailingRecords(sailingFile);
    int shown = 0;
    for (int i = 0; i < count; ++i) {
        Sailing s;
        if (!loadSailingByIndex(sailingFile, i, s)) continue;
        cout << setw(4) << (i+1) << ") "
             << left  << setw(12) << s.getSailingID() << " "
             << setw(24) << s.getVesselName() << " "
             << setw(6)  << fixed << setprecision(1) << s.getCurrentCapacitySmall() << " "
             << setw(6)  << s.getCurrentCapacityBig() << " "
             << setw(14) << 0 << " "  // TotalVehicles (future)
             << setw(13) << "0%"      // DeckUsage (future)
             << endl;
        if (++shown == 5) {
            cout << "0) Exit or M for more: ";
            string in; getline(cin >> ws, in);
            if (in.empty() || in[0]=='0') break;
            shown = 0;
            printSailingReportHeader();
        }
    }
}

//----------------------------------------------------------------------------
// querySailing
//----------------------------------------------------------------------------
// Prompts for one SailingID and prints its details.
void querySailing(fstream& sailingFile) {
    while (true) {
        cout << "Enter SailingID (ccc-dd-dd) or blank to return: ";
        string sid; getline(cin >> ws, sid);
        if (sid.empty() || !isValidSailingID(sid)) return;
        Sailing s;
        sailingFile.clear(); sailingFile.seekg(0, ios::beg);
        if (findSailingByID(sailingFile, sid, s)) {
            cout << "== Sailing Details ==" << endl;
            printSailingReportHeader();
            cout << setw(4) << "1) "
                 << left  << setw(12) << s.getSailingID() << " "
                 << setw(24) << s.getVesselName() << " "
                 << setw(6)  << fixed << setprecision(1) << s.getCurrentCapacitySmall() << " "
                 << setw(6)  << s.getCurrentCapacityBig() << " "
                 << setw(14) << 0 << " "
                 << setw(13) << "0%" << endl;
        } else {
            cout << "No sailing " << sid << " found." << endl;
        }
        cout << "Query another? (Y/N): ";
        string r; getline(cin >> ws, r);
        if (r.empty() || (r[0]!='Y' && r[0]!='y')) break;
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
