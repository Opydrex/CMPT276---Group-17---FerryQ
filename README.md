# FerryQ

FerryQ — simple ferry booking & sailing simulation  
CMPT 276 — Group 17

## Summary

A C++ console program that models vessels, sailings, vehicles, and bookings. Designed for coursework: easy to build, run, and extend.

## Features

- Create and track sailings and assign vessels  
- Handle vehicle & passenger bookings  
- Simple console-based UI  
- Basic file save/load and simple tests

## Requirements

- C++ compiler supporting C++11 or newer (g++, clang++)  
- `make` is optional

## Build

From the repository root, compile all source files into an executable named `ferryq`:

    g++ -std=c++11 *.cpp -o ferryq

If there are test programs (example: `testFileOps.cpp`), compile and run them like this:

    g++ -std=c++11 testFileOps.cpp *.cpp -o testFileOps
    ./testFileOps

## Run

    ./ferryq

Follow the on-screen prompts to create sailings, add bookings, list vessels, etc.



# Project layout

## File I/O modules

BookingFileIO.h / BookingFileIO.cpp

SailingFileIO.h / SailingFileIO.cpp

VehicleFileIO.h / VehicleFileIO.cpp

VesselFileIO.h / VesselFileIO.cpp


## User I/O modules

BookingUserIO.h / BookingUserIO.cpp

SailingUserIO.h / SailingUserIO.cpp

VehicleUserIO.h / VehicleUserIO.cpp

VesselUserIO.h / VesselUserIO.cpp

UserInterface.h / UserInterface.cpp — overall console UI

## Other

createBookingTest.cpp — booking test

testFileOps.cpp — file operations test

main.cpp — program entry point



## Contributors

Group 17 — CMPT 276  
Alexander Barbedo, Ahmed Khaled, Aly Soliman, Vladimir Yegorin
