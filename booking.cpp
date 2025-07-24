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
#include "Vehicle.h"
#include "Sailing.h"
#include "BookingIO.h"
#include "SailingIO.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

//-----------------------------------------------------------------------------
// Booking constructor
//----------------------------------------------------------------------------- 
Booking::Booking(const string& licensePlate,
                 const string& sailingId,
                 const string& phoneNumber,
                 const bool& checkedIn) {
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';
    strncpy(this->sailingId, sailingId.c_str(), sizeof(this->sailingId) - 1);
    this->sailingId[sizeof(this->sailingId) - 1] = '\0';
    strncpy(this->phoneNumber, phoneNumber.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0';
    this->checkedIn = checkedIn;
}

//-----------------------------------------------------------------------------
// Write this booking to the end of the already-open bookingFile
//----------------------------------------------------------------------------- 

//-----------------------------------------------------------------------------
// createBooking
// : collects user input, registers new Vehicle (if needed), then
//   delegates record creation to BookingIO::appendBookingRecord()
//----------------------------------------------------------------------------- 
void createBooking(fstream& vehicleFile,
                   fstream& bookingFile,
                   fstream& sailingFile)
{
    // gather sailing ID
    string sailingId;
    while (true) {
        cout << "Enter Sailing ID (ccc-dd-dd) or blank to cancel: ";
        getline(cin >> ws, sailingId);
        if (sailingId.empty()) return;
        if (findSailingIndexByID(sailingFile, sailingId) == -1) {
            cout << "Bad entry! Sailing ID format is ccc-dd-dd." << endl;
            continue;
        }
        if (findSailingIndexByID(sailingFile, sailingId) == -1) {
            cout << "Error: Sailing not found." << endl;
            continue;
        }
        break;
    }

    // gather license plate & ensure unique booking
    string plate;
    while (true) {
        cout << "Enter license plate (3-10 chars) or blank to cancel: ";
        getline(cin >> ws, plate);
        if (plate.empty()) return;
        if (plate.size() < 3 || plate.size() > 10) {
            cout << "Bad entry! Plate must be 3-10 chars." << endl;
            continue;
        }
        Booking dummy;
        if (loadBookingByKey(sailingId, plate, dummy, bookingFile)) {
            cout << "Error: Booking already exists for that vehicle on this sailing." << endl;
            continue;
        }
        break;
    }

    // ensure vehicle record
    float height, length;
    if (!isVehicleExist(vehicleFile, plate)) {
        // new vehicle—prompt dimensions
        while (true) {
            cout << "Enter height (0–" << maxHeight << "): ";
            if (!(cin >> height) || height < 0 || height > maxHeight) {
                cout << "Invalid. Try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
        while (true) {
            cout << "Enter length (0–" << maxLength << "): ";
            if (!(cin >> length) || length < 0 || length > maxLength) {
                cout << "Invalid. Try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
        Vehicle v(plate, height, length);
        if (!writeVehicle(vehicleFile, v)) {
            cerr << "Error: Unable to append vehicle record." << endl;
        }
    } else {
        // existing: fetch dims
        if (!getVehicleDimensions(vehicleFile, plate, length, height)) {
            cerr << "Error: Failed to read vehicle dimensions." << endl;
        } else {
            cout << "Using stored dims—H=" << height << " L=" << length << endl;
        }
    }

    // customer phone
    string phone;
    while (true) {
        cout << "Enter customer phone (at least 7 digits): ";
        getline(cin >> ws, phone);
        int cnt = 0;
        for (char c : phone) if (isdigit(c)) cnt++;
        if (cnt < 7) {
            cout << "Too few digits. Try again." << endl;
            continue;
        }
        break;
    }

    // append booking
    Booking b(plate, sailingId, phone, false);
    if (!writeBooking(b, bookingFile)) {
        cerr << "Error: Unable to append booking record." << endl;
    } else {
        cout << "Booking created for " << plate << " on " << sailingId << endl;
    }
}

//-----------------------------------------------------------------------------
// checkIn
// : look up an existing booking, collect fare, and mark checked‑in in place.
//   Uses overwrite+truncate—no full file copy.
//-----------------------------------------------------------------------------
void checkIn(fstream& bookingFile,
             fstream& vehicleFile,
             fstream& sailingFile)
{
    while (true) {
        string sid, plate;
        cout << "Enter SailingID (ccc-dd-dd) or blank to exit: ";
        getline(cin >> ws, sid);
        if (sid.empty()) return;
        if (!isValidSailingID(sid) || findSailingIndexByID(sailingFile, sid) == -1) {
            cout << "Invalid or missing sailing. Try again." << endl;
            continue;
        }

        cout << "Enter license plate (3-10 chars) or blank to exit: ";
        getline(cin >> ws, plate);
        if (plate.empty()) return;

        Booking found;
        if (!loadBookingByKey(sid, plate, found, bookingFile)) {
            cout << "Booking not found." << endl;
            continue;
        }
        if (found.getCheckedIn()) {
            cout << "Already checked in." << endl;
            continue;
        }

        // calc fare
        float L, H;
        if (!getVehicleDimensions(vehicleFile, plate, L, H)) {
            cout << "Error: Vehicle dimensions not found." << endl;
            continue;
        }
        float fare = calculateFare(L, H);
        cout << "Fare is " << fare << ". Press Enter once collected.";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // mark done: delete old + append new checked‑in
        if (!deleteBookingRecord(sid, plate, bookingFile)) {
            cerr << "Error: Unable to delete old booking record." << endl;
            return;
        }
        Booking updated(plate, sid, found.getPhoneNumber(), true);
        if (!writeBooking(updated, bookingFile)) {
            cerr << "Error: Unable to append updated booking record." << endl;
        }

        cout << "Checked in " << plate << " on " << sid << endl;
        return;
    }
}

//-----------------------------------------------------------------------------
// promptToDeleteBooking : user drive for deleteBookingRecord()
//-----------------------------------------------------------------------------
void promptToDeleteBooking(fstream& bookingFile) {
    string sid, plate;
    cout << "Enter SailingID (ccc-dd-dd): ";
    getline(cin >> ws, sid);
    if (sid.empty() || !isValidSailingID(sid)) return;
    cout << "Enter license plate (3-10 chars): ";
    getline(cin >> ws, plate);
    if (plate.empty()) return;

    bool ok = deleteBookingRecord(sid, plate, bookingFile);
    cout << (ok ? "Booking deleted" : "Booking not found") << endl;
}

//-----------------------------------------------------------------------------
// calculateFare : unchanged
//-----------------------------------------------------------------------------
float calculateFare(const float& length, const float& height) {
    float extra = 0;
    if (length > maxLengthForRegularSizedVehicle)
        extra += static_cast<int>(length - maxLengthForRegularSizedVehicle) * extraPerMeterInLength;
    if (height > maxHeightForRegularSizedVehicle)
        extra += static_cast<int>(height - maxHeightForRegularSizedVehicle) * extraPerMeterInHeight;
    return regularSizedVehicleFare + extra;
}

// Setters for Booking class:
void Booking::setSailingID(const string& id) {
    strncpy(this->sailingId, id.c_str(), sizeof(this->sailingId) - 1);
    this->sailingId[sizeof(this->sailingId) - 1] = '\0';
}
void Booking::setLicensePlate(const string& plate) {
    strncpy(this->licensePlate, plate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';
}
void Booking::setPhoneNumber(const string& phone) {
    strncpy(this->phoneNumber, phone.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0';
}
void Booking::setCheckedIn(bool status) {
    this->checkedIn = status;
}

// Getters for Booking:
string Booking::getSailingID() const {
    return sailingId;
}
string Booking::getLicensePlate() const {
    return licensePlate;
}
string Booking::getPhoneNumber() const {
    return phoneNumber;
}
bool Booking::getCheckedIn() const {
    return checkedIn;
}
