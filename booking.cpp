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
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
using namespace std;

Booking::Booking(const string& licensePlate, const string& sailingId, const string& phoneNumber, const bool& checkedIn) {
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';
    strncpy(this->sailingId, sailingId.c_str(), sizeof(this->sailingId) - 1);
    this->sailingId[sizeof(this->sailingId) - 1] = '\0';
    strncpy(this->phoneNumber, phoneNumber.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0';
    this->checkedIn = checkedIn;
}

void Booking::writeBooking(fstream& outFile) {
    if (outFile.is_open()) {
        outFile.clear();
        outFile.seekp(0, ios::end);
        outFile.write(reinterpret_cast<const char*>(this), sizeof(Booking));
        outFile.flush();
    } else {
        cerr << "Error: Unable to open booking file for writing." << endl;
    }
}

// Create a new booking by gathering user inputs and updating files
void createBooking(fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile) {
    string sailingId;
    string licensePlate;
    string phone;
    // Prompt for valid SailingID
    while (true) {
        cout << "Enter Sailing ID (ccc-dd-dd) or press Enter to cancel: ";
        getline(cin >> ws, sailingId);
        if (sailingId.empty()) return;
        if (!isValidSailingID(sailingId)) {
            cout << "Bad entry! Sailing ID format is ccc-dd-dd.\n";
            continue;
        }
        if (!isSailingExist(sailingId, sailingFile)) {
            cout << "Error: Sailing ID does not exist.\n";
            continue;
        }
        break;
    }
    // Prompt for vehicle license plate
    while (true) {
        cout << "Enter vehicle license plate (3 - 10 characters) or press Enter to cancel: ";
        getline(cin >> ws, licensePlate);
        if (licensePlate.empty()) return;
        if (licensePlate.length() < 3 || licensePlate.length() > 10) {
            cout << "Bad entry! License plate must be 3-10 characters.\n";
            continue;
        }
        // Ensure no existing booking with same sailing and vehicle
        bookingFile.clear();
        bookingFile.seekg(0, ios::beg);
        Booking temp;
        bool bookingFound = false;
        while (bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
            if (string(temp.getSailingID()) == sailingId && string(temp.getLicensePlate()) == licensePlate) {
                bookingFound = true;
                break;
            }
        }
        if (bookingFound) {
            cout << "Error: A booking for that vehicle on this sailing already exists.\n";
            continue;
        }
        break;
    }
    // Check if vehicle is already known in system
    bool vehicleExists = isVehicleExist(vehicleFile, licensePlate);
    float vehHeight = 0.0f, vehLength = 0.0f;
    if (!vehicleExists) {
        // Prompt for vehicle dimensions if new vehicle
        string heightStr, lengthStr;
        while (true) {
            cout << "Enter vehicle height (up to " << maxHeight << " m): ";
            getline(cin >> ws, heightStr);
            if (heightStr.empty()) return;
            try {
                vehHeight = stof(heightStr);
            } catch (...) {
                vehHeight = -1.0f;
            }
            if (vehHeight < 0.0f || vehHeight > maxHeight) {
                cout << "Bad entry! Height must be between 0 and " << maxHeight << " meters.\n";
                continue;
            }
            break;
        }
        while (true) {
            cout << "Enter vehicle length (up to " << maxLength << " m): ";
            getline(cin >> ws, lengthStr);
            if (lengthStr.empty()) return;
            try {
                vehLength = stof(lengthStr);
            } catch (...) {
                vehLength = -1.0f;
            }
            if (vehLength < 0.0f || vehLength > maxLength) {
                cout << "Bad entry! Length must be between 0 and " << maxLength << " meters.\n";
                continue;
            }
            break;
        }
    } else {
        // Retrieve stored dimensions of existing vehicle
        float storedLen, storedHt;
        getVehicleDimensions(vehicleFile, licensePlate, storedLen, storedHt);
        vehLength = storedLen;
        vehHeight = storedHt;
        cout << "Vehicle is known. Using stored dimensions: Height=" << vehHeight << "m, Length=" << vehLength << "m.\n";
    }
    // Prompt for customer phone number
    while (true) {
        cout << "Enter customer phone number: ";
        getline(cin >> ws, phone);
        if (phone.empty()) return;
        // Simple validation: require at least 7 digits in input
        int digitCount = 0;
        for (char c : phone) {
            if (isdigit(c)) digitCount++;
        }
        if (digitCount < 7) {
            cout << "Phone number format is incorrect (must contain at least 7 digits).\n";
            continue;
        }
        break;
    }
    // Add new vehicle to file if it didn't exist
    if (!vehicleExists) {
        Vehicle newVeh(licensePlate, vehHeight, vehLength);
        writeVehicle(vehicleFile,newVeh);
    }
    // Create new Booking and append to booking file
    Booking newBooking(licensePlate, sailingId, phone, false);
    newBooking.writeBooking(bookingFile);
    cout << "Booking created successfully for vehicle " << licensePlate 
         << " on sailing " << sailingId << ".\n";
}

// Check-in process: mark an existing booking as checked-in (if found)
void checkIn(fstream& bookingFile, fstream& vehicleFile, fstream& sailingFile) {
    string sailingID;
    bool exitFlag = false;
    while (!exitFlag) {
        // Prompt for Sailing ID
        while (true) {
            cout << "Enter a SailingID (ccc-dd-dd): ";
            getline(cin >> ws, sailingID);
            if (sailingID.empty()) {
                exitFlag = true;
                break;
            }
            if (!isValidSailingID(sailingID)) {
                cout << "SailingID format is incorrect. Try again or press Enter to exit.\n";
                continue;
            }
            if (!isSailingExist(sailingID, sailingFile)) {
                cout << "SailingID does not exist! Try again or press Enter to exit.\n";
                continue;
            }
            break;
        }
        if (exitFlag) break;
        // Prompt for vehicle license plate to check in
        string licensePlate;
        while (true) {
            cout << "Enter the vehicle's license plate (3 - 10 characters): ";
            getline(cin >> ws, licensePlate);
            if (licensePlate.empty()) {
                exitFlag = true;
                break;
            }
            if (licensePlate.length() < 3 || licensePlate.length() > 10) {
                cout << "License plate format is incorrect. Try again or press Enter to exit.\n";
                continue;
            }
            // Find matching booking in file
            Booking match;
            bookingFile.clear();
            bookingFile.seekg(0, ios::beg);
            bool bookingFound = false;
            while (bookingFile.read(reinterpret_cast<char*>(&match), sizeof(Booking))) {
                if (string(match.getSailingID()) == sailingID && string(match.getLicensePlate()) == licensePlate && !match.getCheckedIn()) {
                    bookingFound = true;
                    break;
                }
            }
            if (!bookingFound) {
                cout << "No active booking for that license plate on this sailing. Try again or press Enter to exit.\n";
                continue;
            }
            // Calculate fare using vehicle dimensions
            float vLength, vHeight;
            if (!getVehicleDimensions(vehicleFile, licensePlate, vLength, vHeight)) {
                cout << "Vehicle not found in database. Cannot calculate fare.\n";
                continue;
            }
            float fare = calculateFare(vLength, vHeight);
            cout << "The fare is " << fare << ". Press <enter> once collected.";
            string dummy;
            getline(cin, dummy);
            // Update booking status: remove old record and add new record with checkedIn=true
            deleteBookingRecord(sailingID, licensePlate);
            Booking updated(match.getLicensePlate(), match.getSailingID(), match.getPhoneNumber(), true);
            appendBookingRecord(updated);
            cout << "Booking for " << licensePlate << " on " << sailingID << " is now checked in.\n";
            break;
        }
        if (exitFlag) break;
    }
}

bool isBookingExist(const string& sailingId, const string& licensePlate, fstream& inFile) {
    // Linear search in booking file for a matching record
    inFile.clear();
    inFile.seekg(0, ios::beg);
    Booking temp;
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
        if (string(temp.getSailingID()) == sailingId && string(temp.getLicensePlate()) == licensePlate) {
            return true;
        }
    }
    return false;
}

