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

#include <iostream>
#include <string>
using namespace std;

const float regularSizedVehicleFare = 14.0;
const float extraPerMeterInHeight = 2.0;
const float extraPerMeterInLength = 3.0;
const float maxHeightForRegularSizedVehicle = 2.0;
const float maxLengthForRegularSizedVehicle = 7.0;
const string fileNameBooking = "booking.txt";

class Booking {
    public:
        Booking() = default;
        Booking(const string& licensePlate,  // input
                const string& sailingId,     // input
                const string& phoneNumber,   // input
                const bool& checkedIn        // input
        );
        // Job: Booking constructor initializes a booking with given details.

        void writeBooking(fstream& outFile);
        // Job: Writes this Booking to the open booking file (binary).
        // Usage: Called when adding or updating a booking record.

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
        char sailingId[16];
        char licensePlate[16];
        char phoneNumber[16];
        bool checkedIn;
};

// Booking module functions:
void createBooking(fstream& vehicleFile, fstream& bookingFile, fstream& sailingFile);
// Job: Interactively create a new booking (asks for SailingID, vehicle info, phone) and save to file.
// Usage: Called from Bookings menu. Uses open vehicle file to add new Vehicle if needed, open booking file to save Booking, open sailing file to validate sailing existence.

void checkIn(fstream& bookingFile, fstream& vehicleFile, fstream& sailingFile);
// Job: Process vehicle check-in for a sailing (mark booking as checked in).
// Usage: Called from main menu. Verifies sailing exists and booking exists, calculates fare, then updates booking status in file (by removal and re-add).

// bool isBookingExist(const string& sailingId, const string& licensePlate, fstream& inFile);
// Job: Checks if a booking with given sailingId and licensePlate exists in the booking file.

void promptToDeleteBooking(fstream& bookingFile);
// Job: Prompt user for a booking to delete by SailingID and license plate, and remove it from file.
// Usage: Called from Bookings menu. Uses BookingIO low-level deletion function.

float calculateFare(const float& length, const float& height);
// Job: Calculates fare for a booked vehicle based on its dimensions.
// Usage: Called during vehicle check-in. Adds extra costs for oversize dimensions above regular threshold.

#endif // BOOKING_H