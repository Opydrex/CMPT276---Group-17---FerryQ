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
using namespace std;

int main(){
    // Declare the stream objects
    // Step 1: open for reading to check duplicates
    ifstream myInputFile("vessel.txt");
    if (!myInputFile) {
        cerr << "Could not open vessel.txt for reading" << endl;
        return 1;
    }



    ofstream myOutputFile("vessel.txt", ios::app);
    if (!myOutputFile) {
        cerr << "Could not open vessel.txt for appending" << endl;
        return 1;
    }

// Now call your logic
    createVessel(myInputFile, myOutputFile);

// Clean up
    myInputFile.close();
    myOutputFile.close();

}