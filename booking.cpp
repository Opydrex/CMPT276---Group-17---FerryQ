//==========================================================================
//==========================================================================
/*
MODULE NAME: booking.cpp
Rev.1 - 24/07/2025 - Booking class implementation.
----------------------------------------------------------------------------
This module contains the Booking class implementation, as well as supporting
functions.
----------------------------------------------------------------------------
*/
#include "Booking.h"
//#include "Sailing.h"
#include "Vehicle.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Booking::Booking(const string& licensePlate,//Input
                 const string& sailingId,//Input
                 const string& phoneNumber,//Input
                 const bool& checkedIn//Input 
)
{
    this->licensePlate = licensePlate;
    this->sailingId = sailingId;
    this->phoneNumber = phoneNumber;
    this->checkedIn = checkedIn;
}

//void Booking::writeBooking(ofstream& outFile//Input; The Booking's file
//)
//{
//    if(outFile.is_open()){
//        outFile << this->sailingId << "," << this->licensePlate << "," << this->phoneNumber << "," << this->checkedIn << endl;
//        outFile.flush();
//    }
//    else {
//        cerr << "Error: Unable to open file for writing. Check file path and permissions." << endl;
//    }
//}
//
//Booking createBooking(ifstream& inFile, //Input; The Vehicle's file. Needed for getVehicleDimensions
//                      ofstream& outFile, //Input; The Vehicle's file. Needed for writeVehicle
//                      ofstream& outFileBooking //Input; The Booking's file. Needed to write the booking to file
//)
//{
//
//    string vehicleLength;
//    string vehicleHeight;
//    bool exitFlag = false;
//    while(true){// This loop controls the whole checkIn process
//        string sailingID;
//        while(true){//This loop gets the licensePlate from the user
//            cout << "Enter SailingID (ccc-dd-dd): ";
//            getline(cin >> ws, sailingID);
//
//            if(sailingID.empty()){ //Return to main-menu if nothing is entered
//                exitFlag = true;
//                break;
//            }
//
//            //If the sailing ID doesn't match the correct format, let them try again
//            else if (sailingID.length() != 9 || sailingID[3] != '-' || sailingID[6] != '-' ||
//            !isalpha(sailingID[0]) || !isalpha(sailingID[1]) || !isalpha(sailingID[2]) ||
//            !isdigit(sailingID[4]) || !isdigit(sailingID[5]) ||
//            !isdigit(sailingID[7]) || !isdigit(sailingID[8])){
//                cout << "SailingID format is incorrect. Please try again, or press Enter to exit\n";
//                continue;
//            }
//
//            //If the sailing ID doesn't exist, let them try again
//            else if(!isSailingExist(sailingID)){
//                cout << "SailingID does not exist! Please try again, or press Enter to exit\n";
//                continue;
//            }
//
//            break;
//        }
//
//        if(exitFlag){
//            break;
//        }
//
//        string licensePlate;
//        while(true){//This loop gets the licensePlate from the user
//            cout << "Enter license plate number (3 - 10 characters): ";
//            getline(cin >> ws, licensePlate);
//
//            if(licensePlate.empty()){ //
//                exitFlag = true;
//                break;
//            }
//            else if(licensePlate.length() < 3){
//                cout << "License plate is shorter than 3 characters. Please try again, or press Enter to exit\n";
//                continue;
//            }
//            else if(licensePlate.length() > 10){
//                cout << "License plate is longer than 10 characters. Please try again, or press Enter to exit\n";
//                continue;
//            }
//            break;
//        }
//        if(exitFlag){
//            break;
//        }
//
//        string phoneNumber;
//        while(true){ //This loop gets the phone number from the user
//            cout << "Enter a phone number that would be associated with the booking (between 8 and 14 characters): ";
//            getline(cin >> ws, phoneNumber);
//            if(phoneNumber.length() < 8){
//                cout << "The phone number must have at least 8 characters. Please try again\n";
//                continue;
//            }
//            else if(phoneNumber.length() > 14){
//                cout << "the phone number cannot have more than 14 characters. Please try again\n";
//                continue;
//            }
//            break;
//        }
//
//        if(isVehicleExist(licensePlate, inFile)){ //Vehcile already exists on file, use it's data to fill out the booking
//            getVehicleDimensions(licensePlate, &vehicleLength, &vehicleHeight, inFile);
//        }
//        else { //Vehicle does not already exist on file, gather data and create it
//        string vehicleLength;
//        while(true){//This loop gets the vehicle length from the user
//            cout << "Enter the vehicle's length (up to 99.9 meters): ";
//            getline(cin >> ws, vehicleLength);
//
//            if(vehicleLength.length() < 0){
//                cout << "The length of the car cannot be smaller than 0. Please try again\n";
//                continue;
//            }
//            else if(vehicleLength.length() > 99.9){
//                cout << "The length of the car cannot be larger than 99.9 Please try again\n";
//                continue;
//            }
//
//            break;
//        }
//
//        string vehicleHeight;
//        while(true){//This loop gets the vehicle height from the user
//            cout << "Enter the vehicle's length (up to 99.9 meters): ";
//            getline(cin >> ws, vehicleHeight);
//
//            if(vehicleHeight.length() < 0){
//                cout << "The height of the car cannot be smaller than 0. Please try again\n";
//                continue;
//            }
//            else if(vehicleHeight.length() > 9.9){
//                cout << "The height of the car cannot exceed 9.9 Please try again\n";
//                continue;
//            }
//
//            break;
//        }
//
//        //Write the new vehicle to file
//        Vehicle newVehicle(licensePlate, stof(vehicleHeight), stof(vehicleLength));
//        newVehicle.writeVehicle(outFile);
//    }
//
//    float vHeight = stof(vehicleHeight);
//    float vLength = stof(vehicleLength);
//
//    //Write the new booking to file
//    Booking newBooking(sailingID, licensePlate, phoneNumber, false);
//    newBooking.writeBooking(outFileBooking);
//
//    string yesNo;
//    if(vHeight > maxHeightForRegularSizedVehicle || vLength > maxLengthForRegularSizedVehicle){
//        cout << "Special-sized vehicle with a " << licensePlate << " license plate has been booked for Sailing"
//        << sailingID << ". Would you like to create another booking? (Y/N) "; //Print the prompt
//    }
//    else{
//        cout << "Normal-sized vehicle with a " << licensePlate << " license plate has been booked for Sailing"
//        << sailingID << ". Would you like to create another booking? (Y/N) "; //Print the prompt
//    }
//    cin >> yesNo; //Get user input
//    if(yesNo == "Y")
//        continue; //Loops back to the start of create booking
//
//    break; //Exits the loop
//
//    }
//}
//
//bool isBookingExist(const string& sailingId,//input
//                 const string& licensePlate,//input
//                 ifstream& inFile//Input; The booking's file
//){
//    string line;
//    while (getline(inFile, line)) {//This loop searches for a booking
//    stringstream ss(line);
//    string sailingIDFromFile, licensePlateFromFile;
//
//    if (getline(ss, sailingIDFromFile, ',') && getline(ss, licensePlateFromFile, ',')) {
//        if (sailingIDFromFile == sailingId && licensePlateFromFile == licensePlate) {
//            return true;
//        }
//    }
//
//    return false;
//    }
//}

