#include "csvgraphics.h"
#include "csvgraphicsData.h"

void drawModeZero(int machineNo, int centerx, int centery) {
    string allNames[50];
    checkError = readMachines(allNames);
    if (checkError == -1) {
        mode = -1;
        return;
    }
    string temp;
    if (machineNo < machineNums) {
        chooseMColor(machineNo);
        if (inSignals[machineNo] == 2 && currentCount[machineNo] > 300 && blink[machineNo] > -1 &&
            stoi(getFileNameG().substr(14, 2)) % 2 == 1 && clear[machineNo] && machineInUse[machineNo]==1) {
            int j = machineNo % 4;
            int i = (machineNo - j) / 4;
            setColor(DARKGREEN);
            drawFilledBox(j * WINDOW_WIDTH * 0.25, WINDOW_HEIGHT - (i * WINDOW_HEIGHT * .166666),
                          (j + 1) * WINDOW_WIDTH * 0.25, WINDOW_HEIGHT - ((i + 1) * WINDOW_HEIGHT * .166666));
        }
        if (inSignals[machineNo] == 1 && currentCount[machineNo] > 1800 && blink[machineNo] > -1 &&
            stoi(getFileNameG().substr(14, 2)) % 2 == 1 && clear[machineNo] && machineInUse[machineNo]==1) {
            int j = machineNo % 4;
            int i = (machineNo - j) / 4;
            setColor(DARKGREEN);
            drawFilledBox(j * WINDOW_WIDTH * 0.25, WINDOW_HEIGHT - (i * WINDOW_HEIGHT * .166666),
                          (j + 1) * WINDOW_WIDTH * 0.25, WINDOW_HEIGHT - ((i + 1) * WINDOW_HEIGHT * .166666));
        }
        //cout << allNames[machineNo] << " " << inSignals[machineNo] << endl;
        switch (state[machineNo]) {
            case 0: { //Initial Case: Name Center Top, Light Center Middle
                chooseMColor(machineNo);
                drawFilledCircle(centerx, centery - 15, 50);
                setColor(WHITE);
                drawString(allNames[machineNo], centerx, centery);
                //drawLine(centerx,0,centerx,1080);
                break;
            }
            case 1: { //Current runtime case
                chooseMColor(machineNo);
                drawFilledCircle(centerx - 175, centery - 15, 50);
                setColor(WHITE);
                drawString(allNames[machineNo], centerx, centery);
                temp = getTime(currentCount[machineNo]);
                drawMediumNumber(temp[1], centerx - 110, centery - 65);
                drawMediumNumber(temp[2], centerx - 60, centery - 65);
                drawBigColon(centerx - 17, centery - 65);
                drawMediumNumber(temp[4], centerx, centery - 65);
                drawMediumNumber(temp[5], centerx + 50, centery - 65);
                drawBigColon(centerx + 91, centery - 65);
                drawMediumNumber(temp[7], centerx + 110, centery - 65);
                drawMediumNumber(temp[8], centerx + 160, centery - 65);
                break;
            }
            case 2: { //On/Idle/Off Data (daily) case
                chooseMColor(machineNo);
                drawFilledCircle(centerx - 175, centery - 15, 50);
                setColor(WHITE);

                drawString(allNames[machineNo], centerx, centery);
                drawString("d", centerx + 220, centery);

                drawString("on", centerx - 80, centery - 45);
                temp = getTime(signalOn[machineNo]);
                drawString(temp, centerx + 120, centery - 45);

                drawString("idle", centerx - 55, centery - 85);
                temp = getTime(signalIdle[machineNo]);
                drawString(temp, centerx + 120, centery - 85);

                drawString("off", centerx - 70, centery - 125);
                temp = getTime(signalOff[machineNo]);
                drawString(temp, centerx + 120, centery - 125);
                break;
            }
            case 3: { //On/Idle/Off Data (weekly) case
                chooseMColor(machineNo);
                drawFilledCircle(centerx - 175, centery - 15, 50);
                setColor(WHITE);
                drawString(allNames[machineNo], centerx, centery);
                drawString("w", centerx + 220, centery);

                drawString("on", centerx - 80, centery - 45);
                temp = getTime(weeklyOn[machineNo]);
                drawString(temp, centerx + 120, centery - 45);

                drawString("idle", centerx - 55, centery - 85);
                temp = getTime(weeklyIdle[machineNo]);
                drawString(temp, centerx + 120, centery - 85);

                drawString("off", centerx - 70, centery - 125);
                temp = getTime(weeklyOff[machineNo]);
                drawString(temp, centerx + 120, centery - 125);
                break;
            }
            case 4: { //On/Idle/Off Data (monthly) case
                chooseMColor(machineNo);
                drawFilledCircle(centerx - 175, centery - 15, 50);
                setColor(WHITE);
                drawString(allNames[machineNo], centerx, centery);
                drawString("m", centerx + 220, centery);

                drawString("on", centerx - 80, centery - 45);
                if(plcComm) {
                    temp = getTime(machineSignals[machineNo][0]);
                }
                else{
                    temp = getTime(monthlyOn[machineNo]);
                }
                drawString(temp, centerx + 120, centery - 45);

                drawString("idle", centerx - 55, centery - 85);
                if(plcComm) {
                    temp = getTime(machineSignals[machineNo][1]);
                }
                else{
                    temp = getTime(monthlyIdle[machineNo]);
                }
                //temp = getTime(machineSignals[machineNo][1]);
                drawString(temp, centerx + 120, centery - 85);

                drawString("off", centerx - 70, centery - 125);
                if(plcComm) {
                    temp = getTime(machineSignals[machineNo][2]);
                }
                else{
                    temp = getTime(monthlyOff[machineNo]);
                }
                //temp = getTime(machineSignals[machineNo][2]);
                drawString(temp, centerx + 120, centery - 125);
                break;
            }
            case 5: { //On/Idle/Off Data (yearly) case
                chooseMColor(machineNo);
                drawFilledCircle(centerx - 175, centery - 15, 50);
                setColor(WHITE);
                drawString(allNames[machineNo], centerx, centery);
                drawString("y", centerx + 220, centery);

                drawString("on", centerx - 80, centery - 45);
                temp = getTime(yearlyOn[machineNo]);
                drawString(temp, centerx + 120, centery - 45);

                drawString("idle", centerx - 55, centery - 85);
                temp = getTime(yearlyIdle[machineNo]);
                drawString(temp, centerx + 120, centery - 85);

                drawString("off", centerx - 70, centery - 125);
                temp = getTime(yearlyOff[machineNo]);
                drawString(temp, centerx + 120, centery - 125);
                break;
            }
            case 6: { //Drawing Number
                drawFilledCircle(centerx - 175, centery - 15, 50);
                setColor(WHITE);
                drawString(allNames[machineNo], centerx, centery);
                drawString("drawing", centerx, centery - 60);
                drawString(":", centerx + 100, centery - 60);
                drawString(drawing[machineNo], centerx, centery - 100);
                break;
            }
            case 7: { //Machine dates (obtained and fixed) case [Right click box to access]
                //drawFilledCircle(centerx - 175, centery - 15, 50);
                drawString(allNames[machineNo], centerx, centery);
                setColor(WHITE);
                drawString("obtained:" + obtainedDate[machineNo], centerx, centery - 50);
                //cout << fixedDate[machineNo] << endl;
                if (fixedDate[machineNo] == "none") {
                    drawString("fixed: none", centerx, centery - 100);
                } else {
                    drawString("fixed:" + fixedDate[machineNo], centerx, centery - 100);
                }
                break;
            }

            default: {
                break;
            }
        }
    } else {
        setColor(WHITE);
        if (mouseState == 0) {
            drawTime(1465, 0);
        } else {
            drawExit();
        }
    }
}

