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

bool deleteBookingRecord(const std::string& sailingID,
                         const std::string& licensePlate,
                         std::fstream& bookingFile) {
    if (!bookingFile) return false;
    // Determine record count
    bookingFile.clear(); bookingFile.seekg(0, std::ios::end);
    std::streamoff size = bookingFile.tellg();
    int total = static_cast<int>(size / sizeof(Booking));
    if (total <= 0) return false;

    // Find target record index
    bookingFile.clear(); bookingFile.seekg(0, std::ios::beg);
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
    // Overwrite target with last record if necessary
    if (targetIndex != lastIndex) {
        bookingFile.clear();
        bookingFile.seekg(lastIndex * sizeof(Booking), std::ios::beg);
        bookingFile.read(reinterpret_cast<char*>(&temp), sizeof(Booking));
        bookingFile.clear();
        bookingFile.seekp(targetIndex * sizeof(Booking), std::ios::beg);
        bookingFile.write(reinterpret_cast<const char*>(&temp), sizeof(Booking));
        bookingFile.flush();
    }

    // Close stream before truncating
    bookingFile.close();
    // Truncate file to remove last record
    if (truncate(BOOKING_FILENAME, static_cast<long long>(lastIndex) * sizeof(Booking)) != 0) {
        std::perror("truncate");
        return false;
    }
    // Reopen for read/write
    bookingFile.open(BOOKING_FILENAME, std::ios::binary | std::ios::in | std::ios::out);
    return static_cast<bool>(bookingFile);
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