//float calculateFare(const float& length,//input
//					const float& height//input
//)
//{
//    int totalExtra = 0;
//    if(length > maxLengthForRegularSizedVehicle){
//        int extraL = (length - maxLengthForRegularSizedVehicle); //Treat it as an int to truncate the float part
//        extraL *= extraPerMeterInLength; //Calculate the actual amount added
//        totalExtra += extraL; //Add it to the total
//    }
//    if( height > maxHeightForRegularSizedVehicle){
//        int extraH = (height - maxHeightForRegularSizedVehicle); //Treat it as an int to truncate the float part
//        extraH *= extraPerMeterInLength; //Calculate the actual amount added
//        totalExtra += extraH; //Add it to the total
//    }
//
//    return regularSizedVehicleFare + totalExtra;
//
//}
//
//void checkIn(ifstream& inFile,//Input; The booking's file
//             ifstream& inFileVehicle,//Input; the Vehicle's file
//             ofstream& outFile//Input; The booking's file
//)
//{
//    inFile.clear(); // clear any error flags
//    inFile.seekg(0, ios::beg); // move read pointer to the beginning of the file
//
//    string sailingID;
//    bool exitFlag = false;
//    while(true){// This loop controls the whole checkIn process
//        while(true){//This loop gets the licensePlate from the user
//            cout << "Enter  a SailingID (ccc-dd-dd): ";
//            getline(cin >> ws, sailingID);
//
//            if(sailingID.empty()){ //Return to main-menu if nothing is entered
//                exitFlag = true;
//                break;
//            }
//
//            //If the sailing ID doesn't match the correct format, let them try again
//            else if (sailingID.length() != 9 || sailingID[3] != '-' || sailingID[6] != '-' ||
//            !isalpha(sailingID[0]) || !isalpha(sailingID[1]) || !isalpha(sailingID[2]) ||
//            !isdigit(sailingID[4]) || !isdigit(sailingID[5]) ||
//            !isdigit(sailingID[7]) || !isdigit(sailingID[8])){
//                cout << "SailingID format is incorrect. Please try again, or press Enter to exit\n";
//                continue;
//            }
//
//            //If the sailing ID doesn't exist, let them try again
//            else if(!isSailingExist(sailingID)){ !
//
//                cout << "SailingID does not exist! Please try again, or press Enter to exit\n";
//                continue;
//            }
//
//            break;
//        }
//
//        if(exitFlag){
//            break;
//        }
//
//        string licensePlate;
//        while(true){//This loop gets the licensePlate from the user
//            cout << "Enter the vehicle's license plate (3 - 10 characters): ";
//            getline(cin >> ws, licensePlate);
//
//            if(licensePlate.empty()){ //
//                exitFlag = true;
//                break;
//            }
//            else if(licensePlate.length() < 3 || licensePlate.length() > 10){
//                cout << "licensePlate format is incorrect. Please try again, or press Enter to exit\n";
//                continue;
//            }
//            else if(!isBookingExist(sailingID, licensePlate, inFile)){
//                cout << "licensePlate does not have an associated booking. Please enter a different license plate, or press Enter to exit\n";
//                continue;
//            }
//
//            break;
//        }
//        if(exitFlag){
//            break;
//        }
//
//        bool exists = isBookingExist(sailingID, licensePlate, inFile);
//        string sailingIDFromFile, licensePlateFromFile, phoneNumberFromFile;
//        if(exists){ //Gets the booking's details so it can be re-created as a checked-in vehicle
//            string line;
//            while (getline(inFile, line)) {//This loop gets the parameters from the file
//                stringstream ss(line);
//                if (getline(ss, sailingIDFromFile, ',') && getline(ss, licensePlateFromFile, ',') && getline(ss, phoneNumberFromFile, ',')) {
//                    if (sailingIDFromFile == sailingID && licensePlateFromFile == licensePlate) {
//                        break;
//                    }
//                }
//            }
//
//        }
//        else {
//            cout << "No booking found for this license plate " << licensePlate << " and sailing " << sailingID << ".\n";
//        }
//
//        //Calculate and display the fare to the user so they can collect it
//        string len, height;
//        getVehicleDimensions(licensePlate, &len, &height, inFileVehicle);
//        float vLength = stof(len);
//        float vHeight = stof(height);
//        cout << "The fare is " << calculateFare(vLength, vHeight) << ". Press <enter> once it has been collected. ";
//        string dummy;
//        getline(cin, dummy); // wait for Enter
//
//        //Update the booking by deleting the old one and creating a new one.
//        Booking newBooking(licensePlateFromFile,sailingIDFromFile, phoneNumberFromFile, true);
//        deleteBooking(sailingIDFromFile, licensePlateFromFile,  outFile, inFile);
//        newBooking.writeBooking(outFile);
//
//    } //The check-in function will automatically loop until the user presses enter for the sailingID or the license Plate
//}

