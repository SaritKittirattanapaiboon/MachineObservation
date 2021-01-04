#include "csvgraphicsData.h"

int PLCSignals[machineNums][3];
int inSignalsData[machineNums];
int prevTime;
int thisTime;
//int yesterday[machineNums];
int machineInUse[machineNums];
int currentCount[machineNums];
int todayOn[machineNums];
int todayIdle[machineNums];
int todayOff[machineNums];
int weeklyOn[machineNums];
int weeklyIdle[machineNums];
int weeklyOff[machineNums];
int monthlyOn[machineNums];
int monthlyIdle[machineNums];
int monthlyOff[machineNums];
int yearlyOn[machineNums];
int yearlyIdle[machineNums];
int yearlyOff[machineNums];
int machineSignals[machineNums][4];
string drawing[machineNums];
int successes;
int checkTen;

int setup() {
    TCHAR filename2[MAX_PATH + 1];
    if (GetModuleFileName(NULL, filename2, MAX_PATH) != 0) {
        TCharString strExe = filename2;
    }
    string filename = filename2;
    size_t pos;
    pos = findLast(filename, "\\");
    filename = filename.substr(0, pos);
    //cout << filename << endl;
    const char *testFile;
    testFile = filename.c_str();

    srand(time(NULL));
    prevTime = stoi(getFileName().substr(10, 6));
    int day;
    string currentMonth;
    string currentDateTime = getFileName();
    string thisMonth=currentDateTime.substr(2,3);
    int tempDay = stoi(currentDateTime.substr(0,2));
    int numberofDays=getNumDays(&thisMonth,stoi(currentDateTime.substr(5,4)));
    int tempMonth = stoi(getMonth(currentDateTime.substr(2,3)));
    int tempYear = stoi(currentDateTime.substr(5,4));
    int thisday = dayofweek(tempDay,tempMonth,tempYear);
    //cout << thisday << endl;
    readYearly();
    prevTime = stoi(currentDateTime.substr(10, 6));
    if (getSavedData(&currentMonth, &day) == -1) {
        return -1;
    }

    if(resetWeekly(stoi(getMonth(thisMonth)),tempDay,stoi(getMonth(currentMonth)),day)){
        writeWeekly();
        for(int x=0;x<machineNums;x++) {
            weeklyOn[x] = 0;
            weeklyIdle[x] = 0;
            weeklyOff[x] = 0;
        }
    }
    else{
        if(readWeekly() == -1){
            return -1;
        }
    }

    if (backup() == -1) {
        return -1;
    }

    //Check for different month, then for different day
    if (currentMonth != currentDateTime.substr(2, 3)) { //Different month is now working. Fixed
        if (readcsv() == -1) {
            return -1;
        }
        if (backupMonth(currentDateTime.substr(2, 7)) == -1) {
            return -1;
        }
        if (backupTotals(currentMonth) == -1) {
            return -1;
        }
        if (createNewMonth(currentDateTime.substr(2, 7)) == -1) {
            return -1;
        }
        int i = 0;
        while (i < 23) {
            todayOn[i] = 0;
            todayIdle[i] = 0;
            todayOff[i] = 0;
            currentCount[i]=0;
            i++;
        }
        newtxtDay();
        if (getSignalsData() == -1) {
            return -1;
        }
        if (getSavedData(&currentMonth, &day) == -1) {
            return -1;
        }
        if(currentMonth=="Jan"){
            i = 0;
            while (i < 23) {
                yearlyOn[i] = 0;
                yearlyIdle[i] = 0;
                yearlyOff[i] = 0;
                //currentCount[i]=0;
                i++;
                writeYearly();
            }
        }
    } else if (day != stoi(currentDateTime.substr(0, 2))) {
        //Get yesterday from savedata.txt and update savedata.txt
        //Write yesterday data to current.csv
        if (readcsv() == -1) {
            return -1;
        }
        int i = 0;
        while (i < 23) {
            todayOn[i] = 0;
            todayIdle[i] = 0;
            todayOff[i] = 0;
            currentCount[i]=0;
            i++;
        }
        newtxtDay();
        if (getSignalsData() == -1) {
            return -1;
        }
        if (getSavedData(&currentMonth, &day) == -1) {
            return -1;
        }
    } else {
        if (readTodayData() == -1) {
            return -1;
        }
        if (getSavedData(&currentMonth, &day) == -1) {
            return -1;
        }
    }

    if (readcsv() == -1) {
        return -1;
    }
    return 0;
}

