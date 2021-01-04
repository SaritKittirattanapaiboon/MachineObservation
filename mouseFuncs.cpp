#include "csvgraphics.h"
#include "overlap.h"

int getColumn(int xpos) {
    int i = 0;
    int width = glutGet(GLUT_WINDOW_WIDTH);
    while (i * (width / 4) < xpos) {
        i++;
    }
    return i - 1;
}

int getRow(int ypos) {
    int i = 0;
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    while (i * (height / 6) < ypos) {
        i++;
    }
    return i - 1;
}

void quitKey(unsigned char key, int x, int y) {
    if (mode == -1) {
        ofstream closePLC;
        closePLC.open("togo.txt");
        closePLC.close();
        exit(0);
    }
    if (key == 'c' || key == 'C') {
        int xpos = getColumn(x);
        int ypos = getRow(y);
        int boxNum = (ypos * 4) + xpos;
        if (mode == 0 && !keyboardArrow) {
            if (currentCount[boxNum] > 600 && inSignals[boxNum] == 2) {
                clear[boxNum] = !clear[boxNum];
            }
        } else if (mode == 0 && keyboardArrow) {
            if (currentCount[selectBox] > 600 && inSignals[selectBox] == 2) {
                clear[selectBox] = !clear[selectBox];
            }
        }
    } else if (key == 'd' || key == 'D') {
        if (mode != 100 && mode != 101 && selectBox != 23) {
            state[selectBox] = 4;
        } else if (!keyboardArrow && mode == 0) {
            for (int i = 0; i < machineNums; i++) {
                if (state[i] != 4) {
                    state[i] = 4;
                } else {
                    state[i] = 0;
                }
            }
        }
        glutPostRedisplay();
    } else if (key == 'e' || key == 'E') {
        //mode=-1;
        //glutPostRedisplay();
    } else if (key == 'f' || key == 'F') {
        if (mode != 100) {
            if (fullscreen) {
                glutReshapeWindow(1080, 720);
                glutPositionWindow(250, 50);
                glutPostRedisplay();
                fullscreen = false;
            } else {
                glutFullScreen();
                fullscreen = true;
                glutPostRedisplay();
            }
        }
    } else if (key == 'h' || key == 'H') {
        if (mode != 100) {
            if (mode != 101) {
                prevMode = mode;
                mode = 101;
            } else {
                mode = prevMode;
            }
        }
        glutPostRedisplay();
    } else if (key == 'i' || key == 'I') {
        if (mode == 0 && !keyboardArrow) {
            for (int i = 0; i < machineNums; i++) {
                state[i] = (state[i] + 1) % 6;
                defaultState[i] = state[i];
            }
        } else if (mode == 0 && keyboardArrow) {
            state[selectBox] = (state[selectBox] + 1) % 7;
            defaultState[selectBox] = state[selectBox];
        }
        glutPostRedisplay();
    } else if (key == 'k' || key == 'K') {
        selectBox = 23;
        keyboardArrow = !keyboardArrow;
        glutPostRedisplay();
    } else if (key == 'm' || key == 'M') {
        /*
        if (mode != 100 && mode != 101) {
            mouseState = 0;
            prevMode = mode;
            mode = (mode + 1) % 4;
            glutPostRedisplay();
        }
         */
    } else if (key == 'n' || key == 'N') {
        if (mode == 100) {
            mode = prevMode;
            glutPostRedisplay();
        }
    } else if (key == 'q' || key == 'Q') {
        if (mode == 101) {
            mode = 100;
            glutPostRedisplay();
        } else if (mode == 100) {
            mode = prevMode;
            glutPostRedisplay();
        } else {
            prevMode = mode;
            mode = 100;
            glutPostRedisplay();
        }
    } else if (key == 'r' || key == 'R') {
        if (mode != 100) {
            mode = 0;
            mouseState = 0;
            selectBox = 23;
            for (int i = 0; i < machineNums; i++) {
                state[i] = 0;
                clear[i] = true;
                defaultState[i] = 0;
            }
            keyboardArrow = false;
            glutPostRedisplay();
        }
    } /*else if (key == 't' || key == 'T') {
        if (mode == 0) {
            int xpos = getColumn(x);
            int ypos = getRow(y);
            int boxNum = (ypos * 4) + xpos;
            if (mode == 0 && !keyboardArrow) {
                currentCount[boxNum] += 60;
            }
            glutPostRedisplay();
        }
    } */else if (key == 'y' || key == 'Y') {
        if (mode == 100) {
            if(plcComm) {
                save();
                ofstream closePLC;
                closePLC.open("togo.txt");
                closePLC.close();
            }
            exit(0);
        }

    } else if (key == '1') {
        mouseState = 0;
        if (mode != 100 && selectBox != 23) {
            state[selectBox] = 0;
        } else if (!keyboardArrow && mode == 0) {
            for (int i = 0; i < machineNums; i++) {
                state[i] = 0;
                defaultState[i] = 0;
            }
        }
        if (mode == 0 && keyboardArrow) {
            defaultState[selectBox] = 0;
        }
        glutPostRedisplay();
    } else if (key == '2') {
        mouseState = 0;
        if (mode != 100 && selectBox != 23) {
            state[selectBox] = 1;
        } else if (!keyboardArrow && mode == 0) {
            for (int i = 0; i < machineNums; i++) {
                state[i] = 1;
                defaultState[i] = 1;
            }
        }
        if (mode == 0 && keyboardArrow) {
            defaultState[selectBox] = 1;
        }
        glutPostRedisplay();
    } else if (key == '3') {
        mouseState = 0;
        if (mode != 100 && selectBox != 23) {
            state[selectBox] = 2;
        } else if (!keyboardArrow && mode == 0) {
            for (int i = 0; i < machineNums; i++) {
                state[i] = 2;
                defaultState[i] = 2;
            }
        }
        if (mode == 0 && keyboardArrow) {
            defaultState[selectBox] = 2;
        }
        glutPostRedisplay();
    } else if (key == '4') {
        mouseState = 0;
        if (mode != 100 && selectBox != 23) {
            state[selectBox] = 3;
        } else if (!keyboardArrow && mode == 0) {
            for (int i = 0; i < machineNums; i++) {
                state[i] = 3;
                defaultState[i] = 3;
            }
        }
        if (mode == 0 && keyboardArrow) {
            defaultState[selectBox] = 3;
        }
        glutPostRedisplay();
    } else if (key == '5') {
        mouseState = 0;
        if (mode != 100 && selectBox != 23) {
            state[selectBox] = 4;
        } else if (!keyboardArrow && mode == 0) {
            for (int i = 0; i < machineNums; i++) {
                state[i] = 4;
                defaultState[i] = 4;
            }
        }
        if (mode == 0 && keyboardArrow) {
            defaultState[selectBox] = 4;
        }
        glutPostRedisplay();
    } else if (key == '6') {
        mouseState = 0;
        if (mode != 100 && selectBox != 23) {
            state[selectBox] = 5;
        } else if (!keyboardArrow && mode == 0) {
            for (int i = 0; i < machineNums; i++) {
                state[i] = 5;
                defaultState[i] = 5;
            }
        }
        if (mode == 0 && keyboardArrow) {
            defaultState[selectBox] = 5;
        }
        glutPostRedisplay();
    }
    //cout << "Current Mode:" << mode << " Previous Mode:" << prevMode << endl;
}

