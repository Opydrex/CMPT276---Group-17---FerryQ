//==========================================================================
// VehicleIO.h - Header for Vehicle file I/O operations
//==========================================================================
#ifndef VEHICLEIO_H
#define VEHICLEIO_H

#include "Vehicle.h"
#include <string>
using namespace std;

const string fileNameVehicle = "vehicle.dat";

// Appends a new vehicle to the binary file
bool appendVehicle(const Vehicle& vehicle);

// Checks if a vehicle exists by license plate
bool isVehicleExist(const string& licensePlate);

// Retrieves vehicle dimensions (length and height) by license plate
bool getVehicleDimensions(const string& licensePlate, float& length, float& height);

#endif
