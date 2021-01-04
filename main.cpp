#include "csvgraphics.h"
#include "overlap.h"

/*
This program's purpose is to turn the signals obtained into a displayable format. The code is written in a way that is
 probably not easily read. Sorry for that. Explanation of each function can be found in csvgraphics.h, as well as where
 each function's implentation can be found. If you're reading this you are probably trying to do some maintanance. Again,
 my apologies.
*/

string readTogo();

int main(int argc, char** argv) {
    string programMode = "Demo";
    LPSTR buffer;
    srand(time(NULL));
    GetModuleFileName(nullptr, buffer, 100);
    size_t pos;
    mode = 0;
    testTimer = 0;
    pathName = buffer;
    pos = findLastG(pathName, "\\");
    pathName = pathName.substr(0, pos + 1);
    pathName.append("dataFiles\\");
    chdir(pathName.c_str());

    string checkingPLC;
    ifstream checkPLC;
    ofstream closePLC;

    checkPLC.open("togo.txt");
    if (!checkPLC.is_open()) {
        cout << "Error. Check the togo.txt and restart the program" << endl;
        system("pause");
        return 0;
    }
    while (!checkPLC.eof()) {
        getline(checkPLC, checkingPLC, '\n');
    }
    checkPLC.close();

    if (checkingPLC == "e") {
        cout << "The program is open on another computer. This program will simply display the graphics" << endl;
        readCurrent();
        //getSavedDataG(nullptr, nullptr);
        plcComm = false;
    } else {
        closePLC.open("togo.txt");
        closePLC << "e";
        closePLC.close();


        //string ClientName2 = "ClientTest.exe";
        //ShellExecute(NULL, "open", ClientName2.c_str(), NULL, NULL, SW_SHOWMINIMIZED);

        cout << "Waiting to Communicate with PLC" << endl << flush;
        Sleep(5000);

        checkingPLC=readTogo();

        if(checkingPLC!="e"){
            if(checkingPLC=="i") {
                cout << "Communication with PLC Error. Check ipPort.txt" << endl;
                closePLC.open("togo.txt");
                closePLC << "";
                closePLC.close();
            }
            else if(checkingPLC=="q"){
                cout << "Cannot communicate with PLC. Check the cables and try again" << endl;
                closePLC.open("togo.txt");
                closePLC << "";
                closePLC.close();
            }
            else{
                cout << "Unkonown error communicating with PLC" << endl;
                closePLC.open("togo.txt");
                closePLC << "";
                closePLC.close();
            }
            system("pause");
            exit(0);
        }


        for (int i = 0; i < machineNums; i++) {
            currentCount[i] = 0;
            blink[i] = 0;
            state[i] = 0;
            //Set Clear Here
            clear[i] = true;
            defaultState[i] = 0;
        }
        checkError = setup();
        readMachineUsed();
    }

    if (checkError == -1) {
        cout << "Error. Check the files and restart the program" << endl;
        closePLC.open("togo.txt");
        closePLC.close();
        system("pause");
        return 0;
    }
    if(plcComm) {
        checkError = createDates();
    }
    if (checkError == -1) {
        cout << "Error. Check the files and restart the program" << endl;
        closePLC.open("togo.txt");
        closePLC.close();
        system("pause");
        return 0;
    }

    if(plcComm) {
        checkError = getDrawing();
    }

    if (checkError == -1) {
        cout << "Error. Check the files and restart the program" << endl;
        closePLC.open("togo.txt");
        closePLC.close();
        system("pause");
        return 0;
    }
    fullscreen = false;
    keyboardArrow = false;
    mouseState = 0;
    selectBox = 23;
    cout << "Running" << endl;
    graphicsSetup(argc, argv);
    glutDisplayFunc(drawScene);
    string temp=getFileNameG();
    graphicsTimePrev = stoi(temp.substr(10, 6));
    glutTimerFunc(10, timer, 0);
    glutMainLoop();
    return 0;
}


string readTogo(){
    ifstream checkPLC;
    string checkingPLC;
    checkPLC.open("togo.txt");
    if (!checkPLC.is_open()) {
        cout << "Error. Check the togo.txt and restart the program" << endl;
        system("pause");
        return "Err";
    }
    getline(checkPLC, checkingPLC, '\n');
    checkPLC.close();
    return checkingPLC;
}