#ifndef GRAPHICS5_OVERLAP_H
#define GRAPHICS5_OVERLAP_H

static const int machineNums=23;
extern int currentCount[machineNums]; //Tracking count (in seconds) of each machine's current status. Resets when the
//signal is changed
extern bool plcComm;


int setup(); //setup when the program is first opened, for data backups and all that
int backup();
std::string getMonth(std::string month); //get a month from the string of day/month/year
int data_write(); //writes back the data to the excel and txt files
int save(); //saves the data before exit
int getSignalsData(); //Gets the signal from txt on opening program. Temporary, will be replaced by reading from PLC
int getDrawing();
void saveCurrent();
void readCurrent();
int readYearly();
int readMonthly();
int readWeekly();
bool getPLCData();
int dayofweek(int d, int m, int y);
bool resetWeekly(int thisMonth, int thisDay, int prevMonth, int prevDay);


#endif //GRAPHICS5_OVERLAP_H
