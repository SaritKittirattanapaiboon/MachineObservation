#include "csvgraphics.h"

double PI = acos(-1.0);
double ANGLE_STEP = PI/180.0;

int readMachines(string machineNames[]) {
    int i;
    ifstream machineFile;
    machineFile.open("Machines.csv");
    if (!machineFile.is_open()) {
        return -1;
    }
    string input;
    string temp;
    size_t posi, posf;
    getline(machineFile, input, '\n');
    istringstream readData(input);
    getline(readData, temp, ',');
    //machineNum=20; //For whatever unholy reason the stoi function isn't working, temp becomes a string of length 5 that shows "20" but has 5 empty substrings

    i = 0;
    while (i < machineNums) {
        getline(machineFile, input, '\n');
        posi = findPosG(input, ",", 2);
        posf = findPosG(input, ",", 3);
        machineNames[i] = input.substr(posi + 1, posf - posi - 1);
        i++;
    }
    machineNames[10] = "cnc-3";
    machineNames[11] = "cnc-4";
    machineNames[12] = "cnc-5";
    machineNames[13] = "nc-1";

    return machineNums;
}

int readMachineUsed(){
    string temp;
    int loop=0;
    ifstream machineUsed;
    machineUsed.open("usingMachine.txt");
    if(!machineUsed.is_open()){
        return -1;
    }
    while(loop<machineNums){
        getline(machineUsed,temp,'\n');
        machineInUse[loop]=stoi(temp.substr(0,1));
        loop++;
    }
    return 0;
}

void setColor(double red, double green, double blue) {
    glColor3d(red, green, blue);
}

void graphicsSetup(int argc, char **argv) {
    glutInit(&argc, argv);
    RECT rect;
    HWND consoleWindow = GetConsoleWindow();
    GetWindowRect(consoleWindow, &rect);
    //std::cout << rect.right-rect.left << " " << rect.bottom-rect.top << std::endl;
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    //glutInitWindowPosition(WINDOW_X,WINDOW_Y);
    glutInitWindowSize(1080, 720);
    glutInitWindowPosition(250, 50);
    glutCreateWindow("Machine Observation");
    glClearColor(GREY, 0.0);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glutKeyboardFunc(quitKey);
    glutMouseFunc(mousefunc);
    glutSpecialFunc(specialKeyboard);
    //glutFullScreen();
}

void clearWindow() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    glBegin(GL_LINE_STRIP);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glVertex2i(x1, y1);
    glEnd();
    glFlush();
}

void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    glBegin(GL_POLYGON);
    glVertex2i(x1,y1);
    glVertex2i(x2,y2);
    glVertex2i(x3,y3);
    glVertex2i(x1,y1);
    glEnd();
    glFlush();
}

void drawLine(int x1, int y1, int x2, int y2) {
    glBegin(GL_LINE_STRIP);
    glVertex2i(x1,y1);
    glVertex2i(x2,y2);
    glEnd();
    glFlush();
}

void drawBox(int x1, int y1, int x2, int y2) {
    glBegin(GL_LINE_STRIP);
    glVertex2i(x1,y1);
    glVertex2i(x2,y1);
    glVertex2i(x2,y2);
    glVertex2i(x1,y2);
    glVertex2i(x1,y1);
    glEnd();
    glFlush();
}

void drawFilledBox(int x1, int y1, int x2, int y2) {
    glBegin(GL_POLYGON);
    glVertex2i(x1,y1);
    glVertex2i(x2,y1);
    glVertex2i(x2,y2);
    glVertex2i(x1,y2);
    glVertex2i(x1,y1);
    glEnd();
    glFlush();
}

void drawCircle(int x1, int y1, int radius) {
    double angle;
    int X, Y;
    glBegin(GL_LINE_STRIP);
    for (angle=0;angle< 2.0*PI + ANGLE_STEP; angle += ANGLE_STEP) {
        X = x1 + int(double(radius) * cos(angle));
        Y = y1 + int(double(radius) * sin(angle));
        glVertex2i(X,Y);
    }
    glEnd();
    glFlush();
}

