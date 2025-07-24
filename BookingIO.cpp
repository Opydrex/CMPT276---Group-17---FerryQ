// BookingIO.cpp
#include "BookingIO.h"
#include <iostream>
#include <io.h>
using namespace std;
static const char* BOOKING_FILENAME = "booking.txt";

bool writeBooking(const Booking& booking, fstream& bookingFile) {
    bookingFile.clear();
    bookingFile.seekp(0, ios::end);
    bookingFile.write(reinterpret_cast<const char*>(&booking), sizeof(Booking));
    bookingFile.flush();
    return bookingFile.good();
}

bool deleteBookingRecord(const string& sailingID,
                         const string& licensePlate,
                         fstream& bookingFile) {
    if (!bookingFile.good()) return false;
    // Compute total records
    bookingFile.clear(); bookingFile.seekg(0, ios::end);
    int total = static_cast<int>(bookingFile.tellg() / sizeof(Booking));
    if (total <= 0) return false;
    // Find target index
    bookingFile.clear(); bookingFile.seekg(0, ios::beg);
    Booking temp;
    int targetIndex = -1;
    for (int i = 0; i < total; ++i) {
        bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking));
        if (temp.getSailingID() == sailingID && temp.getLicensePlate() == licensePlate) {
            targetIndex = i;
            break;
        }
    }
    if (targetIndex < 0) return false;
    int lastIndex = total - 1;
    // Overwrite if not last
    if (targetIndex != lastIndex) {
        bookingFile.clear();
        bookingFile.seekg(lastIndex * sizeof(Booking), ios::beg);
        Booking lastRec;
        bookingFile.read(reinterpret_cast<char*>(&lastRec), sizeof(Booking));
        bookingFile.clear();
        bookingFile.seekp(targetIndex * sizeof(Booking), ios::beg);
        bookingFile.write(reinterpret_cast<const char*>(&lastRec), sizeof(Booking));
        bookingFile.flush();
    }
    // Truncate file
    bookingFile.close();
#ifdef _WIN32
    FILE* f = fopen(BOOKING_FILENAME, "rb+");
    if (f) { _chsize_s(_fileno(f), lastIndex * sizeof(Booking)); fclose(f); }
#else
    truncate(BOOKING_FILENAME, lastIndex * sizeof(Booking));
#endif
    bookingFile.open(BOOKING_FILENAME, ios::binary | ios::in | ios::out);
    return bookingFile.good();
}

bool loadBookingByKey(const string& sailingID,
                      const string& licensePlate,
                      Booking& result,
                      fstream& bookingFile) {
    if (!bookingFile.good()) return false;
    bookingFile.clear(); bookingFile.seekg(0, ios::beg);
    Booking temp;
    while (bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
        if (temp.getSailingID() == sailingID && temp.getLicensePlate() == licensePlate) {
            result = temp;
            return true;
        }
    }
    return false;
}

int countBookingRecords(fstream& bookingFile) {
    if (!bookingFile.good()) return 0;
    bookingFile.clear(); bookingFile.seekg(0, ios::end);
    return static_cast<int>(bookingFile.tellg() / sizeof(Booking));
}