//==========================================================================
//==========================================================================
/*
MODULE NAME: Booking.h
Rev.1 - Booking class header created
----------------------------------------------------------------------------
This module is used to store data about bookings.
----------------------------------------------------------------------------
*/


#ifndef BOOKING_H
#define BOOKING_H


//#include "Vehicle.h"
#include <iostream>
#include <string>
using namespace std;

class Booking {
private:
    string SailingId;
    string licensePlate;
    bool checkedIn = false;//at default not checkedIn

public:
    Booking(const string& licensePlate, const string& SailingId);//createBooking() basically
    void init();
    void checkIn(const Booking& booking);
    bool isCheckedIn(const Booking& booking);
    void shutDown();


    //deleteBooking
    //findBooking
    //these two shouldn't be here


};

#endif // BOOKING_H