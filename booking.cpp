//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==================================================================
// booking.cpp
// Rev.1 – 24/07/2025
// Implements Booking class and all booking‑related UI functions.
//==================================================================
// In this module, we handle everything about bookings: creating new ones,
// marking check-ins, and removing records. We rely on BookingIO for file
// operations and SailingIO to make sure sailings exist before booking.

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

//----------------------------------------------------------------------------
// Booking::Booking
// Purpose: Build a Booking object from raw values.
// How it works: We copy each input string into fixed-size char arrays,
// adding a '\0' at the end to prevent overflow, and store the checked-in flag.
//----------------------------------------------------------------------------
Booking::Booking(const string& licensePlate,
                 const string& sailingId,
                 const string& phoneNumber,
                 const bool& checkedIn) {
    // Copy license plate safely
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';
    // Copy sailing ID safely
    strncpy(this->sailingId, sailingId.c_str(), sizeof(this->sailingId) - 1);
    this->sailingId[sizeof(this->sailingId) - 1] = '\0';
    // Copy phone number safely
    strncpy(this->phoneNumber, phoneNumber.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0';
    // Store checked-in flag
    this->checkedIn = checkedIn;
}

//----------------------------------------------------------------------------
// createBooking
// Purpose: Interactively get booking info and save it.
// How it works:
// 1. Ask user for a valid sailing ID (must exist).
// 2. Ask for a unique license plate on that sailing.
// 3. If the vehicle is new, prompt its dimensions and save it.
// 4. Get customer's phone number (at least 7 digits).
// 5. Finally, append the record with appendBookingRecord.
//----------------------------------------------------------------------------
void createBooking(fstream& vehicleFile,
                   fstream& bookingFile,
                   fstream& sailingFile)
{
    // gather sailing ID
    string sailingId;
    // Step 1: validate sailing
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

    string plate;
    // Step 2: ensure no duplicate booking
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
    // Step 3: handle vehicle record
    if (!isVehicleExist(vehicleFile, plate)) {
        // new vehicle, askf or dimensions
        while (true) {
            cout << "Enter height (0–" << maxHeight << "): ";
            if (!(cin >> height) || height < 0 || height > maxHeight) {
                cout << "Invalid. Try again." << endl;
                cin.clear();//flush out any stray input.
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
        while (true) {
            cout << "Enter length (0–" << maxLength << "): ";
            if (!(cin >> length) || length < 0 || length > maxLength) {
                cout << "Invalid. Try again." << endl;
                cin.clear();//flush out any stray input.
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
        //fetch existing stored dimensions
        if (!getVehicleDimensions(vehicleFile, plate, length, height)) {
            cerr << "Error: Failed to read vehicle dimensions." << endl;
        } else {
            cout << "Using stored dims—H=" << height << " L=" << length << endl;
        }
    }


    string phone;
    // Step 4: get the phone numbers
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

    // Step 5: append the booking to the file through BookingIO
    Booking b(plate, sailingId, phone, false);
    if (!writeBooking(b, bookingFile)) {
        cerr << "Error: Unable to append booking record." << endl;
    } else {
        cout << "Booking created for " << plate << " on " << sailingId << endl;
    }
}

//----------------------------------------------------------------------------
// checkIn
// Purpose: Mark an existing booking as checked-in.
// How it works:
// - Confirm booking exists and not already checked-in.
// - Calculate fare via calculateFare.
// - Delete old record, then append a new record with checkedIn=true.
//----------------------------------------------------------------------------
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

        // calculate fare here
        float L, H;
        if (!getVehicleDimensions(vehicleFile, plate, L, H)) {
            cout << "Error: Vehicle dimensions not found." << endl;
            continue;
        }
        float fare = calculateFare(L, H);
        cout << "Fare is " << fare << ". Press Enter once collected.";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //mark done: delete old + append new checked‑in
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

//----------------------------------------------------------------------------
// promptToDeleteBooking
// Purpose: Let the user delete a booking interactively.
//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
// calculateFare
// Purpose: Compute the total fare including any oversize charges.
// How it works: Adds extraPerMeter rates for each meter over size limits.
//----------------------------------------------------------------------------
float calculateFare(const float& length, const float& height) {
    float extra = 0;
    if (length > maxLengthForRegularSizedVehicle)
        extra += static_cast<int>(length - maxLengthForRegularSizedVehicle) * extraPerMeterInLength;
    if (height > maxHeightForRegularSizedVehicle)
        extra += static_cast<int>(height - maxHeightForRegularSizedVehicle) * extraPerMeterInHeight;
    return regularSizedVehicleFare + extra;
}

//----------------------------------------------------------------------------
// Booking setters/getters
//----------------------------------------------------------------------------
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
