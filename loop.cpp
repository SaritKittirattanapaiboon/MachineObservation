#include "csvgraphics.h"
#include "overlap.h"

int getSignals() {
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
        inSignals[i] = stoi(temp);
        if (prevSignals[i] != inSignals[i]) {
            if (prevSignals[i] == 2 && currentCount[i] > 300) {
                state[i] = defaultState[i];
                blink[i] = 0;
            }
            if (prevSignals[i] == 1 && currentCount[i] > 1800){
                state[i] = defaultState[i];
                blink[i] = 0;
            }
            if (inSignals[i] == 2 || inSignals[i] == 1) {
                clear[i] = true;
            }
            currentCount[i] = 0;
        }
        i++;
        while (i < machineNums) {
            getline(input, temp, '\n');
            inSignals[i] = stoi(temp);
            if (prevSignals[i] != inSignals[i]) {
                if (prevSignals[i] == 2 && currentCount[i] > 300) {
                    state[i] = defaultState[i];
                    blink[i] = 0;
                }
                if (prevSignals[i] == 1 && currentCount[i] > 1800){
                    state[i] = defaultState[i];
                    blink[i] = 0;
                }
                if (inSignals[i] == 2 || inSignals[i] == 1) {
                    clear[i] = true;
                }
                currentCount[i] = 0;
            }
            i++;
        }
    }
    i = 0;
    while (i < machineNums) {
        prevSignals[i] = inSignals[i];
        i++;
    }
    input.close();
    return 0;
}

void timer(int resetRotation) {
    if (mode != -1) {
        graphicsTime = stoi(getFileNameG().substr(10, 6));
        if (graphicsTime - graphicsTimePrev != 0) {
            if(plcComm) {
                checkError = data_write();
            }
            else{
                readCurrent();
                readWeekly();
                readMonthly();
                readYearly();
                //readcsv();
            }
            if (checkError == -1) {
                mode = -1;
                glutPostRedisplay();
            }
            for (int i = 0; i < machineNums; i++) {
                currentCount[i]++;
                if (currentCount[i] > 600 && inSignals[i] == 2 && clear[i]) {
                    //state[i] = 1;
                    /*
                    if (blink[i] > 19) {
                        blink[i] = -279;
                    } else {
                        blink[i] = blink[i] + 1;
                    }
                     */
                }
            }
            string checkingPLC;
            ifstream checkPLC;
            checkPLC.open("togo.txt");
            if (!checkPLC.is_open()) {
                mode = -1;
                glutPostRedisplay();
            }
            while (!checkPLC.eof()) {
                getline(checkPLC, checkingPLC, '\n');
            }
            if (checkingPLC.empty()) {
                cout << "The program closed by another computer. Press Enter to exit the program" << endl;
                ofstream closePLC;
                closePLC.open("togo.txt");
                closePLC.close();
                system("pause");
                exit(0);
            }
            checkPLC.close();

            checkError = getSignals();
            if (checkError == -1) {
                mode = -1;
                glutPostRedisplay();
            }
            checkError = getData();
            if (checkError == -1) {
                mode = -1;
                glutPostRedisplay();
            }
            checkError = getDrawing();
            if (checkError == -1) {
                mode = -1;
                glutPostRedisplay();
            }
            glutPostRedisplay();
            graphicsTimePrev = graphicsTime;
        }
        if (mode == 2) {
            if (reset) {
                resetRotation = 0;
                reset = false;
            }
            if (resetRotation == 9300) {
                rotation = (rotation + 1) % 6;
                resetRotation = 0;
            } else {
                resetRotation = resetRotation + 10;
            }
        }
    }
    glutTimerFunc(10, timer, resetRotation);
}

void drawScene() {
    int rownum = 6;
    int colnum = 4;
    int i = 0;
    int j = 0;
    int machineNo;

    int centerx, centery;
    string temp;

    i = 0;
    if (mode == 0) {
        clearWindow();
        while (i < rownum) {
            j = 0;
            while (j < colnum) {
                machineNo = (i * colnum) + j;
                centerx = (j * WINDOW_WIDTH * 0.25) + (WINDOW_WIDTH * 0.125);
                centery = WINDOW_HEIGHT - (i * WINDOW_HEIGHT * .166666) - (WINDOW_HEIGHT * 0.083333);
                if (machineNo == selectBox && keyboardArrow) {
                    if (machineNo != 23) {
                        setColor(MIDNIGHT_BLUE);
                        drawFilledBox((j) * WINDOW_WIDTH * 0.25, (rownum - i) * WINDOW_HEIGHT * 0.16666,
                                      (j + 1) * WINDOW_WIDTH * 0.25, (rownum - i - 1) * WINDOW_HEIGHT * 0.16666);
                    } else {
                        setColor(CYAN);
                        drawFilledCircle((j * WINDOW_WIDTH * 0.25) + 20, ((rownum - i) * WINDOW_HEIGHT * 0.16666) - 20,
                                         10);
                    }
                }
                drawModeZero(machineNo, centerx, centery);
                setColor(WHITE);
                drawLine(j * WINDOW_WIDTH * 0.25, WINDOW_HEIGHT, j * WINDOW_WIDTH * 0.25, 0);
                j++;
            }
            drawLine(0, i * WINDOW_HEIGHT * .166666, WINDOW_WIDTH, i * WINDOW_HEIGHT * .166666);
            i++;
        }
    } else if (mode == 1) {
        clearWindow();
        drawModeOne();
    } else if (mode == 2) {
        clearWindow();
        drawModeTwo();
    } else if (mode == 3) {
        clearWindow();
        drawModeThree();
    } else if (mode == 100) {
        drawModeQuit();
    } else if (mode == 101) {
        drawModeHelp();
    } else if (mode == -1) {
        drawModeError();
    }
    glEnd();
    glFlush();
    glutSwapBuffers();
}

size_t findPos(const string full, string find, int num) {
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

size_t findLast(const string full, string find) {
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

size_t findLastN(const string full, string find, int fromBack) {
    int len = full.length();
    string check;
    while (len > 0) {
        check = full.substr(len, 1);
        if (find == check) {
            fromBack = fromBack - 1;

        }
        if (fromBack == 0) {
            break;
        }
        len--;
    }
    return len;
}

void chooseMColor(int macNo) {
    if(machineInUse[macNo]==0){
        setColor(GRAY);
        return;
    }
    else {
        if (inSignals[macNo] == 0) {
            setColor(GREEN);
        } else if (inSignals[macNo] == 1) {
            setColor(YELLOW);
        } else {
            setColor(RED);
        }
    }
}

int getSavedDataG(string* month, int* day) {
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
        i++;
    }
    saved.close();
    return num;
}