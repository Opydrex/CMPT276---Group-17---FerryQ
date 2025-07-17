//==========================================================================
//==========================================================================
/*
MODULE NAME: testFileOps.cpp
Rev.1 - 17/07/2025 - test file operations module created
----------------------------------------------------------------------------
This module contains the operations and functions that will test the program
----------------------------------------------------------------------------
*/

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Vessel.h"
#include "vessel.txt"
using namespace std;

int main(){
    // Declare the stream objects
    std::ifstream myInputFile;
    std::ofstream myOutputFile;

    // Open the physical files on disk
    myInputFile.open("vessel.txt");
    myOutputFile.open("vessel.txt");

    // Check if the files were opened successfully
    if (!myInputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1; // Exit with an error code
    }
    if (!myOutputFile.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return 1; // Exit with an error code
    }

    // Testing function
	createVessel(myInputFile, myOutputFile);

    // Close the files
    myInputFile.close();
    myOutputFile.close();
}