int backup() {
    ifstream toBackup;
    ofstream willBackup;
    string sentenceBackup;
    toBackup.open("Current.csv");
    if (!toBackup.is_open()) {
        return -1;
    }
    willBackup.open("Current-Backup.csv");
    while (!toBackup.eof()) {
        getline(toBackup, sentenceBackup, '\n');
        willBackup << sentenceBackup << "\n";
    }
    toBackup.close();
    willBackup.close();

    toBackup.open("Totals.csv");
    if (!toBackup.is_open()) {
        return -1;
    }
    willBackup.open("Totals-Backup.csv");
    while (!toBackup.eof()) {
        getline(toBackup, sentenceBackup, '\n');
        willBackup << sentenceBackup << "\n";
    }
    toBackup.close();
    willBackup.close();
    return 0;
}

int data_write() {
    int i = 0;
    int day;
    string currentMonth;
    string currentDateTime;
    if (getSavedData(&currentMonth, &day) == -1) {
        return -1;
    }
    //readYearly();
    currentDateTime = getFileName();
    int tempDay = stoi(currentDateTime.substr(0,2));
    int tempMonth = stoi(getMonth(currentDateTime.substr(2,3)));
    int tempYear = stoi(currentDateTime.substr(5,4));
    int thisday = dayofweek(tempDay,tempMonth,tempYear);
    //cout << currentMonth << endl;
    //cout << currentDateTime.substr(2, 3) << endl;

    if(resetWeekly(tempMonth,tempDay,stoi(getMonth(currentMonth)),day)){
        writeWeekly();
        for(int x=0;x<machineNums;x++) {
            weeklyOn[x] = 0;
            weeklyIdle[x] = 0;
            weeklyOff[x] = 0;
        }
    }
    else{
        if(readWeekly() == -1){
            return -1;
        }
    }

    if (currentMonth != currentDateTime.substr(2, 3)) {
        //cout << "Month Changed" << endl;
        readcsv();
        backupMonth(currentDateTime.substr(2, 7));
        backupTotals(currentMonth);
        createNewMonth(currentDateTime.substr(2, 7));
        i = 0;
        while (i < machineNums) {
            todayOn[i] = 0;
            todayIdle[i] = 0;
            todayOff[i] = 0;
            inSignalsData[i] = 0;
            i++;
        }
        newtxtDay();
        getSavedData(&currentMonth, &day);
        if(currentMonth=="Jan"){
            i = 0;
            while (i < 23) {
                yearlyOn[i] = 0;
                yearlyIdle[i] = 0;
                yearlyOff[i] = 0;
                //currentCount[i]=0;
                i++;
                writeYearly();
            }
        }
    } else if (day != stoi(currentDateTime.substr(0, 2))) {
        //cout << "Day Changed" << endl;
        //Get yesterday from savedata.txt and update savedata.txt
        //Write yesterday data to current.csv
        i = 0;
        while (i < machineNums) {
            todayOn[i] = 0;
            todayIdle[i] = 0;
            todayOff[i] = 0;
            inSignalsData[i] = 0;
            i++;
        }
        newtxtDay();
        getSavedData(&currentMonth, &day);
    }

    if (readcsv() == -1) {
        return -1;
    }
    //getSignalFromPLC(incomeSignals);
    if (getSignalsData() == -1) {
        return -1;
    }
    /*
    if (stoi(getFileName().substr(10, 6)) - prevTime > 3) {
        //cout << getFileName().substr(10,2) << ":" << getFileName().substr(12,2) << ":" << getFileName().substr(14,2) << " - " << "Fetching Data. Please Wait" << endl;
        if (getPLCData()) {
            getSignalsData();
            successes = successes + 1;
            prevTime = stoi(getFileName().substr(10, 6));
        } else {
            cout << getFileName().substr(10, 2) << ":" << getFileName().substr(12, 2) << ":"
                 << getFileName().substr(14, 2) << " - " << "Cannot fetch data" << endl;
        }
        checkTen++;
        if (checkTen == 10) {
            cout << getFileName().substr(10, 2) << ":" << getFileName().substr(12, 2) << ":"
                 << getFileName().substr(14, 2) << " - " << successes << "/" << checkTen << " received." << endl;
            successes=0; checkTen=0;
        }
    }
     */

    i = 0;
    thisTime = stoi(getFileName().substr(10, 6));
    while (i < machineNums) {
        increaseCount(inSignalsData[i], i);
        i++;
    }
    prevTime = thisTime;

    //writeSignals();
    //Remove the above version when can read from PLC
    save();
    readcsv();
    return 0;
}

