//==========================================================================
//==========================================================================

/*
MODULE NAME: BookingFileIO.h
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
bool writeBooking(const Booking& booking, fstream& bookingFile);
//Job: Appends a Booking record to the end of the binary booking file.
//Usage: Called when a new booking is created.
//Restrictions: File must be opened in binary write mode.

//----------------------------------------------------------------------------
bool deleteBookingRecord(const string& sailingID, const string& licensePlate, fstream& bookingFile);
//Job: Deletes a Booking matching the given SailingID and License Plate by replacing it with the last record and truncating the file.
//Usage: Called by check-in or booking cancellation workflows.
//Restrictions: File must be opened in binary read/write mode.

//----------------------------------------------------------------------------
bool loadBookingByKey(const string& sailingID, const string& licensePlate, Booking& result, fstream& bookingFile);
//Job: Searches for and loads a Booking record by SailingID and License Plate.
//Usage: Used to check if a booking exists or to retrieve its data.
//Restrictions: File must be opened in binary read mode. Returns false if not found.

//----------------------------------------------------------------------------
int countBookingRecords(fstream& bookingFile);
//Job: Returns the total number of Booking records in the binary file.
//Usage: Used for iteration, reporting, or validation.
//Restrictions: File must be opened in binary mode.


#endif //BOOKING_IO_H