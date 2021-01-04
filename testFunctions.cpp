#include "csvgraphicsData.h"

int getSignal(int currentSignal) {
    int x;
    x = rand() % 1000;
    if (x < 990) {
        return currentSignal;
    } else {
        x = rand() % 1000;
        if (x > 990) {
            return 0;
        } else if (x > 10) {
            return currentSignal;
        }
        return int(pow((rand() % 3), currentSignal)) % 3;
    }
}

void increaseCount(int signal, int val) {
    if (signal == 0) {
        machineSignals[val][0] += 1;
        todayOn[val] += 1;
        weeklyOn[val] += 1;
        yearlyOn[val] += 1;
        //yesterday[val]+=1;
    } else if (signal == 1) {
        machineSignals[val][1] += 1;
        todayIdle[val] += 1;
        weeklyIdle[val] += 1;
        yearlyIdle[val] += 1;
        //yesterday[val]+=1;
    } else if (signal == 2) {
        machineSignals[val][2] += 1;
        todayOff[val] += 1;
        weeklyOff[val] += 1;
        yearlyOff[val] += 1;
        //yesterday[val]+=1;
    }
}