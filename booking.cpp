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
#include "Vehicle.h"
#include "BookingIO.h"
#include "VehicleIO.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

using namespace std;

// Booking::Booking(const string& licensePlate,//Input
//                  const string& sailingId,//Input
//                  const string& phoneNumber,//Input
//                  const bool& checkedIn//Input 
// )
// {
//     this->licensePlate = licensePlate;
//     this->sailingId = sailingId;
//     this->phoneNumber = phoneNumber;
//     this->checkedIn = checkedIn;
// }

Booking::Booking(const string& licensePlate, const string& sailingId, const string& phoneNumber, const bool& checkedIn) {
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';

    strncpy(this->sailingId, sailingId.c_str(), sizeof(this->sailingId) - 1);
    this->sailingId[sizeof(this->sailingId) - 1] = '\0';

    strncpy(this->phoneNumber, phoneNumber.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0';

    this->checkedIn = checkedIn;
}

void Booking::writeBooking(ofstream& outFile) {
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(this), sizeof(Booking));
        outFile.flush();
    } else {
        cerr << "Error: Unable to open file for writing. Check file path and permissions." << endl;
    }
}

void createBooking(ifstream& inFile, // Vehicle file
                   ofstream& outFile, // Vehicle file (write)
                   ifstream& sailingInFile) // Sailing file
{
    string vehicleLength, vehicleHeight;
    float floatVehicleLength, floatVehicleHeight;
    bool exitFlag = false;

    while (true) {
        string sailingID;
        while (true) {
            cout << "Enter SailingID (ccc-dd-dd): ";
            getline(cin >> ws, sailingID);
            if (sailingID.empty()) {
                exitFlag = true;
                break;
            }

            if (sailingID.length() != 9 || sailingID[3] != '-' || sailingID[6] != '-' ||
                !isalpha(sailingID[0]) || !isalpha(sailingID[1]) || !isalpha(sailingID[2]) ||
                !isdigit(sailingID[4]) || !isdigit(sailingID[5]) ||
                !isdigit(sailingID[7]) || !isdigit(sailingID[8])) {
                cout << "SailingID format is incorrect. Try again or press Enter to exit.\n";
                continue;
            }

            if (!isSailingExist(sailingID, sailingInFile)) {
                cout << "SailingID does not exist! Try again or press Enter to exit.\n";
                continue;
            }

            break;
        }
        if (exitFlag) break;

        string licensePlate;
        while (true) {
            cout << "Enter license plate number (3 - 10 characters): ";
            getline(cin >> ws, licensePlate);
            if (licensePlate.empty()) {
                exitFlag = true;
                break;
            } else if (licensePlate.length() < 3 || licensePlate.length() > 10) {
                cout << "License plate must be 3-10 characters. Try again or press Enter to exit.\n";
                continue;
            }
            break;
        }
        if (exitFlag) break;

        string phoneNumber;
        while (true) {
            cout << "Enter phone number (8 to 14 characters): ";
            getline(cin >> ws, phoneNumber);
            if (phoneNumber.length() < 8 || phoneNumber.length() > 14) {
                cout << "Phone number must be 8–14 characters. Try again.\n";
                continue;
            }
            break;
        }

        if (isVehicleExist(licensePlate)) {
            getVehicleDimensions(licensePlate, &vehicleLength, &vehicleHeight);
        } else {
            while (true) {
                cout << "Enter vehicle length (up to 99.9 meters): ";
                getline(cin >> ws, vehicleLength);
                floatVehicleLength = stof(vehicleLength);
                if (floatVehicleLength < 0 || floatVehicleLength > 99.9) {
                    cout << "Invalid length. Must be between 0 and 99.9\n";
                    continue;
                }
                break;
            }

            while (true) {
                cout << "Enter vehicle height (up to 9.9 meters): ";
                getline(cin >> ws, vehicleHeight);
                floatVehicleHeight = stof(vehicleHeight);
                if (floatVehicleHeight < 0 || floatVehicleHeight > 9.9) {
                    cout << "Invalid height. Must be between 0 and 9.9\n";
                    continue;
                }
                break;
            }

            Vehicle newVehicle(licensePlate, floatVehicleHeight, floatVehicleLength);
            newVehicle.writeVehicle(outFile); // still OK for now
        }

        // Create the booking
        Booking newBooking(licensePlate, sailingID, phoneNumber, false);
        if (!appendBookingRecord(newBooking)) {
            cerr << "Error writing booking to file.\n";
        }

        cout << (stof(vehicleHeight) > maxHeightForRegularSizedVehicle || stof(vehicleLength) > maxLengthForRegularSizedVehicle
                 ? "Special-sized"
                 : "Normal-sized")
             << " vehicle with license plate " << licensePlate << " has been booked for Sailing " << sailingID << ".\n";

        cout << "Would you like to create another booking? (Y/N): ";
        string yesNo;
        cin >> yesNo;
        if (yesNo != "Y" && yesNo != "y") break;
    }
}

