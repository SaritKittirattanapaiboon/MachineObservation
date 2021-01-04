#include "csvgraphicsData.h"

int readcsv() {
    if(!plcComm){
        return 0;
    }
    string temp;
    //getSavedData(&temp);

    ifstream csvdata;
    ofstream temptxt;
    size_t posi, posf; //initial position and final position for deletion of data [,on value, idle value, last value]
    string fileName = "Current.csv";
    string readData;
    string fullTempData;
    int i = 0;

    csvdata.open(fileName);
    if (!csvdata.is_open()) {
        return -1;
    }
    temptxt.open("dataBackup.txt");

    if (csvdata.is_open() && temptxt.is_open()) {
        getline(csvdata, fullTempData, '\n');
        fullTempData.append("\n");
        while (i < machineNums) {
            getline(csvdata, readData, '\n');
            //cout << readData << endl;
            posi = findPos(readData, ",", 2);
            posf = findPos(readData, ",", 7);
            temp = readData.substr(posi + 1, posf - posi - 1);
            fullTempData.append(readData.substr(0, posi));
            fullTempData.append(readData.substr(posf, readData.length() - posf));
            istringstream reading(temp);
            getline(reading, readData, ',');
            getline(reading, readData, ',');
            machineSignals[i][0] = stoi(readData);
            getline(reading, readData, ',');
            machineSignals[i][1] = stoi(readData);
            getline(reading, readData, ',');
            machineSignals[i][2] = stoi(readData);
            getline(reading, readData, '\n');
            machineSignals[i][3] = stoi(readData);
            fullTempData.append("\n");
            i++;
        }
    } else {
        cout << "Not Open" << endl;
        return -1;
    }
    temptxt << fullTempData;

    csvdata.close();
    temptxt.close();
    return 0;
}

int writecsv() {
    if(!plcComm){
        return 0;
    }
    string temp;
    int day;
    day = stoi(getFileName().substr(0, 2));

    ofstream csvdata;
    ifstream temptxt;
    size_t pos;
    string fileName = "Current.csv";
    string fullTempData;
    int i = 0;

    csvdata.open(fileName);
    temptxt.open("dataBackup.txt");

    if (temptxt.is_open()) {
        getline(temptxt, fullTempData, '\n');
        fullTempData.append("\n");
        while (i < machineNums) {
            getline(temptxt, temp, '\n');
            pos = findPos(temp, ",", 2);
            temp.insert(pos + 1, to_string(inSignalsData[i]) + "," + to_string(machineSignals[i][0]) + "," +
                                 to_string(machineSignals[i][1]) + "," + to_string(machineSignals[i][2]) + "," +
                                 to_string(machineSignals[i][0] + machineSignals[i][1] + machineSignals[i][2]) + ",");
            temp = editDailyValue(temp, day, inSignalsData[i], i);
            fullTempData.append(temp + "\n");
            i++;
        }
    } else {
        return -1;
    }
    csvdata << fullTempData;
    temptxt.close();
    csvdata.close();
    //remove("dataBackup.txt");
    return 0;
}

string editDailyValue(string fullData, int colnum, int yesterdayT, int noMachine) {
    //string temp;
    size_t pos = findPos(fullData, ",", colnum + 6); //Add 5 for initial columns (Name, Drawing Num, etc)
    size_t posf = findPos(fullData, ",", colnum + 7); //Add 5 for initial columns (Name, Drawing Num, etc)
    int total;

    total = todayOn[noMachine] + todayIdle[noMachine] + todayOff[noMachine];
    fullData.erase(pos + 1, (posf - 1) - pos);
    fullData.insert(pos + 1,
                    to_string(todayOn[noMachine]) + ";" + to_string(todayOff[noMachine]) + ";" + to_string(total));
    //fullData.insert(pos+1,"Hi");
    return fullData;
}

