// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: BookingUserIO.cpp
// Rev.2 - 05/08/2025 - Updated user input logic to correctly check for blank inputs.
//                    - Functions now clear the terminal before outputting their result.
// Rev.1 - 24/07/2025 - Initial implementation of Booking class and UI functions.
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
#include "UserInterface.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <limits>
#include <regex>
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
        cout << endl << "Enter Sailing ID (ccc-dd-dd) or blank to cancel: ";
        getline(cin, sailingId);
        sailingId = trim(sailingId);
        if (sailingId.empty()) {
            system("cls");
            cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
            return;
        }
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
        getline(cin, plate);
        plate = trim(plate);
        if (plate.empty()) {
            system("cls");
            cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
            return;
        }
        if (plate.size() < 3 || plate.size() > 10){
            cout << "\nBad entry! Plate must be 3-10 characters." << endl;
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
            if (!(cin >> height) || height < 0 || height > maxHeight || cin.peek() != '\n'){
                cout << "Invalid. Try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        //Input and validate length
        while (true){
            cout << "Enter length (0 to " << maxLength << "): ";
            if (!(cin >> length) || length < 0 || length > maxLength || cin.peek() != '\n'){
                cout << "Invalid. Try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            cout << "Using stored dimensions. Height: " << height << " ; Length: " << length << endl;
        }
    }

    string phone;
    //Prompt until a valid phone number with at least 7 digits is entered
    while (true){
        cout << "Enter customer phone number (between 7 and 15 characters) or blank to cancel: ";
        getline(cin, phone);
        phone = trim(phone);

        if (phone.empty()) {
            system("cls");
            cout << "\nEnter pressed. Now aborting to the previous Menu\n";
            return;
        }
        if (!std::regex_match(phone, std::regex("^[0-9]+$"))) {
            cout << "Bad entry! Phone must contain digits only.\n";
            continue;
        }
        if (!std::regex_match(phone, std::regex("^[0-9]{7,15}$"))) {
            //at this point we know it's all digits, so it must be a length issue
            if (phone.size() < 7)
                cout << "Too few digits. Try again.\n";
            else
                cout << "Too many digits. Try again.\n";
            continue;
        }
        break;
    }

    Booking b(plate, sailingId, phone, false);
    if (!writeBooking(b, bookingFile)){
        cerr << "Error: Unable to append booking record." << endl;
    } else{
        
        // Update sailing capacity
        bool isSpecial = (height > maxHeightForRegularSizedVehicle || length > maxLengthForRegularSizedVehicle);
        if(isSpecial){
            system("cls");
            cout << "Special-sized vehicle with a \'" << plate 
            << "\' license plate has been booked for sailing "
            << sailingId << ". Would you like to create another booking? (Y/N) ";
        }
        else{
            system("cls");
            cout << "Normal-sized vehicle with a \'" << plate 
            << "\' license plate has been booked for sailing "
            << sailingId << ". Would you like to create another booking? (Y/N) ";
        }
        float regularLengthUsed = isSpecial ? 0.0f : length;
        float specialLengthUsed = isSpecial ? length : 0.0f;

        if (!updateSailingCapacities(sailingFile, sailingId, regularLengthUsed, specialLengthUsed)) {
            cerr << "Error: The vessel does not have enough space to fit this vehicle." << endl;
        }
    }
    string resp;
    getline(cin, resp);
    resp = trim(resp);
    if (!resp.empty() && (resp[0]=='Y'||resp[0]=='y'))
        createBooking(vehicleFile, bookingFile, sailingFile);
}

//----------------------------------------------------------------------------
void checkIn(fstream& bookingFile,
             fstream& vehicleFile,
             fstream& sailingFile){
//Description: Marks a booking as checked in and recalculates fare.
//             Rewrites the record with checkedIn=true.

    while (true){
        string sid, plate;
        while(true){
            cout << endl << "Enter SailingID (ccc-dd-dd) or blank to cancel: ";
            getline(cin, sid);
            sid = trim(sid);
            if (sid.empty()) {
                system("cls");
                cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
                return;
            }
            //Validate sailing ID
            if (!isValidSailingID(sid)){
                cout << "Bad entry! Sailing ID format is ccc-dd-dd." << endl;
                continue;
            }
            //Validate sailing ID existence
            if (findSailingIndexByID(sailingFile, sid) == -1){
                cout << "No Sailing with SailingID" << sid << " was found. Try again." << endl;
                continue;
            }
            break;
        }
        while (true) {
            cout << "Enter the vehicle's license plate (3 - 10 characters) or blank to cancel: ";
            getline(cin, plate);
            plate = trim(plate);

            if (plate.empty()) {
                system("cls");
                cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
                return;
            }

            // plate length check: must be between 3 and 10 chars
            if (plate.size() < 3 || plate.size() > 10) {
                cout << "Bad entry! Must be between 3 and 10 characters." << endl;
                continue;
            }

            break;
        }
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
        cout << "The fare is " << fare << ". Press <enter> once it has been collected.";
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

        system("cls");
        cout << "Checked in \'" << plate << "\' onto " << sid << endl;
    }
}

//----------------------------------------------------------------------------
void promptToDeleteBooking(fstream& bookingFile, fstream& vehicleFile, fstream& sailingFile){
//Description: Interactive prompt to delete a booking by sailing ID and license plate.
    string sid, plate;
    cout << endl << "Enter SailingID (ccc-dd-dd) or blank to cancel: ";
    getline(cin, sid);
    sid = trim(sid);
    if (sid.empty() || !isValidSailingID(sid)) return;

    cout << "Enter license plate (3-10 characters) or blank to cancel: ";
    getline(cin, plate);
    plate = trim(plate);
    if (plate.empty()) {
        system("cls");
        cout << endl << "Enter pressed. Now aborting to the previous Menu" << endl;
        return;
    }
    system("cls");

    Booking bookingToDelete;
    if (loadBookingByKey(sid, plate, bookingToDelete, bookingFile)) {
        float length, height;
        if (getVehicleDimensions(vehicleFile, plate, length, height)) {
            bool isSpecial = (height > maxHeightForRegularSizedVehicle || length > maxLengthForRegularSizedVehicle);
            float regularLengthRestored = isSpecial ? 0.0f : length;
            float specialLengthRestored = isSpecial ? length : 0.0f;

            if (deleteBookingRecord(sid, plate, bookingFile)) {
                cout << "Booking has been successfully deleted" << endl;
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

