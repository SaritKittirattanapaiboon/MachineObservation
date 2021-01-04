#ifndef DATAMAKE_COMPILED_DATALOADER_H
#define DATAMAKE_COMPILED_DATALOADER_H

#include <windows.h>
#include <chrono>
#include <sys/stat.h>

#include <iostream>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <math.h>
#include <ctime>
#include <random>

#include <string>
#include <fstream>
#include <sstream>

#include "overlap.h"

typedef std::basic_string<TCHAR> TCharString;

using namespace std;

//static const int machineNums=23; //number of machines
extern int PLCSignals[machineNums][3];
extern int inSignalsData[machineNums]; //signals read on initialization
extern int prevTime; //Unused time variable
extern int thisTime;
//extern int yesterday[machineNums]; //storing the times of yesterday to do subtraction when needed


extern int todayOn[machineNums]; //storing the on times to write to today.txt
extern int todayIdle[machineNums]; //storing the idle times to write to today.txt
extern int todayOff[machineNums]; //storing the off times to write to today.txt
extern int weeklyOn[machineNums];
extern int weeklyIdle[machineNums];
extern int weeklyOff[machineNums];
extern int monthlyOn[machineNums];
extern int monthlyIdle[machineNums];
extern int monthlyOff[machineNums];
extern int yearlyOn[machineNums];
extern int yearlyIdle[machineNums];
extern int yearlyOff[machineNums];
extern int machineSignals[machineNums][4];
extern int successes;
extern int checkTen;

///////Defined in miscFunc.cpp
string getFileName(); //get the current day/month/year time
size_t findPos(string full, string find, int num); //find position of a char in a string
int getNumDays(const string* month, int year); //get how many numbers are in a certain month
size_t findLast(string full, string find); //finds the position of the last occurance of a char in a string
int getActualSignal(int PLCSigs[]);

///////Defined in csv-related.cpp
int readcsv(); //reads Current.csv and gets the on/idle/off times
int writecsv(); //writes the updated data back to Current.csv
string editDailyValue(string fullData, int colnum, int yesterdayT, int noMachine); //edits the daily value of Current.txt
int backupMonth(string monthYear); //backs up the data to the month
int createNewMonth(string monthyearName); //creates a new Current.csv for the new month
int backupTotals(string monthYearName); //backs up the totals to totals.csv
string editTotals(string toEdit, int machineNo);

///////Defined in txt-related.cpp
string newDay(); //gets the string for today
int newtxtDay(); //makes a new today.txt
int updatetxtDay(); //updates today.txt
int getSavedData(string* month, int* day); //reads data from savedata.txt (numMachines, day, month, on/idle/off values
void writeSignals(); //write signals to signals.txt for the drawing function
int readTodayData(); //reads today.txt to get data
void writeTodayData(); //writes data back to today.txt
void writeWeekly();
void updateWeekly();
void writeYearly();
void updateYearly();
void updateMonthly();


///////Defined in testFunctions.cpp
int getSignal(int currentSignal); //Used for testing purposes, not used in actual implementation
void increaseCount(int signal, int val);




#endif //DATAMAKE_COMPILED_DATALOADER_H
