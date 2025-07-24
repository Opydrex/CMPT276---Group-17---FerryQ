//==========================================================================
// BookingIO.h - Header for Booking file I/O operations
//==================================================================
#ifndef BOOKING_IO_H
#define BOOKING_IO_H

#include "Booking.h"
#include <fstream>
#include <string>
using namespace std;

const string fileNameBooking = "booking.dat";

// Appends a booking to the file
bool appendBookingRecord(const Booking& booking);

// Deletes a booking by SailingID and License Plate
bool deleteBookingRecord(const string& sailingID, const string& licensePlate);

// Loads a booking by SailingID and License Plate
bool loadBookingByKey(const string& sailingID, const string& licensePlate, Booking& result);

// Rewrites all bookings except the one to delete (used for check-in overwrite)
bool overwriteBookingsExcluding(const string& sailingID, const string& licensePlate);

// Counts total bookings
int countBookingRecords();

#endif
