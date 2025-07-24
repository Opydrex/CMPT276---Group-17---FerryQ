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

// Low-level Booking file operations:
bool appendBookingRecord(const Booking& booking);
bool deleteBookingRecord(const string& sailingID, const string& licensePlate);
bool loadBookingByKey(const string& sailingID, const string& licensePlate, Booking& result);
bool overwriteBookingsExcluding(const string& sailingID, const string& licensePlate);
int countBookingRecords();

#endif // BOOKING_IO_H
