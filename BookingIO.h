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

// Low-level binary file I/O for bookings, using already-opened fstream

// Appends a booking to the file
bool appendBookingRecord(const Booking& booking, fstream& bookingFile);

// Deletes a booking by SailingID and License Plate
bool deleteBookingRecord(const string& sailingID,
                         const string& licensePlate,
                         fstream& bookingFile);

// Loads a booking by SailingID and License Plate
bool loadBookingByKey(const string& sailingID,
                      const string& licensePlate,
                      Booking& result,
                      fstream& bookingFile);

// Rewrites all bookings except the one to delete (used for check-in overwrite)
// bool overwriteBookingsExcluding(const string& sailingID,
//                                 const string& licensePlate,
//                                 fstream& bookingFile);

// Counts total bookings in file
int countBookingRecords(fstream& bookingFile);

#endif // BOOKING_IO_H
