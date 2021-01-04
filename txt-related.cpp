#include "csvgraphicsData.h"

string newDay() {
    string toAdd;
    toAdd = getFileName().substr(0, 2);
    toAdd.append("\n");
    int i = 0;
    while (i < machineNums) {
        toAdd.append("0\n");
        i++;
    }
    return toAdd;
}

int newtxtDay() {
    string transfer;
    ifstream changeLog;
    changeLog.open("savedata.txt"); //savedata.txt should have 5 lines, the number of machines currently tracking (int)
    //the month the machine was last used on (JAN), the day the machine was used (int)
    //the ints for the last recorded ON/OFF/TOTAL
    if (!changeLog.is_open()) {
        return -1;
    }
    getline(changeLog, transfer, '\n');
    int number = stoi(transfer);
    getline(changeLog, transfer, '\n');
    changeLog.close();

    ofstream newSave;
    newSave.open("savedata.txt");
    newSave << number << "\n";
    if (transfer == getFileName().substr(2, 3)) {
        newSave << transfer << "\n";
    } else {
        newSave << getFileName().substr(2, 3) << "\n";
    }
    newSave << newDay();
    newSave.close();
    return 0;
}

int updatetxtDay() {
    string transfer;
    ifstream changeLog;
    changeLog.open("savedata.txt"); //savedata.txt should have 5 lines, the number of machines currently tracking (int)
    //the month the machine was last used on (JAN), the day the machine was used (int)
    //the ints for the last recorded TOTAL
    if (!changeLog.is_open()) {
        return -1;
    }
    getline(changeLog, transfer, '\n');
    int number = stoi(transfer);
    getline(changeLog, transfer, '\n');
    changeLog.close();

    ofstream newSave;
    newSave.open("savedata.txt");
    newSave << number << "\n";
    newSave << getFileName().substr(2, 3) << "\n";
    newSave << getFileName().substr(0, 2) << "\n";
    int i = 0;
    while (i < machineNums) {
        newSave << to_string(currentCount[i])
                << "\n";
        i++;
    }
    newSave.close();
    return 0;
}

int getSavedData(string* month, int* day) {
    ifstream saved;
    saved.open("savedata.txt");
    if (!saved.is_open()) {
        cout << "savedata not open" << endl;
        return -1;
    }
    int num;
    int i = 0;
    string tmonth;
    string temp;
    getline(saved, tmonth, '\n');
    num = stoi(tmonth);
    getline(saved, tmonth, '\n');
    *month = tmonth;
    getline(saved, temp, '\n');
    *day = stoi(temp);
    while (i < num) {
        //cout << "blegh" << endl;
        getline(saved, temp, '\n');
        //currentCount[i]=0;
        i++;
    }
    saved.close();
    return num;
}

void writeSignals() {
    ofstream output;
    output.open("signals.txt");
    int i = 0;
    while (i < machineNums) {
        output << to_string(inSignalsData[i]) << "\n";
        i++;
    }
    output.close();
}

int readTodayData() {
    ifstream today;
    today.open("today.txt");
    if (!today.is_open()) {
        return -1;
    }
    int i = 0;
    string read;
    string divided;
    while (i < machineNums) {
        getline(today, read, '\n');
        istringstream divider(read);
        getline(divider, divided, ',');
        todayOn[i] = stoi(divided);
        getline(divider, divided, ',');
        todayIdle[i] = stoi(divided);
        getline(divider, divided, '\n');
        todayOff[i] = stoi(divided);
        i++;
    }
    today.close();
    return 0;
}

void writeTodayData() {
    ofstream today;
    today.open("today.txt");
    int i = 0;
    string read;
    while (i < machineNums) {
        read.append(to_string(todayOn[i]) + "," + to_string(todayIdle[i]) + "," + to_string(todayOff[i]) + "\n");
        i++;
    }
    today << read;
    today.close();
}

void writeWeekly(){
    ofstream week;
    week.open("weekly.txt");
    for(int x=0; x<machineNums; x++) {
        week << "0,0,0" << endl;
    }
    week.close();
}

void updateWeekly(){
    ofstream week;
    week.open("weekly.txt");
    for(int x=0; x<machineNums; x++) {
        week << to_string(weeklyOn[x]) << "," << to_string(weeklyIdle[x]) << "," << to_string(weeklyOff[x]) << "," << endl;
    }
    week.close();
}

int readWeekly(){
    ifstream week;
    week.open("weekly.txt");
    if (!week.is_open()) {
        return -1;
    }
    int i = 0;
    string read;
    string divided;
    while (i < machineNums) {
        getline(week, read, '\n');
        istringstream divider(read);
        getline(divider, divided, ',');
        weeklyOn[i] = stoi(divided);
        getline(divider, divided, ',');
        weeklyIdle[i] = stoi(divided);
        getline(divider, divided, '\n');
        weeklyOff[i] = stoi(divided);
        i++;
    }
    week.close();
    return 0;
}

void writeYearly(){
    ofstream week;
    week.open("yearly.txt");
    for(int x=0; x<machineNums; x++) {
        week << "0,0,0" << endl;
    }
    week.close();
}

void updateYearly(){
    ofstream week;
    week.open("yearly.txt");
    for(int x=0; x<machineNums; x++) {
        week << to_string(yearlyOn[x]) << "," << to_string(yearlyIdle[x]) << "," << to_string(yearlyOff[x]) << "," << endl;
    }
    week.close();
}

int readYearly(){
    ifstream week;
    week.open("yearly.txt");
    if (!week.is_open()) {
        return -1;
    }
    int i = 0;
    string read;
    string divided;
    while (i < machineNums) {
        getline(week, read, '\n');
        istringstream divider(read);
        getline(divider, divided, ',');
        yearlyOn[i] = stoi(divided);
        getline(divider, divided, ',');
        yearlyIdle[i] = stoi(divided);
        getline(divider, divided, '\n');
        yearlyOff[i] = stoi(divided);
        i++;
    }
    week.close();
    return 0;
}

void updateMonthly(){
    ofstream week;
    week.open("monthly.txt");
    for(int x=0; x<machineNums; x++) {
        week << to_string(machineSignals[x][0]) << "," << to_string(machineSignals[x][1]) << "," << to_string(machineSignals[x][2]) << "," << endl;
    }
    week.close();
}

int readMonthly(){
    ifstream week;
    week.open("monthly.txt");
    if (!week.is_open()) {
        return -1;
    }
    int i = 0;
    string read;
    string divided;
    while (i < machineNums) {
        getline(week, read, '\n');
        istringstream divider(read);
        getline(divider, divided, ',');
        monthlyOn[i] = stoi(divided);
        getline(divider, divided, ',');
        monthlyIdle[i] = stoi(divided);
        getline(divider, divided, '\n');
        monthlyOff[i] = stoi(divided);
        i++;
    }
    week.close();
    return 0;
}