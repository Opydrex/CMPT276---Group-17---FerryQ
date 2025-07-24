//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================================
//==========================================================================

/*
MODULE NAME: BookingIO.cpp
Rev.1 – 24/07/2025 – Implements low-level file I/O for Booking records.
----------------------------------------------------------------------------
This module handles binary file access for Booking records. It provides
functions for writing, deleting, searching, and counting bookings using 
fixed-length binary storage. Deletion is handled by overwriting the target 
record with the last and truncating the file.
----------------------------------------------------------------------------
*/

#include "BookingIO.h"
#include <iostream>
#include <io.h> 
using namespace std;
extern "C" int truncate(const char* path, long long length);  //Needed on some systems for file truncation
static const char* BOOKING_FILENAME = "booking.txt";  //Physical file name

//----------------------------------------------------------------------------
bool writeBooking(const Booking& booking, fstream& bookingFile){
    //Description: Appends a Booking record to the end of the file.
    bookingFile.clear();
    bookingFile.seekp(0, ios::end);  //Go to end of file
    bookingFile.write(reinterpret_cast<const char*>(&booking), sizeof(Booking));
    bookingFile.flush();
    return bookingFile.good();
}

//----------------------------------------------------------------------------
bool deleteBookingRecord(const string& sailingID,
                         const string& licensePlate,
                         fstream& bookingFile){
    //Description: Deletes a Booking record by matching sailing ID and license plate.
    //             Replaces the target with the last record and truncates the file.

    if (!bookingFile.good()) return false;
    //Compute total records
    bookingFile.clear();
    bookingFile.seekg(0, ios::end);
    int total = static_cast<int>(bookingFile.tellg() / sizeof(Booking));
    if (total <= 0) return false;
    //Find target index
    bookingFile.clear();
    bookingFile.seekg(0, ios::beg);
    Booking temp;
    int targetIndex = -1;
    for (int i = 0; i < total; ++i){
        bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking));
        if (temp.getSailingID() == sailingID && temp.getLicensePlate() == licensePlate){
            targetIndex = i;
            break;
        }
    }
    if (targetIndex < 0) return false;
    int lastIndex = total - 1;
    //Overwrite if not last
    if (targetIndex != lastIndex){
        bookingFile.clear();
        bookingFile.seekg(static_cast<streampos>(lastIndex) * sizeof(Booking), ios::beg);
        Booking lastRec;
        bookingFile.read(reinterpret_cast<char*>(&lastRec), sizeof(Booking));
        bookingFile.clear();
        bookingFile.seekp(static_cast<streampos>(targetIndex) * sizeof(Booking), ios::beg);
        bookingFile.write(reinterpret_cast<const char*>(&lastRec), sizeof(Booking));
        bookingFile.flush();
    }
    //Truncate file
    bookingFile.close();
    long newSize = static_cast<long>(lastIndex) * static_cast<long>(sizeof(Booking));
    if (truncate(BOOKING_FILENAME, newSize) != 0){
        return false;
    }
    //Reopen file for further operations
    bookingFile.open(BOOKING_FILENAME, ios::in | ios::out | ios::binary);
    return bookingFile.is_open();
}

//----------------------------------------------------------------------------
bool loadBookingByKey(const string& sailingID,
                      const string& licensePlate,
                      Booking& result,
                      fstream& bookingFile){
    //Description: Loads a booking by sailing ID and license plate into result.
    //             Returns true if found.
    if (!bookingFile.good()) return false;

    bookingFile.clear();
    bookingFile.seekg(0, ios::beg);
    Booking temp;

    //Linear search for matching booking
    while (bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking))){
        if (temp.getSailingID() == sailingID && temp.getLicensePlate() == licensePlate){
            result = temp;
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------
int countBookingRecords(fstream& bookingFile){
    //Description: Returns the number of Booking records in the file.
    if (!bookingFile.good()) return 0;

    bookingFile.clear();
    bookingFile.seekg(0, ios::end);
    return static_cast<int>(bookingFile.tellg() / sizeof(Booking));
}
