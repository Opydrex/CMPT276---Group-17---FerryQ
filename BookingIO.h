// Updated BookingIO.h
#ifndef BOOKING_IO_H
#define BOOKING_IO_H

#include "Booking.h"
#include <fstream>
#include <string>
#include <unistd.h> // POSIX truncate
#ifdef _WIN32
#include <io.h>
#endif
using namespace std;


// Appends a booking to the file
bool writeBooking(const Booking&, std::fstream&);

// Deletes a booking by SailingID and License Plate
bool deleteBookingRecord(const string&, const string&, std::fstream&);

// Loads a booking by SailingID and License Plate
bool loadBookingByKey(const string&, const string&, Booking&, std::fstream&);

// Rewrites all bookings except the one to delete (used for check-in overwrite)
// bool overwriteBookingsExcluding(const string& sailingID,
//                                 const string& licensePlate,
//                                 fstream& bookingFile);

// Counts total bookings in file
int countBookingRecords(fstream& bookingFile);

#endif // BOOKING_IO_H
