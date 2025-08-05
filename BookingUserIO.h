// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: BookingUserIO.h
// Rev.2 - 24/07/2025 - Changed the module name from 'Booking' to current.
//                    - Modified function declarations to match implementation
// Rev.1 - 09/07/2025 - BookingUserIO module header created
//
// ----------------------------------------------------------------------------
// This module provides the high-level interface for all user-facing booking
// operations. It defines the Booking class structure for fixed-length binary
// I/O and declares the functions that orchestrate the workflows for creating,
// deleting, and checking-in bookings.
//
// These functions are called from the user interface and coordinate actions
// between multiple low-level FileIO modules.
// ----------------------------------------------------------------------------

#ifndef BOOKING_H
#define BOOKING_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//Constants used in fare calculations and validation
const float regularSizedVehicleFare = 14.0;
const float extraPerMeterInHeight = 2.0;
const float extraPerMeterInLength = 3.0;
const float maxHeightForRegularSizedVehicle = 2.0;
const float maxLengthForRegularSizedVehicle = 7.0;
const string fileNameBooking = "booking.txt";

//Booking record class (fixed-length for binary I/O)
class Booking{
public:
    Booking() = default;
    Booking(const string& licensePlate,  //input
            const string& sailingId,     //input
            const string& phoneNumber,   //input
            const bool& checkedIn        //input
    );
    //Job: Constructor to initialize booking details (sailing, vehicle, contact, status).
    //Usage: Called when creating a new Booking object.
    //Restrictions: Fields must be within their character limits.

//----------------------------------------------------------------------------
    void setSailingID(const string& id);
    //Job: Sets the SailingID for the booking.
    //Usage: Called when creating or editing a booking.
    //Restrictions: ID must be a valid formatted string (e.g., "YVR-08-10").

//----------------------------------------------------------------------------
    void setLicensePlate(const string& plate);
    //Job: Sets the license plate for the booking.
    //Usage: Called during booking creation or modification.
    //Restrictions: Should match expected license plate format (no commas, length limit).

//----------------------------------------------------------------------------
    void setPhoneNumber(const string& phone);
    //Job: Sets the phone number associated with the booking.
    //Usage: Used during booking creation or contact info update.
    //Restrictions: String must be numeric and reasonably sized.

//----------------------------------------------------------------------------
    void setCheckedIn(bool status);
    //Job: Marks whether the booking is checked in.
    //Usage: Called during check-in process.
    //Restrictions: Accepts true (checked in) or false (not checked in).

//----------------------------------------------------------------------------
    string getSailingID() const;
    //Job: Retrieves the SailingID of the booking.
    //Usage: Used for searching, validation, or report output.
    //Restrictions: None.

//----------------------------------------------------------------------------
    string getLicensePlate() const;
    //Job: Retrieves the license plate from the booking.
    //Usage: Used in search, deletion, or report generation.
    //Restrictions: None.

//----------------------------------------------------------------------------
    string getPhoneNumber() const;
    //Job: Retrieves the contact phone number from the booking.
    //Usage: Used when contacting customers or displaying booking info.
    //Restrictions: None.

//----------------------------------------------------------------------------
    bool getCheckedIn() const;
    //Job: Returns the check-in status of the booking.
    //Usage: Used in validation, reporting, or boarding checks.
    //Restrictions: None.
//----------------------------------------------------------------------------
private:
    char sailingId[16];      //Format: ccc-dd-dd (max 15 chars + null)
    char licensePlate[16];   //Max 15 characters
    char phoneNumber[16];    //Max 15 characters
    bool checkedIn;          //Check-in status
};

//----------------------------------------------------------------------------
void createBooking(fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile);
//Job: Interactively collects booking details and appends a record to the file.
//Usage: Called from Bookings menu.
//Restrictions: All three files (vehicle, booking, sailing) must be open and valid. 
//              Ensures SailingID is valid, vehicle exists or is created, and phone is collected.

//----------------------------------------------------------------------------
void checkIn(fstream& bookingFile, fstream& vehicleFile, fstream& sailingFile);
//Job: Marks a booking as checked-in and prints the calculated fare.
//Usage: Called from main menu. Searches booking and vehicle records,  
//       confirms dimensions, calculates fare, and rewrites booking with status.
//Restrictions: All files must be open. Assumes booking exists and vehicle info is valid.

//----------------------------------------------------------------------------
void promptToDeleteBooking(fstream& bookingFile, fstream& vehicleFile, fstream& sailingFile);
//Job: Prompts user for a booking and deletes it from the file if found.
//Usage: Called from Bookings menu.
//Restrictions: Booking file must be open for read/write.

//----------------------------------------------------------------------------
float calculateFare(const float& length, const float& height);
//Job: Computes fare based on regular base and oversize surcharges.
//Usage: Used during check-in to calculate price before confirmation.
//Restrictions: Length and height must be positive floats. 
//              Applies surcharge if either dimension exceeds regular limits.

#endif //BOOKING_H
