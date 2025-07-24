//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: BookingIO.h
Rev.1 – 24/07/2025 – Interface for low-level Booking file I/O operations.
----------------------------------------------------------------------------
This header defines functions for writing, reading, searching, deleting,
and counting Booking records in a binary file. Each record is fixed-length.
All operations assume the file stream is already opened by the caller.
----------------------------------------------------------------------------
*/

#ifndef BOOKING_IO_H
#define BOOKING_IO_H

#include "Booking.h"
#include <fstream>
#include <string>
using namespace std;

//----------------------------------------------------------------------------
//Appends a booking to the end of the open binary file.
//Usage: Called when a new booking is created.
//Restrictions: File must be opened for binary write.
bool writeBooking(const Booking&, fstream&);

//----------------------------------------------------------------------------
//Deletes a booking by matching SailingID and License Plate.
//Usage: Called by check-in or booking cancellation workflows.
//Restrictions: File must be opened for read/write. Replaces target with last.
bool deleteBookingRecord(const string&, const string&, fstream&);

//----------------------------------------------------------------------------
//Loads a specific booking into memory by matching SailingID and License Plate.
//Usage: Used to check if a booking exists or to update it.
//Restrictions: File must be open. Returns false if not found.
bool loadBookingByKey(const string&, const string&, Booking&, fstream&);

//----------------------------------------------------------------------------
//Returns the total number of booking records in the file.
//Usage: Used for reporting or iteration.
//Restrictions: File must be open.
int countBookingRecords(fstream& bookingFile);

#endif //BOOKING_IO_H