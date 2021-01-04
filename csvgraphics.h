#ifndef FINALCOMPILE_CSVGRAPHICS_H
#define FINALCOMPILE_CSVGRAPHICS_H

#include <windows.h>
#include <chrono>
#include <sys/stat.h>

#include <iostream>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <math.h>
#include <ctime>
#include <random>

#include <string>
#include <fstream>
#include <sstream>

#include <GL/freeglut.h>

#include "overlap.h"

using namespace std;

// set the drawing screen dimensions and position
#define WINDOW_HEIGHT   1080
#define WINDOW_WIDTH    1920
#define WINDOW_X        100
#define WINDOW_Y        150

// set the pre-defined colors
#define WHITE           1.0,1.0,1.0
#define BLACK           0.0,0.0,0.0
#define RED             1.0,0.0,0.0
#define BLUE            0.0,0.0,1.0
#define GREEN           0.0,1.0,0.0
#define DARKGREEN       0.8,0.8,0.0
#define GREY            0.25,0.25,0.25
#define GRAY            0.20,0.20,0.20
#define PURPLE          0.5,0.25,0.0
#define FOREST_GREEN    0.0,0.25,0.0
#define MIDNIGHT_BLUE   0.0,0.0,0.25
#define CYAN            0.0,1.0,1.0
#define MAGENTA         1.0,0.0,1.0
#define YELLOW          1.0,0.5,0.0
#define BROWN           0.5,0.25,0.0


//static const int machineNums=23; //Number of machines supported. Currently 23, changing this will probably break everything
extern int inSignals[machineNums]; //Incoming signals that is read from signals.txt
extern int prevSignals[machineNums]; //Previous signals to check if there is a change in signals

extern int machineInUse[machineNums];

extern int blink[machineNums];
extern int defaultState[machineNums];
extern string drawing[machineNums]; //drawing number
extern string curtime; // String to store the current date/time
extern int signalOn[machineNums]; // Stores the daily on time
extern int signalIdle[machineNums]; //Stores the daily idle time
extern int signalOff[machineNums]; //Stores the daily off time
extern int state[machineNums]; //Stores the state of each box (drawing). State 1 is just a circle, state 2 shows the
                                //current count, state 3 shows the daily on/off/idle values
extern int mouseState; //Stores the exit state on the bottom right. 0 shows time, 1 shows the exit
extern bool fullscreen; //True if the screen is in full screen mode
extern bool clear[machineNums]; //Stores the red signal clear boolean. False means that the state will go to current
                                //count no matter what
extern int mode; //Stores the current drawing mode. 0 is the default, 1 is a map, 2 is the test excel version, 100 is exit, 101 is the help
extern int prevMode; //Stores the previous mode for checking the exits and help
extern int rotation; //Rotation for mode 2
extern bool reset; //resetting the rotation
extern int testTimer; //Testing purpose variable. No use
extern string pathName; //Stores the working directory path
extern string obtainedDate[machineNums]; //Stores the obtained date for each machine
extern string fixedDate[machineNums]; //Stores the latest fixed date for each machine
extern int selectBox; //Selection box for arrow keys
extern bool keyboardArrow; //Enable arrow keys
extern string machinePaths[machineNums]; //filename of the machine information file. Folder name not necessary

extern int graphicsTime;
extern int graphicsTimePrev;
extern int checkError;

///////Defined in defineVariables.cpp - All the extern variables are stored here
int createDates(); //Reads the dates from a txt file for obtained and fixed dates

///////Defined in loop.cpp
int getSignals(); //Reads the signals from signals.txt
void timer(int); //Looping glutTimer function. Increases current count
void drawScene(); //glutDraw function for different modes
size_t findPosG(string full, string find, int num); //Finds the position of a character
size_t findLastG(const string full, string find); //finds the postion of the last time the character appears
size_t findLastN(const string full, string find, int fromBack);
void chooseMColor(int macNo); //Chooses a color based on the signal of the machine (Red/Orange/Green)
int getSavedDataG(string* month, int* day);

///////Defined in modesStates.cpp
void drawModeZero(int machineNo, int centerx, int centery); //Draws the mode 0, dependant on states
void drawModeOne(); //Draws mode 1: Map
void drawModeTwo(); //Draws mode 2: Excel version
void drawModeThree(); //Too Big Excel
void drawModeQuit(); //Draws mode 100: Exit Message Box
void drawModeHelp(); //Draws mode 101: Help Box
void drawModeError();

///////Defined in graphics.cpp
// initialization routine
int readMachineUsed(); //reads if a Machine is currently in use
void graphicsSetup(int argc, char **argv); //setup for glut
void clearWindow(void); //clears windows

// set line or fill color
void setColor(double red, double green, double blue);

// graphic object primatives
void drawTriangle(int x1, int y1,int x2,int y2,int x3,int y3);
void drawLine(int x1, int y1, int x2, int y2);
void drawBox(int x1, int y1, int x2, int y2);
void drawCircle(int x1, int y1, int radius);

// filled graphics primatives
void drawFilledTriangle(int x1, int y1,int x2,int y2,int x3,int y3);
void drawFilledBox(int x1, int y1, int x2, int y2);
void drawFilledCircle(int x1, int y1, int radius);

// custom drawing functions
int readMachines(string machineNames[]); //Reads the machine names from Machines.csv for drawString
void drawString(string machineName, int centerx, int centery); //draws a string, calls drawLetter
void drawLetter(char x, int strIniX, int strIniY); //draws a letter, symbol, or number
void drawDash(int strIniX, int strIniY); //draws a dash
void drawColon(int strIniX, int strIniY); //draws a :
void drawNumber(char x, int numMachine, int charNum); //draws a number
void drawLetterAJ(char x, int strIniX, int strIniY); //draws letters from a-j
void drawLetterKT(char x, int strIniX, int strIniY); //draws letters from k-t
void drawLetterUZ(char x, int strIniX, int strIniY); //draws letters from u-z
void drawSymbol(char x, int strIniX, int strIniY); //draws symbols such as '(' ')' or '?'

string getFileNameG(); //get the current date/time
string getTime(int thisTime); //Gets the current time in hour/min/sec format from a total seconds int
int getData(); //reads data for the daily on/idle/off times
void drawTime(int strIniX=1440+15, int strIniY=0); //draws the current time, updated by timer
void drawExit(); //draws an EXIT for the bottom right during mousestate=1
void drawBigColon(int strIniX, int strIniY); //draws a large : for time
void drawMediumNumber(char x, int strIniX, int strIniY); //draws a medium sized number
void drawBigNumber(char x, int strIniX, int strIniY); //draws a large number for time

///Defined in mouseFuncs.cpp
void quitKey(unsigned char key, int x, int y); //Not only quit key. Keyboard function for glut
void mousefunc(int button, int upDown, int xpos, int ypos); //mousefunctions (Left click, etc) for glut
void specialKeyboard(int key, int x, int y); //keyboard functions for arrowkeys
int getColumn(int xpos); //gets the column based on the mouse's xpos
int getRow(int ypos); //gets the row based on the mouse's ypos

#endif //FINALCOMPILE_CSVGRAPHICS_H
