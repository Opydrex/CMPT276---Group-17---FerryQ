// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// MODULE NAME: BookingFileIO.cpp
// Rev.1 – 24/07/2025 – Implements low-level file I/O for Booking records.
//
// ----------------------------------------------------------------------------
// This module performs all low-level binary file operations for booking records.
//
// What it does:
// - Provides functions to read, write, search, and delete Booking objects
//   from the "booking.txt" binary file.
//
// Implementation Strategy:
// - Data is stored as fixed-length binary records (using sizeof(Booking)).
// - All lookups are performed using a linear search of the file.
// - Deletion is handled with a "swap-and-truncate" method to maintain a
//   compact, unordered data file.
//
// Used By: Called by the BookingUserIO.cpp module to persist booking data.
// ----------------------------------------------------------------------------

#include "BookingFileIO.h"
#include <iostream>

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

bool deleteBookingsBySailingID(fstream& bookingFile, const string& sailingID) {
    if (!bookingFile.good()) return false;

    bool deletedAtLeastOne = false;
    bool deletedThisPass;

    do {
        deletedThisPass = false;
        bookingFile.clear();
        bookingFile.seekg(0, ios::beg);

        Booking temp;
        while (bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
            if (temp.getSailingID() == sailingID) {
                // Delete this booking (license plate needed for deleteBookingRecord)
                if (deleteBookingRecord(sailingID, temp.getLicensePlate(), bookingFile)) {
                    deletedAtLeastOne = true;
                    deletedThisPass = true;
                    break; // restart loop because file changed
                }
            }
        }
    } while (deletedThisPass);

    return deletedAtLeastOne;
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

//----------------------------------------------------------------------------
int countBookingsForSailing(const string& sailingID, fstream& bookingFile) {
    //Description: Counts the number of bookings for a specific sailing.
    if (!bookingFile.good()) return 0;

    bookingFile.clear();
    bookingFile.seekg(0, ios::beg);
    Booking temp;
    int count = 0;

    while (bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
        if (temp.getSailingID() == sailingID) {
            count++;
        }
    }
    return count;
}