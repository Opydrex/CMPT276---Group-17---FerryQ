//==========================================================================
//==========================================================================
/*
MODULE NAME: Sailing.cpp
Rev.1 - 25/07/2025 - Sailing class implementation.
----------------------------------------------------------------------------
This module contains implementation of the Sailing class and related functions.
Unsorted fixed-length text records are used. Linear searches and simple append/deletion
(in-memory streaming via a temp file) strategies keep file I/O straightforward.
----------------------------------------------------------------------------
*/

#include "Sailing.h"
#include "Booking.h"            
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>               
#include <limits>
using namespace std;
const string fileNameVessel = "vessel.txt";

//----------------------------------------------------------------------------
// Sailing constructor
//----------------------------------------------------------------------------
//makes a sailing obj using the given infomation (id, name, small & big caps)
Sailing::Sailing(const string& sailingId,   //input
                 const string& vesselName,  //input
                 const float& currentCapacitySmall,  //input
                 const float& currentCapacityBig     //input
                )
{
    this->sailingId = sailingId; //save the sailing id
    this->vesselName = vesselName; //save vessel name
    this->currentCapacitySmall = currentCapacitySmall; //small cars
    this->currentCapacityBig = currentCapacityBig; //big vehicals
}

//----------------------------------------------------------------------------
// Writes an instance of the Sailing object to the file.
//----------------------------------------------------------------------------
//open file and stick data of sailing to it in csv like format
void Sailing::writeSailing(const string& filename){
    ofstream outFile(filename, ios::app); // open in append mode
    if (outFile.is_open()){
        outFile << sailingId << ","
                << vesselName << ","
                << currentCapacitySmall << ","
                << currentCapacityBig
                << endl;
        outFile.flush(); // force write to disk just in case
    }else{
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
    }
}

//----------------------------------------------------------------------------
// Prompts the user for appropriate data and returns a Sailing object
//----------------------------------------------------------------------------
//gathers user input and creats a sailing if all data is fine
void createSailing() {
        string userInp, vessel, dayStr, hourStr, line;
        int day = 0, hour = 0;
        float capSmall = 0.0f, capBig = 0.0f;

        //3) Terminal ID
        while (true) {
            cout << "Enter departure terminal ID (ccc): ";
            getline(cin, userInp);
            if (userInp.empty()) return; //exit if the prompt is empty
            if (userInp.size() != 3 || !isalpha(userInp[0]) || !isalpha(userInp[1]) || !isalpha(userInp[2])) {
                cout << "Bad entry! Must be exactly three letters.\n";
                continue;
            }
            break; // all gud
        }
}

//----------------------------------------------------------------------------
// Delete a sailing record
//----------------------------------------------------------------------------
// ask for id then delete if it exsist, lets user retry too
void deleteSailing() {
    while (true) {
        cout << "Enter SailingID (ccc-dd-dd): ";
        string sailingId;
        getline(cin >> ws, sailingId);

        if (sailingId.empty()) return;

        //Validate format: ccc-dd-dd
        if (sailingId.length() != 9 || sailingId[3] != '-' || sailingId[6] != '-' ||
            !isalpha(sailingId[0]) || !isalpha(sailingId[1]) || !isalpha(sailingId[2]) ||
            !isdigit(sailingId[4]) || !isdigit(sailingId[5]) ||
            !isdigit(sailingId[7]) || !isdigit(sailingId[8])) {
            cout << "Bad entry! SailingID must follow the format ccc-dd-dd.\n";
        } else {
            //Attempt deleting
            const string tmp = fileNameSailing + ".tmp";
            ifstream in(fileNameSailing);
            ofstream out(tmp);
            if (!in || !out) {
                cerr << "Error accessing sailing records.\n";
                return;
            }

            string line;
            bool found = false;
            while (getline(in, line)) {
                string id;
                stringstream ss(line);
                getline(ss, id, ',');
                if (id == sailingId) {
                    found = true;
                    continue;
                }
                out << line << "\n";
            }

            in.close();
            out.close();

            if (!found) {
                remove(tmp.c_str());
                cout << "No sailing with SailingID " << sailingId
                     << " was found. Would you like to try deleting another sailing? (Y/N)\n";
            } else {
                remove(fileNameSailing.c_str());
                rename(tmp.c_str(), fileNameSailing.c_str());
                //Also delete related bookings
                const string tmpb = fileNameVehicle + ".tmp";
                ifstream fileInp(fileNameVehicle);
                ofstream bo(tmpb);
                if (fileInp && bo) {
                    while (getline(fileInp, line)) {
                        string sid;
                        stringstream ss(line);
                        getline(ss, sid, ',');
                        if (sid != sailingId) bo << line << "\n";
                    }
                    fileInp.close();
                    bo.close();
                    remove(fileNameVehicle.c_str());
                    rename(tmpb.c_str(), fileNameVehicle.c_str());
                }
                cout << "Sailing with SailingID " << sailingId
                     << " deleted successfully. Would you like to delete another sailing? (Y/N)\n";
            }
        }

        string again;
        getline(cin >> ws, again);
        if (again != "Y" && again != "y") return;
    }
}

