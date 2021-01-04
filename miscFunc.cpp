#include "csvgraphicsData.h"

string getFileName() {
    time_t end_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string full_time = ctime(&end_time);
    istringstream iterate(full_time);
    string day;
    string mon;
    string year;
    string time;
    iterate >> mon;
    iterate >> mon;
    iterate >> day;
    iterate >> time;
    iterate >> year;
    return day + mon + year + " " + time.substr(0, 2) + time.substr(3, 2) + time.substr(6, 2);
}

size_t findLastG(const string full, string find) {
    int len = full.length();
    string check;
    while (len > 0) {
        check = full.substr(len, 1);
        if (find == check) {
            break;
        }
        len--;
    }
    return len;
}

size_t findPosG(const string full, string find, int num) {
    int i = 0;
    size_t pos = 0;
    while (i < num) {
        pos += 1;
        pos = full.find(find, pos);
        if (pos == string::npos)
            return -1;
        i++;
    }
    return pos;
}

int getNumDays(const string* monthName, int year) {
    int numdays = 31;
    if (*monthName == "Jan" || *monthName == "Mar" || *monthName == "May" || *monthName == "Jul" ||
        *monthName == "Aug" || *monthName == "Oct" || *monthName == "Dec") {
        numdays = 31;
    } else if (*monthName == "Apr" || *monthName == "Jun" || *monthName == "Sep" || *monthName == "Nov") {
        numdays = 30;
    } else if (*monthName == "Feb") {
        if (year % 400 == 0) {
            numdays = 29;
        } else if (year % 4 == 0 && year % 100 != 0) {
            numdays = 29;
        } else {
            numdays = 28;
        }
    }
    return numdays;
}

int getActualSignal(int PLCSigs[]) {
    if (PLCSigs[2] == 1) {
        return 0;
    } else if (PLCSigs[1] == 1 && PLCSigs[0] == 0) {
        return 1;
    } else return 2;
}