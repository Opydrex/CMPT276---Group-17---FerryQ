//==========================================================================
//==========================================================================
/*
MODULE NAME: Booking.h
Rev.1 - 09/07/2025 - Booking class header created
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
const float regularSizedVehicleFare = 14.0; //constant stating regular sized vehicle fare
const float extraPerMeterInHeight = 2.0; //constant stating how much to charge per every extra meter in height
const float extraPerMeterInLength = 3.0; //constant stating how much to charge per every extra meter in length
const float maxHeightForRegularSizedVehicle = 2.0; //constant stating max height for a regular sized vehicle
const float maxLengthForRegularSizedVehicle = 7.0; //constat stating max length for a regular sized vehicle

class Booking {
    public:
        Booking(const string& licensePlate,//input
                const string& sailingId//input
                );
        //Job: Booking object constructor.
        //Usage: Used when a vehicle is getting booked for a Sailing.
        //Restrictions: licensePlate and sailingId must comply to domain. sailingIDd must exist.

        //----------------------------------------------------------------------------



        void writeBooking(const string& filename//input
                          );
        //Job: Writes the Booking data into the file.
        //Usage: Used when all data about Booking is collected. It is then written into a data file.
        //Restrictions: Booking mustn't already exist in the data file.

        //----------------------------------------------------------------------------




    private:
        string sailingId;//SailingID is used to identify on which Sailing the vehicle is booked
        string licensePlate;//vehicle's license plate. acts as a primary key
        bool checkedIn = false;//Flag to identify if vehicle has been registered





};


void checkIn(const string& sailingId,//input
             const string& licensePlate//input
             );
//Job: Checks in a vehicle for a Sailing.
//Usage: Used when a booked vehicle needs to be checked in upon arrival to the port.
//Restrictions: Vehicle must be booked for this Sailing to be checked in. Sailing must exist.

//----------------------------------------------------------------------------

bool isCheckedIn(const string& sailingId,//input
                 const string& licensePlate//input
                    );
//Job: Returns a boolean flag indicating if the vehicle is booked in.
//Usage: Call when need to find out if the vehicle is booked. Simply a getter for a checkedIn variable.
//Restrictions: Booking must exist. Sailing must exist.

//----------------------------------------------------------------------------

bool deleteBooking(const string& sailingId,//input
                   const string& licensePlate//input
                    );
//Job: Deletes a booking from the file.
//Usage: Call when need to delete a booking.
//Restrictions: Booking must exist. Sailing must exist.

//----------------------------------------------------------------------------

float calculateFare(const float& length,//input
					const float& height//input
					);

//Job: Calculates fare for a booked vehicle.
//Usage: Called when checking the vehicle in. Only call if special-sized vehicle. For normal sized - fixed fare.
//Restrictions: height and length have to be non-zero.

//----------------------------------------------------------------------------

#endif // BOOKING_H