//----------------------------------------------------------------------------
// Check if sailing already in file
//----------------------------------------------------------------------------
bool isSailingExist(const string& sailingId) {
    ifstream in(fileNameSailing);
    if (!in) return false;
    string line;
    while (getline(in, line)) {
        string id;
        stringstream ss(line);
        if (getline(ss, id, ',')) {
            if (id == sailingId) return true; //yep its there
        }
    }
    return false;
}

//----------------------------------------------------------------------------
// Print paged report with exit-on-enter and error messages
//----------------------------------------------------------------------------
//shows all the sailingss in a nice list, 5 per page
void printReport() {
    ifstream in(fileNameSailing);
    if (!in) { cerr<<"Cannot open "<<fileNameSailing<<" for report.\n"; return; }
    // header
    cout<<"==Sailings Report==\n";
    //Just formatting the report
    cout<<left<<setw(4)<<"No."<<"  "<<setw(10)<<"SailingID"<<"  " <<setw(24)<<"Vessel Name"<<"  "<<right<<setw(5)<<"LHR"<<setw(7)<<"HHR"<<"  "
        <<setw(14)<<"Total Vehicles"<<"  "<<setw(14)<<"Deck Usage(%)"<<"\n";
    //Just formatting the report
    cout<<left<<string(4,'-')<<"  "<<string(10,'-')<<"  "<<string(24,'-')<<"  "<<right<<string(5,'-')<<string(7,'-')<<"  "
        <<string(14,'-')<<"  "<<string(14,'-')<<"\n";

    string line;
    int total=0,page=0;
    bool exit=false;

    while(getline(in,line)){
        stringstream ss(line);

        string id,vesName,freeSpace,howFull;

        getline(ss,id,','); getline(ss,vesName,','); getline(ss,freeSpace,','); getline(ss,howFull,',');

        float vehicleSpace=stof(freeSpace), bookedSpace=stof(howFull);

        int totalCapacity=int(vehicleSpace+bookedSpace), usage= totalCapacity?int((bookedSpace/totalCapacity)*100):0;
        string idx=to_string(total+1)+")";
        //Just formatting the report
        cout<<right<<setw(4)<<idx<<"  "<<left<<setw(10)<<id<<"  "<<setw(24)<<vesName<<"  "<<right<<setw(5)<<fixed<<setprecision(1)<<vehicleSpace<<setw(7)<<bookedSpace<<"  "
            <<setw(14)<<totalCapacity<<"  "<<setw(14)<<(to_string(usage)+"%")<<"\n";
        total++; 
        page++;

        if(page==5){
            string inp;
            while(true){
                cout<<"Enter M for more, 0 or Enter to exit: ";
                getline(cin,inp);
                if(inp.empty()||inp=="0"){
                    exit=true;
                    break;
                }
                if(inp=="M"||inp=="m") break;
                cout<<"Bad entry! Please enter M or 0 (or press Enter).\n";
            }
            if(exit) break;
            page=0;
        }
    }
    if(!exit && total>0) cout<<"No more sailings.\n";
    in.close();
}