int backupMonth(string year) {
    if(!plcComm){
        return 0;
    }
    string tmonth;
    int day;
    getSavedData(&tmonth, &day);

    ifstream current;
    ofstream oldMonth;

    string transferdata;

    current.open("Current.csv");
    if (!current.is_open()) {
        return -1;
    }
    if (tmonth == "Dec") {
        mkdir(year.substr(3, 4).c_str());
        int numyear = stoi(year.substr(3, 4));
        numyear = numyear - 1;
        year = tmonth + to_string(numyear);

    }
    string file = year.substr(3, 4) + "/" + tmonth + ".csv";
    //cout << file << endl;
    oldMonth.open(file);

    getline(current, transferdata, '\n');
    transferdata.insert(0, year.substr(3, 4));
    oldMonth << transferdata + "\n";

    while (!current.eof()) {
        getline(current, transferdata, '\n');
        oldMonth << transferdata + "\n";
    }

    current.close();
    oldMonth.close();
    return 0;
}

int createNewMonth(string monthyearName) {
    if(!plcComm){
        return 0;
    }
    string monthName = monthyearName.substr(0, 3);
    int year = stoi(monthyearName.substr(3, 4));
    int numdays = getNumDays(&monthName, year);

    string dataInput;
    string fromTemplate;
    int i = 0;
    ifstream templateFile;
    templateFile.open("Template.csv");
    if (!templateFile.is_open()) {
        return -1;
    }
    getline(templateFile, fromTemplate, '\n');
    dataInput.append(fromTemplate);
    while (i < numdays) {
        dataInput.append("," + to_string(i + 1) + "-" + monthName);
        i++;
    }
    dataInput.append("\n");
    i = 0;
    //cout << dataInput << endl;
    while (i < machineNums) {
        getline(templateFile, fromTemplate, '\n');
        dataInput.append(fromTemplate);
        dataInput.append("\n");
        i++;
    }

    templateFile.close();

    ofstream changingFiles;
    changingFiles.open("Current.csv");
    changingFiles << dataInput;
    changingFiles.close();
    return 0;
}

int backupTotals(string monthYearName) {
    if(!plcComm){
        return 0;
    }
    ifstream inputFile;
    string fullText;
    string temp;
    string month;
    int i = 0;
    getSavedData(&month, &i);
    i = 0;
    inputFile.open("Totals.csv");
    if (!inputFile.is_open()) {
        return -1;
    }
    getline(inputFile, fullText, '\n');
    if (fullText.substr(fullText.length() - 1, 1) != ",") {
        fullText.append(",");
    }
    fullText.append(monthYearName + getFileName().substr(5, 4) + "\n");
    while (i < machineNums) {
        getline(inputFile, temp, '\n');
        temp = editTotals(temp, i);
        fullText.append(temp);
        if (fullText.substr(fullText.length() - 1, 1) != ",") {
            fullText.append(",");
        }
        fullText.append(to_string(machineSignals[i][0]) + ";" + to_string(machineSignals[i][1]) + ";" +
                        to_string(machineSignals[i][2]));
        fullText.append("\n");
        i++;
    }
    inputFile.close();

    ofstream outputFile;
    outputFile.open("Totals.csv");
    outputFile << fullText;
    outputFile.close();
    return 0;
}

string editTotals(string toEdit, int machineNo) {
    string all = toEdit.substr(0, toEdit.length());
    string temp;
    size_t posi, posf;
    int on, idle, off;
    posi = findPos(toEdit, ",", 1);
    posf = findPos(toEdit, ",", 2);
    temp = toEdit.substr(posi + 1, posf - posi - 1);
    posi = findPos(temp, ";", 1);
    posf = findPos(temp, ";", 2);
    on = stoi(temp.substr(0, posi)) + machineSignals[machineNo][0];
    idle = stoi(temp.substr(posi + 1, posf - posi - 1)) + machineSignals[machineNo][1];
    off = stoi(temp.substr(posf + 1, temp.length() - posf - 1)) + machineSignals[machineNo][2];
    posi = findPos(toEdit, ",", 1);
    posf = findPos(toEdit, ",", 2);

    toEdit.erase(posi + 1, posf - posi - 1);
    toEdit.insert(posi + 1, to_string(on) + ";" + to_string(idle) + ";" + to_string(off));

    return toEdit;
}