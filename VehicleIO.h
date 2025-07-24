//==========================================================================
// VehicleIO.h - Header for Vehicle file I/O operations
//==========================================================================
#ifndef VEHICLEIO_H
#define VEHICLEIO_H

#include "Vehicle.h"
#include <string>
using namespace std;

const string fileNameVehicle = "vehicle.dat";

// Low-level Vehicle file operations:
bool appendVehicle(fstream& vehicleFile, const Vehicle& vehicle);
// Appends a new vehicle record to the vehicle file (open fstream).

bool isVehicleExist(fstream& vehicleFile, const string& licensePlate);
// Checks if a vehicle with the given license plate exists in the open vehicle file.

bool getVehicleDimensions(fstream& vehicleFile, const string& licensePlate, float& length, float& height);
// Finds a vehicle by license plate in the open file and outputs its length and height (returns true if found).

#endif // VEHICLEIO_H