void drawModeOne() {
    //Draw the "road"
    setColor(WHITE);
    drawLine(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.70, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT);
    drawLine(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.1, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * .65);
    drawLine(WINDOW_WIDTH * .625, WINDOW_HEIGHT * .45, WINDOW_WIDTH * .625, WINDOW_HEIGHT * .9);
    drawLine(WINDOW_WIDTH * .625, 0, WINDOW_WIDTH * .625, WINDOW_HEIGHT * .4);

    //drawLine(WINDOW_WIDTH*.9,WINDOW_HEIGHT*0.1,WINDOW_WIDTH*.625,WINDOW_HEIGHT*0.1);
    drawLine(WINDOW_WIDTH, WINDOW_HEIGHT * .9, WINDOW_WIDTH * .625, WINDOW_HEIGHT * .9);
    drawLine(0, WINDOW_HEIGHT * 0.1, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.1);

    drawLine(WINDOW_WIDTH * .625, WINDOW_HEIGHT * 0.45, WINDOW_WIDTH, WINDOW_HEIGHT * 0.45);
    drawLine(WINDOW_WIDTH * .625, WINDOW_HEIGHT * 0.40, WINDOW_WIDTH, WINDOW_HEIGHT * 0.40);

    drawLine(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.65, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.65);
    drawLine(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.70, WINDOW_WIDTH * 0.13, WINDOW_HEIGHT * 0.70);

    drawLine(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.65, WINDOW_WIDTH * 0.1, WINDOW_HEIGHT);
    drawLine(WINDOW_WIDTH * 0.13, WINDOW_HEIGHT * 0.7, WINDOW_WIDTH * 0.13, WINDOW_HEIGHT);

    //CNC's & NC
    chooseMColor(1);
    drawFilledBox(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.40, WINDOW_WIDTH * 0.28, WINDOW_HEIGHT * 0.13); //CNC #2
    setColor(BLACK);
    drawString("c1", WINDOW_WIDTH * 0.319, WINDOW_HEIGHT * 0.21);

    chooseMColor(2);
    drawFilledBox(WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.62, WINDOW_WIDTH * 0.32, WINDOW_HEIGHT * 0.52); //CNC #1
    setColor(BLACK);
    drawString("c2", WINDOW_WIDTH * 0.336, WINDOW_HEIGHT * 0.52);

    chooseMColor(3);
    drawFilledBox(WINDOW_WIDTH * 0.48, WINDOW_HEIGHT * 0.36, WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.26); //CNC #3
    setColor(BLACK);
    drawString("c3", WINDOW_WIDTH * 0.466, WINDOW_HEIGHT * 0.26);

    chooseMColor(4);
    drawFilledBox(WINDOW_WIDTH * 0.48, WINDOW_HEIGHT * 0.49, WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.39); //CNC #4
    setColor(BLACK);
    drawString("c4", WINDOW_WIDTH * 0.466, WINDOW_HEIGHT * 0.39);

    chooseMColor(5);
    drawFilledBox(WINDOW_WIDTH * 0.48, WINDOW_HEIGHT * 0.62, WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.52); //CNC #5
    setColor(BLACK);
    drawString("c5", WINDOW_WIDTH * 0.466, WINDOW_HEIGHT * 0.52);

    chooseMColor(19);
    drawFilledBox(WINDOW_WIDTH * 0.48, WINDOW_HEIGHT * 0.23, WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.13); //NC
    setColor(BLACK);
    drawString("nc", WINDOW_WIDTH * 0.466, WINDOW_HEIGHT * 0.13);


    //Lathe
    chooseMColor(8);
    drawFilledBox(WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.85, WINDOW_WIDTH * 0.80, WINDOW_HEIGHT * 0.75); //Top Left
    setColor(BLACK);
    drawString("l1", WINDOW_WIDTH * 0.725, WINDOW_HEIGHT * .75);
    chooseMColor(9);
    drawFilledBox(WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.72, WINDOW_WIDTH * 0.80, WINDOW_HEIGHT * 0.62); //Mid Left
    setColor(BLACK);
    drawString("l2", WINDOW_WIDTH * 0.725, WINDOW_HEIGHT * .62);
    chooseMColor(10);
    drawFilledBox(WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.59, WINDOW_WIDTH * 0.80, WINDOW_HEIGHT * 0.49); //Bot Right
    setColor(BLACK);
    drawString("l3", WINDOW_WIDTH * 0.725, WINDOW_HEIGHT * .49);

    chooseMColor(11);
    drawFilledBox(WINDOW_WIDTH * 0.84, WINDOW_HEIGHT * 0.85, WINDOW_WIDTH * 0.99, WINDOW_HEIGHT * 0.75); //Top Right
    setColor(BLACK);
    drawString("l4", WINDOW_WIDTH * 0.915, WINDOW_HEIGHT * .75);
    chooseMColor(12);
    drawFilledBox(WINDOW_WIDTH * 0.84, WINDOW_HEIGHT * 0.72, WINDOW_WIDTH * 0.99, WINDOW_HEIGHT * 0.62); //Mid Right
    setColor(BLACK);
    drawString("l5", WINDOW_WIDTH * 0.915, WINDOW_HEIGHT * .62);
    chooseMColor(13);
    drawFilledBox(WINDOW_WIDTH * 0.84, WINDOW_HEIGHT * 0.59, WINDOW_WIDTH * 0.99, WINDOW_HEIGHT * 0.49); //Bot Right
    setColor(BLACK);
    drawString("l6", WINDOW_WIDTH * 0.915, WINDOW_HEIGHT * .49);

    //MIL
    chooseMColor(14);
    drawFilledBox(WINDOW_WIDTH * 0.48, WINDOW_HEIGHT * 0.97, WINDOW_WIDTH * 0.43, WINDOW_HEIGHT * 0.90); //Top Right
    setColor(BLACK);
    drawString("m1", WINDOW_WIDTH * .457, WINDOW_HEIGHT * .88);
    chooseMColor(15);
    drawFilledBox(WINDOW_WIDTH * 0.40, WINDOW_HEIGHT * 0.97, WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.90); //Top Mid
    setColor(BLACK);
    drawString("m2", WINDOW_WIDTH * .377, WINDOW_HEIGHT * .88);
    chooseMColor(17);
    drawFilledBox(WINDOW_WIDTH * 0.32, WINDOW_HEIGHT * 0.97, WINDOW_WIDTH * 0.27, WINDOW_HEIGHT * 0.90); //Top Left
    setColor(BLACK);
    drawString("m4", WINDOW_WIDTH * .297, WINDOW_HEIGHT * .88);

    chooseMColor(18);
    drawFilledBox(WINDOW_WIDTH * 0.48, WINDOW_HEIGHT * 0.80, WINDOW_WIDTH * 0.43, WINDOW_HEIGHT * 0.73); //Bot Right
    setColor(BLACK);
    drawString("m5", WINDOW_WIDTH * .457, WINDOW_HEIGHT * .71);
    chooseMColor(16);
    drawFilledBox(WINDOW_WIDTH * 0.40, WINDOW_HEIGHT * 0.80, WINDOW_WIDTH * 0.35, WINDOW_HEIGHT * 0.73); //Bot Left
    setColor(BLACK);
    drawString("m3", WINDOW_WIDTH * .377, WINDOW_HEIGHT * .71);

    //PMIL
    chooseMColor(20);
    drawFilledBox(WINDOW_WIDTH * 0.01, WINDOW_HEIGHT * 0.97, WINDOW_WIDTH * 0.07, WINDOW_HEIGHT * 0.82); //Top
    setColor(BLACK);
    drawString("p1", WINDOW_WIDTH * .042, WINDOW_HEIGHT * .82);
    chooseMColor(21);
    drawFilledBox(WINDOW_WIDTH * 0.01, WINDOW_HEIGHT * 0.80, WINDOW_WIDTH * 0.07, WINDOW_HEIGHT * 0.65); //Mid
    setColor(BLACK);
    drawString("p2", WINDOW_WIDTH * .042, WINDOW_HEIGHT * .65);
    chooseMColor(22);
    drawFilledBox(WINDOW_WIDTH * 0.01, WINDOW_HEIGHT * 0.63, WINDOW_WIDTH * 0.07, WINDOW_HEIGHT * 0.48); //Bot
    setColor(BLACK);
    drawString("p3", WINDOW_WIDTH * .042, WINDOW_HEIGHT * .48);


    //Flat & Circle
    chooseMColor(6);
    drawFilledBox(WINDOW_WIDTH * 0.21, WINDOW_HEIGHT * 0.80, WINDOW_WIDTH * 0.16, WINDOW_HEIGHT * 0.73); //Left
    setColor(BLACK);
    drawString("f1", WINDOW_WIDTH * .187, WINDOW_HEIGHT * .71);
    chooseMColor(7);
    drawFilledBox(WINDOW_WIDTH * 0.23, WINDOW_HEIGHT * 0.80, WINDOW_WIDTH * 0.32, WINDOW_HEIGHT * 0.73); //Right
    setColor(BLACK);
    drawString("f2", WINDOW_WIDTH * .275, WINDOW_HEIGHT * .71);
    chooseMColor(0);
    drawFilledBox(WINDOW_WIDTH * 0.225, WINDOW_HEIGHT * 0.97, WINDOW_WIDTH * 0.175, WINDOW_HEIGHT * 0.90);
    setColor(BLACK);
    drawString("ci", WINDOW_WIDTH * .20, WINDOW_HEIGHT * .88);

    setColor(WHITE);
    if (mouseState == 0) {
        drawTime(1465, 0);
    } else {
        drawExit();
    }
}

