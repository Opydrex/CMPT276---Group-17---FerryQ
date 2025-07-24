//==========================================================================
// BookingIO.cpp - Implementation of Booking file I/O operations
//==========================================================================
#include "BookingIO.h"
#include <fstream>
#include <iostream>
using namespace std;

bool appendBookingRecord(const Booking& booking) {
    ofstream out(fileNameBooking, ios::binary | ios::app);
    if (!out) return false;

    out.write(reinterpret_cast<const char*>(&booking), sizeof(Booking));
    return true;
}

bool deleteBookingRecord(const string& sailingID, const string& licensePlate) {
    ifstream in(fileNameBooking, ios::binary);
    ofstream out("booking_tmp.dat", ios::binary | ios::trunc);
    if (!in || !out) return false;

    Booking temp;
    bool found = false;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
        if (temp.getSailingID() == sailingID && temp.getLicensePlate() == licensePlate) {
            found = true;
            continue;
        }
        out.write(reinterpret_cast<const char*>(&temp), sizeof(Booking));
    }

    in.close();
    out.close();
    remove(fileNameBooking.c_str());
    rename("booking_tmp.dat", fileNameBooking.c_str());

    return found;
}

bool loadBookingByKey(const string& sailingID, const string& licensePlate, Booking& result) {
    ifstream in(fileNameBooking, ios::binary);
    if (!in) return false;

    Booking temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
        if (temp.getSailingID() == sailingID && temp.getLicensePlate() == licensePlate) {
            result = temp;
            return true;
        }
    }
    return false;
}

bool overwriteBookingsExcluding(const string& sailingID, const string& licensePlate) {
    ifstream in(fileNameBooking, ios::binary);
    ofstream out("booking_tmp.dat", ios::binary | ios::trunc);
    if (!in || !out) return false;

    Booking temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Booking))) {
        if (temp.getSailingID() == sailingID && temp.getLicensePlate() == licensePlate) {
            continue;
        }
        out.write(reinterpret_cast<const char*>(&temp), sizeof(Booking));
    }

    in.close();
    out.close();
    remove(fileNameBooking.c_str());
    rename("booking_tmp.dat", fileNameBooking.c_str());

    return true;
}

int countBookingRecords() {
    ifstream in(fileNameBooking, ios::binary | ios::ate);
    if (!in) return 0;
    streampos size = in.tellg();
    return static_cast<int>(size / sizeof(Booking));
}