void drawFilledCircle(int x1, int y1, int radius) {
    double angle;
    int X0, Y0, X1, Y1;
    glBegin(GL_TRIANGLES);
    X1 = x1 + radius;
    Y1 = y1;
    for (angle = 0; angle < 2.0 * PI + ANGLE_STEP; angle += ANGLE_STEP) {
        X0 = X1;
        Y0 = Y1;
        X1 = x1 + int(double(radius) * cos(angle));
        Y1 = y1 + int(double(radius) * sin(angle));
        glVertex2i(x1, y1);
        glVertex2i(X0, Y0);
        glVertex2i(X1, Y1);
    }
    glEnd();
    glFlush();
}

void drawLetter(char x, int strIniX, int strIniY) {
    //Box of 50x50, bottom left coordinate is (384*(numMachine%5)+(50*charNum), 1080-(int(numMachine/5)*270)-60)
    if (x >= 'a' && x <= 'j') {
        drawLetterAJ(x, strIniX, strIniY);
    } else if (x >= 'k' && x <= 't') {
        drawLetterKT(x, strIniX, strIniY);
    } else if (x >= 'u' && x <= 'z') {
        drawLetterUZ(x, strIniX, strIniY);
    } else if (x >= '0' && x <= '9') {
        drawNumber(x, strIniX, strIniY);
    } else {
        drawSymbol(x, strIniX, strIniY);
    }
}

void drawString(string machineName, int centerx, int centery) {
    int charNum = 0;
    char c;
    int strIniX, strIniY;
    if (machineName.length() % 2 == 0) {
        strIniX = centerx - (25 * (machineName.length() / 2));
        strIniY = centery + 50;
    } else {
        strIniX = centerx - 10 - (25 * int(machineName.length() / 2));
        strIniY = centery + 50;
    }
    strIniX -= 5;
    while (charNum < machineName.length()) {
        c = machineName[charNum];
        //cout << c << " " << strIniX << " " << strIniY << endl;
        if (c == '-') {
            drawDash(strIniX, strIniY);
        } else if (c == ':') {
            drawColon(strIniX, strIniY);
        } else {
            drawLetter(c, strIniX, strIniY);
        }
        strIniX += 25;
        charNum++;
    }
}

void drawDash(int strIniX, int strIniY) {
    drawLine(strIniX + 5, strIniY + 15, strIniX + 25, strIniY + 15);
}

void drawColon(int strIniX, int strIniY) {
    drawFilledCircle(strIniX + 15, strIniY + 10, 3);
    drawFilledCircle(strIniX + 15, strIniY + 20, 3);
}

void drawNumber(char x, int strIniX, int strIniY) {
    if (x == '1') {
        drawLine(strIniX + 15, strIniY + 5, strIniX + 15, strIniY + 25); //Middle Vertical Line
    } else if (x == '2') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 15); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 15); //Right Vertical Line
    } else if (x == '3') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == '4') {
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 15, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == '5') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 15, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == '6') {
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == '7') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == '8') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == '9') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 15, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == '0') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    }
}

void drawLetterAJ(char x, int strIniX, int strIniY) {
    if (x == 'a') {
        drawLine(strIniX + 5, strIniY + 5, strIniX + 15, strIniY + 25); //Diagonal Positive
        drawLine(strIniX + 25, strIniY + 5, strIniX + 15, strIniY + 25); //Diagonal Negative
        drawLine(strIniX + 10, strIniY + 15, strIniX + 20, strIniY + 15);
    } else if (x == 'b') {
        drawLine(strIniX + 5, strIniY + 25, strIniX + 25, strIniY + 20); //Top Horizontal Line
        drawLine(strIniX + 5, strIniY + 15, strIniX + 25, strIniY + 20); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 15, strIniX + 25, strIniY + 10); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 25, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
    } else if (x == 'c') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
    } else if (x == 'd') {
        drawLine(strIniX + 5, strIniY + 25, strIniX + 25, strIniY + 15);
        drawLine(strIniX + 5, strIniY + 5, strIniX + 25, strIniY + 15);
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25);
    } else if (x == 'e') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
    } else if (x == 'f') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
    } else if (x == 'g') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 20, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == 'h') {
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == 'i') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 15, strIniY + 5, strIniX + 15, strIniY + 25); //Middle Vertical Line
    } else if (x == 'j') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 15, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 15, strIniY + 5, strIniX + 15, strIniY + 25); //Middle Vertical Line
    }
}