void drawModeTwo() {
    string allNames[50]; string temp;
    int machineNo;
    checkError = readMachines(allNames);
    if (checkError == -1) {
        mode = -1;
        return;
    }
    setColor(WHITE);
    drawLine(0, WINDOW_HEIGHT * 0, WINDOW_WIDTH, WINDOW_HEIGHT * 0);
    drawLine(0, WINDOW_HEIGHT * 0.2, WINDOW_WIDTH, WINDOW_HEIGHT * 0.2);
    drawLine(0, WINDOW_HEIGHT * 0.4, WINDOW_WIDTH, WINDOW_HEIGHT * 0.4);
    drawLine(0, WINDOW_HEIGHT * 0.6, WINDOW_WIDTH, WINDOW_HEIGHT * 0.6);
    drawLine(0, WINDOW_HEIGHT * 0.8, WINDOW_WIDTH, WINDOW_HEIGHT * 0.8);
    drawLine(0, WINDOW_HEIGHT * 1, WINDOW_WIDTH, WINDOW_HEIGHT * 1);
    //drawLine(WINDOW_WIDTH * 0.5, 0, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT);
    drawTime(WINDOW_WIDTH * 0.3825, 0);
    for (int x = 0; x < 4; x++) {
        machineNo = (rotation * 4) + (3 - x);
        if (machineNo < 23) {
            setColor(WHITE);
            drawString(allNames[machineNo], WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * ((0.2 * x) + 0.3));

            drawString("current", WINDOW_WIDTH * 0.3, WINDOW_HEIGHT * ((0.2 * x) + 0.3));
            temp = getTime(currentCount[machineNo]);
            drawString(temp, WINDOW_WIDTH * 0.3, WINDOW_HEIGHT * ((0.2 * x) + 0.225));

            drawString("on", WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * ((0.2 * x) + 0.3));
            temp = getTime(machineSignals[machineNo][0]);
            drawString(temp, WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * ((0.2 * x) + 0.225));

            drawString("idle", WINDOW_WIDTH * 0.60, WINDOW_HEIGHT * ((0.2 * x) + 0.3));
            temp = getTime(machineSignals[machineNo][1]);
            drawString(temp, WINDOW_WIDTH * 0.60, WINDOW_HEIGHT * ((0.2 * x) + 0.225));

            drawString("off", WINDOW_WIDTH * 0.75, WINDOW_HEIGHT * ((0.2 * x) + 0.3));
            temp = getTime(machineSignals[machineNo][2]);
            drawString(temp, WINDOW_WIDTH * 0.75, WINDOW_HEIGHT * ((0.2 * x) + 0.225));

            chooseMColor(machineNo);
            drawFilledCircle(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * ((0.2 * x) + 0.275), 50);
        }
    }
}

