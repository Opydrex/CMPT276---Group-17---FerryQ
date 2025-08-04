// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: BookingUserIO.cpp
// Rev.1 – 24/07/2025 – Initial implementation of Booking class and UI functions.
//
// ----------------------------------------------------------------------------
// This module handles all high-level, interactive logic for managing bookings.
//
// What it does:
// - Implements the user-facing workflows for creating a new booking, deleting
//   an existing booking, and processing a vehicle check-in.
// - Contains all console prompts and data validation for booking details.
// - Orchestrates calls to low-level FileIO modules to verify data (e.g.,
//   checking if a sailing exists) and persist records.
//
// Used By: Called by UserInterface.cpp from the "Bookings" and "Check-in" menus.
// ----------------------------------------------------------------------------

#include "BookingUserIO.h"
#include "VehicleFileIO.h"
#include "SailingUserIO.h"
#include "BookingFileIO.h"
#include "SailingFileIO.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

//----------------------------------------------------------------------------
Booking::Booking(const string& licensePlate,
                 const string& sailingId,
                 const string& phoneNumber,
                 const bool& checkedIn){
    //Description: This is a constructor that creates a Booking object from raw input values.
    strncpy(this->licensePlate, licensePlate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0';

    strncpy(this->sailingId, sailingId.c_str(), sizeof(this->sailingId) - 1);
    this->sailingId[sizeof(this->sailingId) - 1] = '\0';

    strncpy(this->phoneNumber, phoneNumber.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0';

    this->checkedIn = checkedIn;
}

//----------------------------------------------------------------------------
void createBooking(fstream& vehicleFile,
                   fstream& bookingFile,
                   fstream& sailingFile){
    //Description: Prompts the user to create a booking and adds it to the file.
    //             Validates sailing ID, vehicle record, and phone number format.
    string sailingId;
    //Loop until a valid sailing ID is entered or cancelled
    while (true){
        cout << "Enter Sailing ID (ccc-dd-dd) or blank to cancel: ";
        getline(cin >> ws, sailingId);
        if (sailingId.empty()) return;
        if (findSailingIndexByID(sailingFile, sailingId) == -1){
            cout << "Bad entry! Sailing ID format is ccc-dd-dd." << endl;
            continue;
        }
        if (findSailingIndexByID(sailingFile, sailingId) == -1){
            cout << "Error: Sailing not found." << endl;
            continue;
        }
        break;
    }

    string plate;
    //Loop until a valid and unique license plate is entered
    while (true){
        cout << "Enter license plate (3-10 chars) or blank to cancel: ";
        getline(cin >> ws, plate);
        if (plate.empty()) return;
        if (plate.size() < 3 || plate.size() > 10){
            cout << "Bad entry! Plate must be 3-10 chars." << endl;
            continue;
        }
        Booking dummy;
        if (loadBookingByKey(sailingId, plate, dummy, bookingFile)){
            cout << "Error: Booking already exists for that vehicle on this sailing." << endl;
            continue;
        }
        break;
    }

    float height, length;

    //If vehicle is new, ask for its dimensions and save
    if (!isVehicleExist(vehicleFile, plate)){
        //Input and validate height
        while (true){
            cout << "Enter height (0 to " << maxHeight << "): ";
            if (!(cin >> height) || height < 0 || height > maxHeight){
                cout << "Invalid. Try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        //Input and validate length
        while (true){
            cout << "Enter length (0 to " << maxLength << "): ";
            if (!(cin >> length) || length < 0 || length > maxLength){
                cout << "Invalid. Try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        Vehicle v(plate, height, length);
        if (!writeVehicle(vehicleFile, v)){
            cerr << "Error: Unable to append vehicle record." << endl;
        }
    } else{
        //Retrieve vehicle dimensions from file
        if (!getVehicleDimensions(vehicleFile, plate, length, height)){
            cerr << "Error: Failed to read vehicle dimensions." << endl;
        } else{
            cout << "Using stored dims—H=" << height << " L=" << length << endl;
        }
    }

    string phone;
    //Prompt until a valid phone number with at least 7 digits is entered
    while (true){
        cout << "Enter customer phone (at least 7 digits): ";
        getline(cin >> ws, phone);
        int cnt = 0;
        for (char c : phone) if (isdigit(c)) cnt++;
        if (cnt < 7){
            cout << "Too few digits. Try again." << endl;
            continue;
        }
        break;
    }

    Booking b(plate, sailingId, phone, false);
    if (!writeBooking(b, bookingFile)){
        cerr << "Error: Unable to append booking record." << endl;
    } else{
        cout << "Booking created for " << plate << " on " << sailingId << endl;
        // Update sailing capacity
        bool isSpecial = (height > maxHeightForRegularSizedVehicle || length > maxLengthForRegularSizedVehicle);
        float regularLengthUsed = isSpecial ? 0.0f : length;
        float specialLengthUsed = isSpecial ? length : 0.0f;

        if (!updateSailingCapacities(sailingFile, sailingId, regularLengthUsed, specialLengthUsed)) {
            cerr << "Error: Failed to update sailing capacity." << endl;
        }
    }
}

//----------------------------------------------------------------------------
void checkIn(fstream& bookingFile,
             fstream& vehicleFile,
             fstream& sailingFile){
//Description: Marks a booking as checked in and recalculates fare.
//             Rewrites the record with checkedIn=true.

    while (true){
        string sid, plate;
        cout << "Enter SailingID (ccc-dd-dd) or blank to exit: ";
        getline(cin >> ws, sid);
        if (sid.empty()) return;

        //Validate sailing ID and existence
        if (!isValidSailingID(sid) || findSailingIndexByID(sailingFile, sid) == -1){
            cout << "Invalid or missing sailing. Try again." << endl;
            continue;
        }

        cout << "Enter license plate (3-10 chars) or blank to exit: ";
        getline(cin >> ws, plate);
        if (plate.empty()) return;

        Booking found;
        if (!loadBookingByKey(sid, plate, found, bookingFile)){
            cout << "Booking not found." << endl;
            continue;
        }

        if (found.getCheckedIn()){
            cout << "Already checked in." << endl;
            continue;
        }

        //Load vehicle dimensions and calculate fare
        float L, H;
        if (!getVehicleDimensions(vehicleFile, plate, L, H)){
            cout << "Error: Vehicle dimensions not found." << endl;
            continue;
        }

        float fare = calculateFare(L, H);
        cout << "Fare is " << fare << ". Press Enter once collected.";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //Replace record with updated (checked-in) version
        if (!deleteBookingRecord(sid, plate, bookingFile)){
            cerr << "Error: Unable to delete old booking record." << endl;
            return;
        }

        Booking updated(plate, sid, found.getPhoneNumber(), true);
        if (!writeBooking(updated, bookingFile)){
            cerr << "Error: Unable to append updated booking record." << endl;
        }

        cout << "Checked in " << plate << " on " << sid << endl;
        return;
    }
}

//----------------------------------------------------------------------------
void promptToDeleteBooking(fstream& bookingFile, fstream& vehicleFile, fstream& sailingFile){
//Description: Interactive prompt to delete a booking by sailing ID and license plate.
    string sid, plate;
    cout << "Enter SailingID (ccc-dd-dd): ";
    getline(cin >> ws, sid);
    if (sid.empty() || !isValidSailingID(sid)) return;

    cout << "Enter license plate (3-10 chars): ";
    getline(cin >> ws, plate);
    if (plate.empty()) return;

    Booking bookingToDelete;
    if (loadBookingByKey(sid, plate, bookingToDelete, bookingFile)) {
        float length, height;
        if (getVehicleDimensions(vehicleFile, plate, length, height)) {
            bool isSpecial = (height > maxHeightForRegularSizedVehicle || length > maxLengthForRegularSizedVehicle);
            float regularLengthRestored = isSpecial ? 0.0f : length;
            float specialLengthRestored = isSpecial ? length : 0.0f;

            if (deleteBookingRecord(sid, plate, bookingFile)) {
                cout << "Booking deleted" << endl;
                // Restore sailing capacity
                if (!updateSailingCapacities(sailingFile, sid, -regularLengthRestored, -specialLengthRestored)) { // Note the negative sign to add back
                    cerr << "Error: Failed to restore sailing capacity." << endl;
                }
            } else {
                 cout << "Error deleting booking." << endl;
            }
        } else {
            cout << "Could not find vehicle to restore capacity." << endl;
        }
    } else {
        cout << "Booking not found" << endl;
    }
}

//----------------------------------------------------------------------------
float calculateFare(const float& length, const float& height){
//Description: Calculates the fare based on vehicle dimensions. Adds a 
//             surcharge if length or height exceed allowed limits.
    float extra = 0;
    if (length > maxLengthForRegularSizedVehicle)
        extra += static_cast<int>(length - maxLengthForRegularSizedVehicle) * extraPerMeterInLength;
    if (height > maxHeightForRegularSizedVehicle)
        extra += static_cast<int>(height - maxHeightForRegularSizedVehicle) * extraPerMeterInHeight;
    return regularSizedVehicleFare + extra;
}

//----------------------------------------------------------------------------
void Booking::setSailingID(const string& id){
//Description: Sets the SailingID (e.g., "YVR-08-10") for this booking.
    strncpy(this->sailingId, id.c_str(), sizeof(this->sailingId) - 1);

    this->sailingId[sizeof(this->sailingId) - 1] = '\0'; //Force null termination
}

//----------------------------------------------------------------------------
void Booking::setLicensePlate(const string& plate){
//Description: Sets the license plate for the booking.
    strncpy(this->licensePlate, plate.c_str(), sizeof(this->licensePlate) - 1);
    this->licensePlate[sizeof(this->licensePlate) - 1] = '\0'; //Force null termination
}

//----------------------------------------------------------------------------
void Booking::setPhoneNumber(const string& phone){
//Description: Sets the phone number associated with the booking.
    strncpy(this->phoneNumber, phone.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0'; //Force null termination
}

//----------------------------------------------------------------------------
void Booking::setCheckedIn(bool status){
//Description: Marks whether the booking is checked in.
    this->checkedIn = status;
}

//----------------------------------------------------------------------------
string Booking::getSailingID() const{ 
//Description: Returns the SailingID for this booking.
    return sailingId; 
}

//----------------------------------------------------------------------------
string Booking::getLicensePlate() const{ 
//Description: Returns the license plate for this booking.
    return licensePlate; 
}

//----------------------------------------------------------------------------
string Booking::getPhoneNumber() const{ 
//Description: Returns the phone number associated with the booking.
    return phoneNumber; 
}

//----------------------------------------------------------------------------
bool Booking::getCheckedIn() const{ 
//Description: Returns whether the booking has been checked in.
    return checkedIn; 
}

//----------------------------------------------------------------------------