int save() {
    if (writecsv() == -1) {
        return -1;
    }
    if (updatetxtDay() == -1) {
        return -1;
    }
    writeTodayData();
    saveCurrent();
    updateWeekly();
    updateMonthly();
    updateYearly();
    if (backupMonth(getFileName().substr(2, 7)) == -1) {
        return -1;
    }
    return 0;
}

int getSignalsData() {
    ifstream input;
    input.open("signals.txt");
    if (!input.is_open()) {
        return -1;
    }
    int i = 0;
    string temp;
    getline(input, temp, '\n');
    if (temp.empty()) {
    } else {
        inSignalsData[i] = stoi(temp);
        i++;
        while (i < machineNums) {
            getline(input, temp, '\n');
            inSignalsData[i] = stoi(temp);
            i++;
        }
    }
    input.close();
    return 0;
}

int getDrawing() {
    ifstream input;
    input.open("drawing.txt");
    if (input.is_open()) {
        int i = 0;
        string temp;
        getline(input, temp, '\n');
        if (temp.empty()) {
        } else {
            drawing[i] = temp;
            i++;
            while (i < machineNums) {
                getline(input, temp, '\n');
                drawing[i] = temp;
                i++;
            }
        }
        input.close();
        return 0;
    } else {
        input.close();
        return -1;
    }
}

bool getPLCData() {
    int x = rand() % 100000;
    return x < 32000;
}

void saveCurrent(){
    ofstream currenttxt;
    currenttxt.open("currenttxt.txt");
    for(int i=0; i<machineNums; i++){
        currenttxt << currentCount[i] << "\n";
    }
    currenttxt.close();
}

void readCurrent() {
    ifstream currenttxt;
    string temp;
    currenttxt.open("currenttxt.txt");
    if (currenttxt.is_open()) {
        for (int i = 0; i < machineNums; i++) {
            getline(currenttxt, temp, '\n');
            currentCount[i] = stoi(temp);
        }
    } else {
        for (int i = 0; i < machineNums; i++) {
            currentCount[i] = 0;
        }
    }
}

string getMonth(string month) {
    if (month == "Jan") {
        return "01";
    } else if (month == "Feb") {
        return "02";
    } else if (month == "Mar") {
        return "03";
    } else if (month == "Apr") {
        return "04";
    } else if (month == "May") {
        return "05";
    } else if (month == "Jun") {
        return "06";
    } else if (month == "Jul") {
        return "07";
    } else if (month == "Aug") {
        return "08";
    } else if (month == "Sep") {
        return "09";
    } else if (month == "Oct") {
        return "10";
    } else if (month == "Nov") {
        return "11";
    } else if (month == "Dec") {
        return "12";
    } else {
        return "";
    }
}

int dayofweek(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3,
                       5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y / 4 - y / 100 +
             y / 400 + t[m - 1] + d) % 7;
}

bool resetWeekly(int thisMonth, int thisDay, int prevMonth, int prevDay){
    int curDayWeek = dayofweek(thisDay,thisMonth,stoi(getFileName().substr(5,4)));
    int prevDayWeek = dayofweek(prevDay,prevMonth,stoi(getFileName().substr(5,4)));
    if(thisMonth==prevMonth){
        if(curDayWeek == 1 && thisDay!=prevDay){
            return true;
        }
        else if(prevDayWeek>=curDayWeek && thisDay!=prevDay){
            return true;
        }
        else return curDayWeek>1 && (thisDay-prevDay>=7);
    }
    else{
        if(curDayWeek==1 && thisDay!=prevDay){
            return true;
        }
        else if(prevDayWeek>=curDayWeek){
            return true;
        }
        else if(thisDay>7){
            return true;
        }
        else if(prevDay<24){
            return true;
        }
        else return curDayWeek>1 && ((7-prevDayWeek) >= thisDay);
    }
    return true;
}