void promptToDeleteBooking() {
    string sailingId, licensePlate;
    // Prompt for Sailing ID
    while (true) {
        cout << "Enter Sailing ID (ccc-dd-dd): ";
        getline(cin >> ws, sailingId);
        if (sailingId.empty()) return;
        if (!isValidSailingID(sailingId)) {
            cout << "Bad entry! Sailing ID must follow the format ccc-dd-dd.\n";
        } else {
            break;
        }
    }
    // Prompt for license plate
    while (true) {
        cout << "Enter license plate number (3 - 10 characters): ";
        getline(cin >> ws, licensePlate);
        if (licensePlate.empty()) return;
        if (licensePlate.length() < 3 || licensePlate.length() > 10) {
            cout << "Bad entry! License plate must be between 3 and 10 characters.\n";
        } else {
            break;
        }
    }
    // Use low-level function to delete matching booking record
    bool result = deleteBookingRecord(sailingId, licensePlate);
    cout << (result ? "Booking has been successfully deleted.\n" : "Booking not found.\n");
}

// Fare calculation for a booking (base fare + extras for oversize)
float calculateFare(const float& length, const float& height) {
    float totalExtra = 0.0;
    // Charge extra per meter for length beyond regular threshold
    if (length > maxLengthForRegularSizedVehicle) {
        int extraLengthMeters = static_cast<int>(length - maxLengthForRegularSizedVehicle);
        totalExtra += extraLengthMeters * extraPerMeterInLength;
    }
    // Charge extra per meter for height beyond regular threshold
    if (height > maxHeightForRegularSizedVehicle) {
        int extraHeightMeters = static_cast<int>(height - maxHeightForRegularSizedVehicle);
        totalExtra += extraHeightMeters * extraPerMeterInHeight;
    }
    return regularSizedVehicleFare + totalExtra;
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