void specialKeyboard(int key, int x, int y) {
    if (mode == -1) {
        ofstream closePLC;
        closePLC.open("togo.txt");
        closePLC.close();
        exit(0);
    }
    if (mode == 0 && keyboardArrow) {
        if(mode == 0) {
            if (key == GLUT_KEY_RIGHT) {
                if (selectBox < 23) {
                    selectBox++;
                }
            } else if (key == GLUT_KEY_LEFT) {
                if (selectBox > 0) {
                    selectBox--;
                }
            } else if (key == GLUT_KEY_UP) {
                selectBox = (selectBox - 4);
                if (selectBox < 0) {
                    selectBox = selectBox + 4;
                }
            } else if (key == GLUT_KEY_DOWN) {
                selectBox = (selectBox + 4);
                if (selectBox > 23) {
                    selectBox = selectBox - 4;
                }
            }
        }
    }
    else if(mode == 2){
        if (key == GLUT_KEY_UP) {
            if(rotation!=0){
                rotation--;
                reset=true;
            }
        } else if (key == GLUT_KEY_DOWN) {
            if(rotation!=5){
                rotation++;
                reset=true;
            }
        }
        //cout << rotation << endl;
    }
    glutPostRedisplay();

}

void mousefunc(int button, int upDown, int xpos, int ypos) {
    if (mode == -1) {
        ofstream closePLC;
        closePLC.open("togo.txt");
        closePLC.close();
        exit(0);
    }
    int x = getColumn(xpos);
    int y = getRow(ypos);
    int boxNum = (y * 4) + x;
    if (button == GLUT_LEFT_BUTTON && upDown == GLUT_DOWN) {
        if (boxNum == 23) {
            if (mouseState == 0) {
                mouseState = 1;
            } else {
                if(plcComm) {
                    save();
                    ofstream closePLC;
                    closePLC.open("togo.txt");
                    closePLC.close();
                }
                exit(0);
            }
        } else {
            if (mode == 0) {
                mouseState = 0;
                if (state[boxNum] > 5) {
                    state[boxNum] = defaultState[boxNum];
                } else {
                    state[boxNum] = (state[boxNum] + 1) % 6;
                }
                defaultState[boxNum] = state[boxNum];
            }
        }
    }
    if (button == GLUT_RIGHT_BUTTON && upDown == GLUT_DOWN) {
        if (boxNum == 23) {
            if (mouseState == 0) {
                mouseState = 1;
            } else {
                if(plcComm) {
                    save();
                    ofstream closePLC;
                    closePLC.open("togo.txt");
                    closePLC.close();
                }

                exit(0);
            }
        } else {
            if (mode == 0) {
                if (state[boxNum] < 6) {
                    //state[boxNum] = 5;
                    string ClientName = "MachineInfo\\" + machinePaths[boxNum];
                    //cout << ClientName << endl;
                    //cout << "Test" << endl;
                    ShellExecute(NULL, "open", ClientName.c_str(), NULL, NULL, SW_SHOWMINIMIZED);
                }
            }
            mouseState = 0;
        }
    }
}