void drawModeThree(){
    string allNames[50]; string temp;
    int machineNo;
    checkError = readMachines(allNames);
    if (checkError == -1) {
        mode = -1;
        return;
    }
    for(int x=0; x<23; x++){
        machineNo=22-x;
        chooseMColor(machineNo);
        drawFilledBox(WINDOW_WIDTH*0.25,WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+2)/24,WINDOW_WIDTH*0.35,WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+1)/24);

        setColor(WHITE);
        drawLine(0,WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+1)/24,WINDOW_WIDTH,WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+1)/24);

        //setColor(BLACK);
        drawString(allNames[machineNo], WINDOW_WIDTH*0.1, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);

        setColor(WHITE);
        temp = getTime(currentCount[machineNo]);
        drawString(temp, WINDOW_WIDTH * 0.45, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);

        temp = getTime(machineSignals[machineNo][0]);
        drawString(temp, WINDOW_WIDTH * 0.60, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);

        temp = getTime(machineSignals[machineNo][1]);
        drawString(temp, WINDOW_WIDTH * 0.75, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);

        temp = getTime(machineSignals[machineNo][2]);
        drawString(temp, WINDOW_WIDTH * 0.90, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);
    }
    temp=getFileNameG().substr(10,6);
    //cout << temp << endl;
    string newtemp=temp.substr(0,2)+":"+temp.substr(2,2)+":"+temp.substr(4,2);
    //cout << newtemp << endl;
    machineNo=-1;
    drawString(newtemp,WINDOW_WIDTH*0.1,WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);
    drawString("signal", WINDOW_WIDTH * 0.3, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);
    drawString("current", WINDOW_WIDTH * 0.45, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);
    drawString("on", WINDOW_WIDTH * 0.60, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);
    drawString("idle", WINDOW_WIDTH * 0.75, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);
    drawString("off", WINDOW_WIDTH * 0.90, WINDOW_HEIGHT-WINDOW_HEIGHT*float(machineNo+3)/24);
}