void drawLetterKT(char x, int strIniX, int strIniY) {
    if (x == 'k') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 15); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 15); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
    } else if (x == 'l') {
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
    } else if (x == 'm') {
        drawLine(strIniX + 15, strIniY + 15, strIniX + 5, strIniY + 25);
        drawLine(strIniX + 15, strIniY + 15, strIniX + 25, strIniY + 25);
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == 'n') {
        drawLine(strIniX + 5, strIniY + 25, strIniX + 25, strIniY + 5); //Top Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == 'o') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == 'p') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 15); //Right Vertical Line
    } else if (x == 'q') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
        drawLine(strIniX + 16, strIniY + 12, strIniX + 28, strIniY - 4);
    } else if (x == 'r') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 15); //Right Vertical Line
        drawLine(strIniX + 5, strIniY + 15, strIniX + 25, strIniY + 5);
    } else if (x == 's') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 20); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 10, strIniX + 5, strIniY + 20); //Middle Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 25, strIniY + 10); //Bottom Horizontal Line
    } else if (x == 't') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 15, strIniY + 25, strIniX + 15, strIniY + 5); //Right Vertical Line
    }
}

void drawLetterUZ(char x, int strIniX, int strIniY) {
    if (x == 'u') {
        drawLine(strIniX + 25, strIniY + 5, strIniX + 5, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 5); //Right Vertical Line
    } else if (x == 'v') {
        drawLine(strIniX + 15, strIniY + 5, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 15, strIniY + 5, strIniX + 25, strIniY + 25); //Right Vertical Line
    } else if (x == 'w') {
        drawLine(strIniX + 5, strIniY + 25, strIniX + 10, strIniY + 5); //Left Vertical Line
        drawLine(strIniX + 10, strIniY + 5, strIniX + 15, strIniY + 25); //Right Vertical Line
        drawLine(strIniX + 15, strIniY + 25, strIniX + 20, strIniY + 5); //Left Vertical Line
        drawLine(strIniX + 20, strIniY + 5, strIniX + 25, strIniY + 25); //Right Vertical Line
    } else if (x == 'x') {
        drawLine(strIniX + 5, strIniY + 5, strIniX + 25, strIniY + 25); //Diagonal Positive
        drawLine(strIniX + 5, strIniY + 25, strIniX + 25, strIniY + 5); //Diagonal Negative
    } else if (x == 'y') {
        drawLine(strIniX + 15, strIniY + 15, strIniX + 5, strIniY + 25); //Left Vertical Line
        drawLine(strIniX + 15, strIniY + 15, strIniX + 25, strIniY + 25); //Right Vertical Line
        drawLine(strIniX + 15, strIniY + 15, strIniX + 15, strIniY + 5); //Left Vertical Line
    } else if (x == 'z') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 25, strIniY + 5); //Bottom Horizontal Line
        drawLine(strIniX + 5, strIniY + 5, strIniX + 25, strIniY + 25); //Left Vertical Line
    }
}

