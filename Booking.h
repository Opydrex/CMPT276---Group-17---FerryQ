//==========================================================================
//==========================================================================
/*
MODULE NAME: Booking.h
Rev.1 - 09/07/2025 - Booking class header created
----------------------------------------------------------------------------
This module contains functions and implementations related to Bookings.
----------------------------------------------------------------------------
*/


#ifndef BOOKING_H
#define BOOKING_H


//#include "Vehicle.h"
#include <iostream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------

const float regularSizedVehicleFare = 14.0; //constant stating regular sized vehicle fare
const float extraPerMeterInHeight = 2.0; //constant stating how much to charge per every extra meter in height
const float extraPerMeterInLength = 3.0; //constant stating how much to charge per every extra meter in length
const float maxHeightForRegularSizedVehicle = 2.0; //constant stating max height for a regular sized vehicle
const float maxLengthForRegularSizedVehicle = 7.0; //constat stating max length for a regular sized vehicle
const string fileNameBooking = "booking.txt";//constant that stores file name with all bookings

class Booking {
    public:
        // Default constructor
        Booking() = default;

        // Parameterized constructor
        Booking(const string& licensePlate,  // input
                const string& sailingId,     // input
                const string& phoneNumber,   // input
                const bool& checkedIn        // input
        );

        // Write booking to binary file
        void writeBooking(ofstream& outFile);

        // Setters
        void setSailingID(const string& id);
        void setLicensePlate(const string& plate);
        void setPhoneNumber(const string& phone);
        void setCheckedIn(bool status);

        // Getters
        string getSailingID() const;
        string getLicensePlate() const;
        string getPhoneNumber() const;
        bool getCheckedIn() const;

    private:
        char sailingId[16];      // "ccc-dd-hh" + null terminator
        char licensePlate[16];   // up to 15 characters + null terminator
        char phoneNumber[16];    // up to 15 characters + null terminator
        bool checkedIn;          // flag for check-in
};


//----------------------------------------------------------------------------

void createBooking(ifstream& inFile, //Input; The Vehicle's file. Needed for getVehicleDimensions
                      ofstream& outFile, //Input; The Vehicle's file. Needed for writeVehicle
                      ofstream& outFileBooking, //Input; The Booking's file. Needed to write the booking to file
                      ifstream& sailingInFile
);
// Job: Prompts the user for a sailingID, license plate and a phone number and returns a booking object that contains them
// Usage: Called by the UI to collect booking information from the user. The collected data can then be passed to writeBooking to save it to file.
// Restrictions: User input must conform to each variable's domain

//----------------------------------------------------------------------------

void checkIn(ifstream& inFile,//Input; The booking's file
             ifstream& inFileVehicle,//Input; the Vehicle's file
             ofstream& outFile,//Input; The booking's file
             ifstream& sailingInFile
);
//Job: Checks in one or multiple vehicle for a Sailing.
//Usage: Used when a booked vehicle needs to be checked in upon arrival to the port.
//Restrictions: Vehicle must be booked for this Sailing to be checked in. Sailing must exist. inFile and outFile must be open before calling this function

//----------------------------------------------------------------------------

bool isBookingExist(const string& sailingId,//input
                 const string& licensePlate,//input
                 ifstream& inFile//Input
                 );
//Job: Returns a boolean flag indicating if the vehicle is booked in.
//Usage: Call when need to find out if the vehicle is booked. Simply a look-up for a booking.
//Restrictions: Sailing must exist.

//----------------------------------------------------------------------------

bool deleteBooking(const string& licensePlate,
                   const string& sailingId,//input
                   ifstream& inFile, // input
                   ofstream& outFile //input
                   );
//Job: Deletes a booking from the file.
//Usage: Call when need to delete a booking.
//Restrictions: Booking must exist. Sailing must exist.

//----------------------------------------------------------------------------


void promptToDeleteBooking(ifstream& inFile, ofstream& outFile);
//Job: Prompts user to understand which booking to delete. it later calls deleteBooking
//Usage: Call when user chooses an option to Delete Booking
//Restrictions: Booking must exist. Sailing must exist.

//----------------------------------------------------------------------------

float calculateFare(const float& length,//input
					const float& height//input
					);

//Job: Calculates fare for a booked vehicle.
//Usage: Called when checking the vehicle in. Only call if special-sized vehicle. For normal sized - fixed fare.
//Restrictions: height and length have to be non-zero.

//----------------------------------------------------------------------------

#endif // BOOKING_H