void drawModeQuit() {
    setColor(WHITE);
    drawFilledBox(WINDOW_WIDTH * 0.329, WINDOW_HEIGHT * 0.327, WINDOW_WIDTH * 0.661, WINDOW_HEIGHT * 0.663);
    setColor(GRAY);
    drawFilledBox(WINDOW_WIDTH * 0.33, WINDOW_HEIGHT * 0.33, WINDOW_WIDTH * 0.66, WINDOW_HEIGHT * 0.66);
    setColor(WHITE);
    drawString("exit the program?", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5);
    drawString("y/n", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.4);
}

void drawModeHelp() {
    float sub = 1;
    setColor(GRAY);
    drawFilledBox(WINDOW_WIDTH * 0.24, 0, WINDOW_WIDTH * 0.75, WINDOW_HEIGHT * sub);
    setColor(WHITE);
    sub -= 0.1;
    drawString("help", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.15;
    drawString("c - clear flashing", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    //drawString("d - show drawing number", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    //sub -= 0.05;
    drawString("f - toggle fullscreen mode", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    drawString("h - toggle help mode", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    drawString("i - toggle information (all)", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    drawString("k - toggle keyboard mode", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    //drawString("m - toggle viewing mode", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    //sub -= 0.05;
    drawString("q - quit", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    drawString("r - reset everything", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    drawString("1-5 - change states", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    drawString("left click - toggle information (one)", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
    sub -= 0.05;
    drawString("right click - toggle machine details", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * sub);
}

void drawModeError() {
    setColor(GRAY);
    drawFilledBox(WINDOW_WIDTH * 0, WINDOW_HEIGHT * 0.6, WINDOW_WIDTH * 1, WINDOW_HEIGHT * 0.4);
    setColor(WHITE);
    drawString("error with opening or closing files", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5);
    drawString("check server connection and restart the program", WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.4);
}