void drawSymbol(char x, int strIniX, int strIniY) {
    if (x == '?') {
        drawLine(strIniX + 25, strIniY + 25, strIniX + 5, strIniY + 25); //Top Horizontal Line
        drawLine(strIniX + 25, strIniY + 25, strIniX + 25, strIniY + 15); //Right Vertical Line
        drawLine(strIniX + 25, strIniY + 15, strIniX + 15, strIniY + 15); //Middle Horizontal Line
        drawLine(strIniX + 15, strIniY + 10, strIniX + 15, strIniY + 15); //Middle Vertical Line
        drawFilledBox(strIniX + 13, strIniY + 5, strIniX + 17, strIniY + 7); //Dot
    } else if (x == '/') {
        drawLine(strIniX + 5, strIniY + 5, strIniX + 25, strIniY + 25);
    } else if (x == '(' || x == '<') {
        drawLine(strIniX + 5, strIniY + 15, strIniX + 25, strIniY + 25);
        drawLine(strIniX + 5, strIniY + 15, strIniX + 25, strIniY + 5);
    } else if (x == ')' || x == '>') {
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 25);
        drawLine(strIniX + 25, strIniY + 15, strIniX + 5, strIniY + 5);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////
string getFileNameG() {
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

string getTime(int thisTime) {
    int hour;
    int min;
    int sec;
    string hourS, minS, secS;
    string temp;

    hour = thisTime / 3600;
    hourS = to_string(hour);
    min = (thisTime / 60) - (hour * 60);
    minS = to_string(min);
    sec = thisTime - (hour * 3600) - (min * 60);
    secS = to_string(sec);
    if (hourS.length() == 1) {
        hourS.insert(0, "00");
    } else if (hourS.length() == 2) {
        hourS.insert(0, "0");
    }
    if (minS.length() == 1) {
        minS.insert(0, "0");
    }
    if (secS.length() == 1) {
        secS.insert(0, "0");
    }
    temp.append(hourS);
    temp.append(":");
    temp.append(minS);
    temp.append(":");
    temp.append(secS);
    return temp;
}

int getData() {
    ifstream input;
    input.open("today.txt");
    if (!input.is_open()) {
        return -1;
    }
    int i = 0;
    string temp;
    string data;
    getline(input, temp, '\n');
    if (temp.empty()) {
    } else {
        istringstream reading(temp);
        getline(reading, data, ',');
        signalOn[0] = stoi(data);
        getline(reading, data, ',');
        signalIdle[0] = stoi(data);
        getline(reading, data, '\n');
        signalOff[0] = stoi(data);
        i++;
        while (i < machineNums) {
            getline(input, temp, '\n');
            istringstream reading2(temp);
            getline(reading2, data, ',');
            signalOn[i] = stoi(data);
            getline(reading2, data, ',');
            signalIdle[i] = stoi(data);
            getline(reading2, data, '\n');
            signalOff[i] = stoi(data);
            i++;
        }
    }
    return 0;
}

void drawTime(int strIniX, int strIniY) {
    curtime = getFileNameG().substr(10, 6);
    drawBigNumber(curtime[0], strIniX, strIniY);
    drawBigNumber(curtime[1], strIniX + 70, strIniY);
    drawBigColon(strIniX + 140, strIniY - 5);
    drawBigNumber(curtime[2], strIniX + 155, strIniY);
    drawBigNumber(curtime[3], strIniX + 225, strIniY);
    drawBigColon(strIniX + 295, strIniY - 5);
    drawBigNumber(curtime[4], strIniX + 310, strIniY);
    drawBigNumber(curtime[5], strIniX + 380, strIniY);

    string month = getMonth(getFileNameG().substr(2, 3));
    curtime = getFileNameG().substr(0, 2);
    drawBigNumber(curtime[0], strIniX, strIniY + 90);
    drawBigNumber(curtime[1], strIniX + 70, strIniY + 90);
    drawLine(strIniX + 145, strIniY + 100, strIniX + 150, strIniY + 170);
    drawBigNumber(month[0], strIniX + 155, strIniY + 90);
    drawBigNumber(month[1], strIniX + 225, strIniY + 90);
    drawLine(strIniX + 300, strIniY + 100, strIniX + 305, strIniY + 170);
    curtime = getFileNameG().substr(7, 2);
    drawBigNumber(curtime[0], strIniX + 310, strIniY + 90);
    drawBigNumber(curtime[1], strIniX + 380, strIniY + 90);
}

void drawExit() {
    int strIniX = 1440 + 20;
    int strIniY = 0;
    //Draw E
    drawLine(strIniX + 20, strIniY + 40, strIniX + 20, strIniY + 160);
    drawLine(strIniX + 20, strIniY + 40, strIniX + 102, strIniY + 40);
    drawLine(strIniX + 20, strIniY + 100, strIniX + 102, strIniY + 100);
    drawLine(strIniX + 20, strIniY + 160, strIniX + 102, strIniY + 160);

    strIniX += 110;
    //Draw X
    drawLine(strIniX + 20, strIniY + 160, strIniX + 102, strIniY + 40);
    drawLine(strIniX + 20, strIniY + 40, strIniX + 102, strIniY + 160);

    strIniX += 110;
    //Draw I
    drawLine(strIniX + 20, strIniY + 160, strIniX + 102, strIniY + 160);
    drawLine(strIniX + 20, strIniY + 40, strIniX + 102, strIniY + 40);
    drawLine(strIniX + 61, strIniY + 40, strIniX + 61, strIniY + 160);

    strIniX += 110;
    //Draw T
    drawLine(strIniX + 20, strIniY + 160, strIniX + 102, strIniY + 160);
    drawLine(strIniX + 61, strIniY + 40, strIniX + 61, strIniY + 160);
}

void drawBigColon(int strIniX, int strIniY) {
    drawFilledBox(strIniX + 5, strIniY + 30, strIniX + 10, strIniY + 40);
    drawFilledBox(strIniX + 5, strIniY + 60, strIniX + 10, strIniY + 70);
}

void drawMediumNumber(char x, int strIniX, int strIniY) {
    if (x == '1') {
        drawLine(strIniX + 20, strIniY + 100, strIniX + 20, strIniY + 10); //Middle Vertical Line
    } else if (x == '2') {
        drawLine(strIniX, strIniY + 100, strIniX + 40, strIniY + 100); //Top Horizontal Line
        drawLine(strIniX, strIniY + 55, strIniX + 40, strIniY + 55); //Middle Horizontal Line
        drawLine(strIniX, strIniY + 10, strIniX + 40, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX, strIniY + 55, strIniX, strIniY + 10); //Left Vertical Line
        drawLine(strIniX + 40, strIniY + 100, strIniX + 40, strIniY + 55); //Right Vertical Line
    } else if (x == '3') {
        drawLine(strIniX, strIniY + 100, strIniX + 40, strIniY + 100); //Top Horizontal Line
        drawLine(strIniX, strIniY + 55, strIniX + 40, strIniY + 55); //Middle Horizontal Line
        drawLine(strIniX, strIniY + 10, strIniX + 40, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 40, strIniY + 100, strIniX + 40, strIniY + 10); //Right Vertical Line
    } else if (x == '4') {
        drawLine(strIniX, strIniY + 55, strIniX + 40, strIniY + 55); //Middle Horizontal Line
        drawLine(strIniX, strIniY + 100, strIniX, strIniY + 55); //Left Vertical Line
        drawLine(strIniX + 40, strIniY + 100, strIniX + 40, strIniY + 10); //Right Vertical Line
    } else if (x == '5') {
        drawLine(strIniX, strIniY + 100, strIniX + 40, strIniY + 100); //Top Horizontal Line
        drawLine(strIniX, strIniY + 55, strIniX + 40, strIniY + 55); //Middle Horizontal Line
        drawLine(strIniX, strIniY + 10, strIniX + 40, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX, strIniY + 100, strIniX, strIniY + 55); //Left Vertical Line
        drawLine(strIniX + 40, strIniY + 55, strIniX + 40, strIniY + 10); //Right Vertical Line
    } else if (x == '6') {
        drawLine(strIniX, strIniY + 55, strIniX + 40, strIniY + 55); //Middle Horizontal Line
        drawLine(strIniX, strIniY + 10, strIniX + 40, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX, strIniY + 100, strIniX, strIniY + 10); //Left Vertical Line
        drawLine(strIniX + 40, strIniY + 55, strIniX + 40, strIniY + 10); //Right Vertical Line
    } else if (x == '7') {
        drawLine(strIniX, strIniY + 100, strIniX + 40, strIniY + 100); //Top Horizontal Line
        drawLine(strIniX + 40, strIniY + 100, strIniX + 40, strIniY + 10); //Right Vertical Line
    } else if (x == '8') {
        drawLine(strIniX, strIniY + 100, strIniX + 40, strIniY + 100); //Top Horizontal Line
        drawLine(strIniX, strIniY + 55, strIniX + 40, strIniY + 55); //Middle Horizontal Line
        drawLine(strIniX, strIniY + 10, strIniX + 40, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX, strIniY + 100, strIniX, strIniY + 10); //Left Vertical Line
        drawLine(strIniX + 40, strIniY + 100, strIniX + 40, strIniY + 10); //Right Vertical Line
    } else if (x == '9') {
        drawLine(strIniX, strIniY + 100, strIniX + 40, strIniY + 100); //Top Horizontal Line
        drawLine(strIniX, strIniY + 55, strIniX + 40, strIniY + 55); //Middle Horizontal Line
        drawLine(strIniX, strIniY + 10, strIniX + 40, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX, strIniY + 100, strIniX, strIniY + 55); //Left Vertical Line
        drawLine(strIniX + 40, strIniY + 100, strIniX + 40, strIniY + 10); //Right Vertical Line
    } else if (x == '0') {
        drawLine(strIniX, strIniY + 100, strIniX + 40, strIniY + 100); //Top Horizontal Line
        drawLine(strIniX, strIniY + 10, strIniX + 40, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX, strIniY + 100, strIniX, strIniY + 10); //Left Vertical Line
        drawLine(strIniX + 40, strIniY + 100, strIniX + 40, strIniY + 10); //Right Vertical Line
    }
}

void drawBigNumber(char x, int strIniX, int strIniY) {
    if (x == '1') {
        drawLine(strIniX + 35, strIniY + 10, strIniX + 35, strIniY + 80); //Middle Vertical Line
    } else if (x == '2') {
        drawLine(strIniX + 60, strIniY + 80, strIniX + 10, strIniY + 80); //Top Horizontal Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 10, strIniY + 45); //Middle Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 10, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 10, strIniY + 10, strIniX + 10, strIniY + 45); //Left Vertical Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 60, strIniY + 80); //Right Vertical Line
    } else if (x == '3') {
        drawLine(strIniX + 60, strIniY + 80, strIniX + 10, strIniY + 80); //Top Horizontal Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 10, strIniY + 45); //Middle Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 10, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 60, strIniY + 80); //Right Vertical Line
    } else if (x == '4') {
        drawLine(strIniX + 60, strIniY + 45, strIniX + 10, strIniY + 45); //Middle Horizontal Line
        drawLine(strIniX + 10, strIniY + 45, strIniX + 10, strIniY + 80); //Right Vertical Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 60, strIniY + 80); //Right Vertical Line
    } else if (x == '5') {
        drawLine(strIniX + 60, strIniY + 80, strIniX + 10, strIniY + 80); //Top Horizontal Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 10, strIniY + 45); //Middle Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 10, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 10, strIniY + 80, strIniX + 10, strIniY + 45); //Left Vertical Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 60, strIniY + 10); //Right Vertical Line
    } else if (x == '6') {
        drawLine(strIniX + 60, strIniY + 45, strIniX + 10, strIniY + 45); //Middle Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 10, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 10, strIniY + 80, strIniX + 10, strIniY + 10); //Left Vertical Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 60, strIniY + 10); //Right Vertical Line
    } else if (x == '7') {
        drawLine(strIniX + 60, strIniY + 80, strIniX + 10, strIniY + 80); //Top Horizontal Line
        drawLine(strIniX + 60, strIniY + 80, strIniX + 60, strIniY + 10); //Right Vertical Line
    } else if (x == '8') {
        drawLine(strIniX + 60, strIniY + 80, strIniX + 10, strIniY + 80); //Top Horizontal Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 10, strIniY + 45); //Middle Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 10, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 10, strIniY + 80, strIniX + 10, strIniY + 10); //Left Vertical Line
        drawLine(strIniX + 60, strIniY + 80, strIniX + 60, strIniY + 10); //Right Vertical Line
    } else if (x == '9') {
        drawLine(strIniX + 60, strIniY + 80, strIniX + 10, strIniY + 80); //Top Horizontal Line
        drawLine(strIniX + 60, strIniY + 45, strIniX + 10, strIniY + 45); //Middle Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 10, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 10, strIniY + 80, strIniX + 10, strIniY + 45); //Left Vertical Line
        drawLine(strIniX + 60, strIniY + 80, strIniX + 60, strIniY + 10); //Right Vertical Line
    } else if (x == '0') {
        drawLine(strIniX + 60, strIniY + 80, strIniX + 10, strIniY + 80); //Top Horizontal Line
        drawLine(strIniX + 60, strIniY + 10, strIniX + 10, strIniY + 10); //Bottom Horizontal Line
        drawLine(strIniX + 10, strIniY + 80, strIniX + 10, strIniY + 10); //Left Vertical Line
        drawLine(strIniX + 60, strIniY + 80, strIniX + 60, strIniY + 10); //Right Vertical Line
    }
}