bool isBookingExist(const string& sailingID, const string& licensePlate) {
    Booking temp;
    return loadBookingByKey(sailingID, licensePlate, temp);
}

float calculateFare(const float& length, // input
                    const float& height  // input
){
    float totalExtra = 0;

    if (length > maxLengthForRegularSizedVehicle) {
        int extraLength = static_cast<int>(length - maxLengthForRegularSizedVehicle);
        totalExtra += extraLength * extraPerMeterInLength;
    }

    if (height > maxHeightForRegularSizedVehicle) {
        int extraHeight = static_cast<int>(height - maxHeightForRegularSizedVehicle);
        totalExtra += extraHeight * extraPerMeterInHeight;
    }

    return regularSizedVehicleFare + totalExtra;
}

void checkIn(ifstream& inFileVehicle, ifstream& sailingInFile) {
    string sailingID;
    bool exitFlag = false;

    while (true) {
        // Prompt for SailingID
        while (true) {
            cout << "Enter a SailingID (ccc-dd-dd): ";
            getline(cin >> ws, sailingID);

            if (sailingID.empty()) {
                exitFlag = true;
                break;
            }

            if (sailingID.length() != 9 || sailingID[3] != '-' || sailingID[6] != '-' ||
                !isalpha(sailingID[0]) || !isalpha(sailingID[1]) || !isalpha(sailingID[2]) ||
                !isdigit(sailingID[4]) || !isdigit(sailingID[5]) ||
                !isdigit(sailingID[7]) || !isdigit(sailingID[8])) {
                cout << "SailingID format is incorrect. Please try again, or press Enter to exit\n";
                continue;
            }

            if (!isSailingExist(sailingID, sailingInFile)) {
                cout << "SailingID does not exist! Please try again, or press Enter to exit\n";
                continue;
            }

            break;
        }

        if (exitFlag) break;

        string licensePlate;
        while (true) {
            cout << "Enter the vehicle's license plate (3 - 10 characters): ";
            getline(cin >> ws, licensePlate);

            if (licensePlate.empty()) {
                exitFlag = true;
                break;
            }

            if (licensePlate.length() < 3 || licensePlate.length() > 10) {
                cout << "License plate format is incorrect. Please try again, or press Enter to exit\n";
                continue;
            }

            Booking match;
            if (!loadBookingByKey(sailingID, licensePlate, match)) {
                cout << "License plate does not have an associated booking. Try another or press Enter to exit.\n";
                continue;
            }

            // Calculate fare
            string len, height;
            getVehicleDimensions(licensePlate, &len, &height); 
            float vLength = stof(len);
            float vHeight = stof(height);
            cout << "The fare is " << calculateFare(vLength, vHeight)
                 << ". Press <enter> once it has been collected. ";
            string dummy;
            getline(cin, dummy);

            // Update booking: delete old and write new with checkedIn = true
            deleteBookingRecord(sailingID, licensePlate);

            Booking updated(
                match.getLicensePlate(),
                match.getSailingID(),
                match.getPhoneNumber(),
                true
            );

            appendBookingRecord(updated);
            break;
        }

        if (exitFlag) break;
    }
}


void promptToDeleteBooking() {
    string sailingId, licensePlate;

    // Prompt for sailing ID
    while (true) {
        cout << "Enter Sailing ID (ccc-dd-dd): ";
        getline(cin >> ws, sailingId);
        if (sailingId.empty()) return;

        if (sailingId.length() != 9 || sailingId[3] != '-' || sailingId[6] != '-' ||
            !isalpha(sailingId[0]) || !isalpha(sailingId[1]) || !isalpha(sailingId[2]) ||
            !isdigit(sailingId[4]) || !isdigit(sailingId[5]) ||
            !isdigit(sailingId[7]) || !isdigit(sailingId[8])) {
            cout << "Bad entry! Sailing ID must follow the format ccc-dd-dd.\n";
        } else {
            break;
        }
    }

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

    // Internal file I/O and deletion
    ifstream in(fileNameBooking, ios::binary);
    ofstream out("booking_tmp.dat", ios::binary | ios::trunc);
    if (!in || !out) {
        cerr << "Error opening booking file.\n";
        return;
    }

    Booking temp;
    bool found = false;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
        if (temp.getSailingID() == sailingId && temp.getLicensePlate() == licensePlate) {
            found = true;
            continue;
        }
        out.write(reinterpret_cast<const char*>(&temp), sizeof(Booking));
    }

    in.close();
    out.close();
    remove(fileNameBooking.c_str());
    rename("booking_tmp.dat", fileNameBooking.c_str());

    cout << (found ? "Booking has been successfully deleted.\n"
                   : "Booking not found.\n");
}

// Setters
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

// Getters
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
