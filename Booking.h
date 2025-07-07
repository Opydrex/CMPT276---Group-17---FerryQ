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

//----------------------------------------------------------------------------

class Booking {
    public:
        Booking(const string& licensePlate,//input
                const string& SailingId//input
                );
        //Job: Booking object constructor.
        //Usage: Used when a vehicle is getting booked for a Sailing.
        //Restrictions: License plate out of domain; SailingID must be in the domain and must exist.

        //----------------------------------------------------------------------------

        void checkIn(const Booking& booking//input
                    );
        //Job: Checks in a vehicle for a Sailing.
        //Usage: Used when a booked vehicle needs to be checked in upon arrival to the port.
        //Restrictions: Vehicle must be first booked to be checked in.

        //----------------------------------------------------------------------------

        bool isCheckedIn(const Booking& booking//input
                         );
        //Job: Returns a boolean flag indicating if the vehicle is booked in.
        //Usage: Call when need to find out if the vehicle is booked.
        //Restrictions: Booking must exist.

        //----------------------------------------------------------------------------



    private:
        string SailingId;//SailingID is used to identify on which Sailing the vehicle is booked
        string licensePlate;//vehicle's license plate. acts as a primary key
        bool checkedIn = false;//Flag to identify if vehicle has been registered

        void writeBooking(const Booking& booking//input
                          );
        //Job: Writes the Booking data into the file.
        //Usage: Used when all data about Booking is collected. It is then written into a data file.
        //Restrictions: Booking mustn't already exist in the data file.

        //----------------------------------------------------------------------------



};

#endif // BOOKING_H