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
                   ofstream& outFileBooking, // Booking file
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

        if (isVehicleExist(licensePlate, inFile)) {
            getVehicleDimensions(licensePlate, &vehicleLength, &vehicleHeight, inFile);
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
            newVehicle.writeVehicle(outFile);
        }

        Booking newBooking(licensePlate, sailingID, phoneNumber, false);
        newBooking.writeBooking(outFileBooking);

        cout << (floatVehicleHeight > maxHeightForRegularSizedVehicle || floatVehicleLength > maxLengthForRegularSizedVehicle
                 ? "Special-sized"
                 : "Normal-sized")
             << " vehicle with license plate " << licensePlate << " has been booked for Sailing " << sailingID << ".\n";

        cout << "Would you like to create another booking? (Y/N): ";
        string yesNo;
        cin >> yesNo;
        if (yesNo != "Y" && yesNo != "y") break;
    }
}

bool isBookingExist(const string& sailingId,      // input
                    const string& licensePlate,   // input
                    ifstream& inFile              // input; the booking's file
) {
    inFile.clear();
    inFile.seekg(0, ios::beg);

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string sailingIDFromFile, licensePlateFromFile;

        if (getline(ss, sailingIDFromFile, ',') &&
            getline(ss, licensePlateFromFile, ',')) {

            if (sailingIDFromFile == sailingId &&
                licensePlateFromFile == licensePlate) {
                return true;
            }
        }
    }
    return false;
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

void checkIn(ifstream& inFile,             // Booking file (binary, read mode)
             ifstream& inFileVehicle,      // Vehicle file (text mode, used as-is)
             ofstream& outFile,            // Booking file (binary, append mode)
             ifstream& sailingInFile       // Sailing file (text mode, used as-is)
) {
    inFile.clear();
    inFile.seekg(0, ios::beg);

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
                cout << "licensePlate format is incorrect. Please try again, or press Enter to exit\n";
                continue;
            }

            if (!isBookingExist(sailingID, licensePlate, inFile)) {
                cout << "licensePlate does not have an associated booking. Please enter a different license plate, or press Enter to exit\n";
                continue;
            }

            break;
        }

        if (exitFlag) break;

        // Locate the booking record in binary mode
        inFile.clear();
        inFile.seekg(0, ios::beg);

        Booking matchedBooking;
        bool found = false;
        while (inFile.read(reinterpret_cast<char*>(&matchedBooking), sizeof(Booking))) {
            if (sailingID == matchedBooking.getSailingID() &&
                licensePlate == matchedBooking.getLicensePlate()) {
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "No booking found for this license plate " << licensePlate
                 << " and sailing " << sailingID << ".\n";
            continue;
        }

        // Calculate fare
        string len, height;
        getVehicleDimensions(licensePlate, &len, &height, inFileVehicle);
        float vLength = stof(len);
        float vHeight = stof(height);
        cout << "The fare is " << calculateFare(vLength, vHeight)
             << ". Press <enter> once it has been collected. ";
        string dummy;
        getline(cin, dummy);

        // Update booking: delete old and write new with checkedIn = true
        deleteBooking(sailingID, licensePlate, inFile, outFile);

        Booking updatedBooking(
            matchedBooking.getLicensePlate(),
            matchedBooking.getSailingID(),
            matchedBooking.getPhoneNumber(),
            true
        );

        updatedBooking.writeBooking(outFile);
    }
}


bool deleteBooking(const string& licensePlate,
                   const string& sailingId,
                   ifstream& inFile,
                   ofstream& outFile)
{
    inFile.clear();
    inFile.seekg(0, ios::beg);

    bool found = false;
    Booking record;

    while (inFile.read(reinterpret_cast<char*>(&record), sizeof(Booking))) {
        if (record.getLicensePlate() == licensePlate && record.getSailingID() == sailingId) {
            found = true;
            continue; // Skip writing this booking
        }
        outFile.write(reinterpret_cast<const char*>(&record), sizeof(Booking));
    }

    return found;
}

// WARNING FOR WHOEVER'S IMPLEMENTING THIS: make sure you check line 327 to see how I want to use it in checkIn. keep in mind, for booking,
//  you don't need to prompt the user for whether he wants to delete another booking or not!

void promptToDeleteBooking(ifstream &inFile, ofstream &outFile)
{
    string sailingId;
    string licensePlate;

    // Prompt for sailing ID
    while (true)
    {
        cout << "Enter Sailing ID (ccc-dd-dd): ";
        getline(cin >> ws, sailingId);

        if (sailingId.empty())
            return;

        if (sailingId.length() != 9 || sailingId[3] != '-' || sailingId[6] != '-' ||
            !isalpha(sailingId[0]) || !isalpha(sailingId[1]) || !isalpha(sailingId[2]) ||
            !isdigit(sailingId[4]) || !isdigit(sailingId[5]) ||
            !isdigit(sailingId[7]) || !isdigit(sailingId[8]))
        {
            cout << "Bad entry! Sailing ID must follow the format ccc-dd-dd.\n";
        }
        else
        {
            break;
        }
    }

    while (true) {
        cout << "Enter license plate number (3 - 10 characters): ";
        getline(cin >> ws, licensePlate);

        if (licensePlate.empty())
            return;

        if (licensePlate.length() < 3 || licensePlate.length() > 10)
        {
            cout << "Bad entry! License plate must be between 3 and 10 characters.\n";
        }
        else
        {
            break;
        }
    }

    // Call deleteBooking
    if (deleteBooking(licensePlate, sailingId, inFile, outFile))
    {
        cout << "Booking has been successfully deleted.\n";
    } else {
        cout << "Booking not found.\n";
    }
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