//THIS FUNCTION WILL BE FOR SIMPLY DELETING A BOOKING. WIPING IT FROM THE TXT FILE
//THE NEXT FUNCTION promptToDeleteBooking will act as a prompter, which will at the end call deleteBooking


bool deleteBooking(const string& licensePlate,
                   const string& sailingId,
                   ifstream& inFile,
                   ofstream& outFile)
{
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        istringstream iss(line);
        string fileLicensePlate, fileSailingId, phoneNumber, checkedIn;

        // Parse the line
        getline(iss, fileLicensePlate, ',');
        getline(iss, fileSailingId, ',');
        getline(iss, phoneNumber, ',');
        getline(iss, checkedIn);

        // Check if this is the booking to delete
        if (fileLicensePlate == licensePlate && fileSailingId == sailingId) {
            found = true;
            continue; // skip writing this booking
        }

        // Otherwise, keep the line
        outFile << fileLicensePlate << ','
                << fileSailingId << ','
                << phoneNumber << ','
                << checkedIn << '\n';
    }

    return found;
}




//WARNING FOR WHOEVER'S IMPLEMENTING THIS: make sure you check line 327 to see how I want to use it in checkIn. keep in mind, for booking,
// you don't need to prompt the user for whether he wants to delete another booking or not!

void promptToDeleteBooking(ifstream& inFile, ofstream& outFile) {
    string sailingId;
    string licensePlate;

    // Prompt for sailing ID
    while (true) {
        cout << "Enter Sailing ID (ccc-dd-dd): ";
        getline(cin >> ws, sailingId);

        if (sailingId.empty()) return;

        if (sailingId.length() != 9 || sailingId[3] != '-' || sailingId[6] != '-' ||
            !isalpha(sailingId[0]) || !isalpha(sailingId[1]) || !isalpha(sailingId[2]) ||
            !isdigit(sailingId[4]) || !isdigit(sailingId[5]) ||
            !isdigit(sailingId[7]) || !isdigit(sailingId[8])) {
            cout << "Bad entry! Sailing ID must follow the format ccc-dd-dd.\n";
        } else {
            break;
        }
    }


    while (true) {
        cout << "Enter license plate number (3 - 10 characters): ";
        getline(cin >> ws, licensePlate);

        if (licensePlate.empty()) return;

        if (licensePlate.length() < 3 || licensePlate.length() > 10) {
            cout << "Bad entry! License plate must be between 3 and 10 characters.\n";
        } else {
            break;
        }
    }

    // Call deleteBooking
    if (deleteBooking(licensePlate, sailingId, inFile, outFile)) {
        cout << "Booking has been successfully deleted.\n";
        //then go to main menu
    } else {
        cout << "Booking not found.\n";
        //then go to main menu
    }
}

