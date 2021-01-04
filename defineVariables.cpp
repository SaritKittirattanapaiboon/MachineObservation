#include "csvgraphics.h"

int inSignals[machineNums];
int prevSignals[machineNums];
string curtime;
int blink[machineNums];
int defaultState[machineNums];
int signalOn[machineNums];
int signalIdle[machineNums];
int signalOff[machineNums];
int state[machineNums];
int mouseState;
bool fullscreen;
bool clear[machineNums];
string pathName;
int mode;
int prevMode;
int rotation=0;
bool reset=false;
bool plcComm=true;
int testTimer;
string obtainedDate[machineNums];
string fixedDate[machineNums];
string machinePaths[machineNums];
int selectBox;
bool keyboardArrow;
int graphicsTime;
int graphicsTimePrev;
int checkError;

int createDates() {
    ifstream machines;
    int machineNum = 0;
    size_t posi, posf;
    machines.open("Machines.csv");
    string test = "this,is,a,test,for,multiple,commas";
    if (machines.is_open()) {
        getline(machines, test, '\n');
        while (!machines.eof()) {
            getline(machines, test, '\n');
            if (test.empty()) {
                break;
            }
            posi = findLastN(test, ",", 13);
            posf = findLastN(test, ",", 12);
            //cout << test << endl;
            //cout << posi << " " << posf << endl;
            obtainedDate[machineNum] = test.substr(posi + 1, posf - posi - 1);
            //cout << obtained[machineNum]  << endl;
            //cout << test.substr(test.length()-2,1) << endl;
            while (test.substr(test.length() - 2, 1) == ",") {
                test = test.substr(0, test.length() - 1);
            }
            test = test.substr(0, test.length() - 1);
            //cout << test << endl;
            posi = findLastN(test, ",", 2);
            posf = findLastN(test, ",", 1);
            //cout << posi << " " << posf << " " << posf-posi << endl;
            //cout << "this: " << test.substr(posi+1,posf-posi) << endl; //Something went wrong here. Fix tmr
            test = test.substr(posi + 1, posf - posi - 1);
            //cout << test.substr(0,1) << endl;
            if (test.substr(0, 1) == "C") {
                fixedDate[machineNum] = "none";
                //cout << fixedDate[machineNum] << endl;
            } else {
                fixedDate[machineNum] = test;
                //cout << fixedDate[machineNum] << endl;
            }
            //cout << test.length() << endl;
            machineNum++;
        }
    } else {
        cout << "Can't open Machines.csv" << endl;
        return -1;
    }
    machines.close();

    machineNum=0;
    machines.open("machineinfopath.txt");
    if(machines.is_open()){
        while(!machines.eof()){
            getline(machines, test, '\n');
            if (test.empty()) {
                break;
            }
            machinePaths[machineNum]=test;
            machineNum++;
        }
    }
    else{
        cout << "Can't open machineinfopath.txt" << endl;
        return -1;
    }
    machines.close();
    return 0;
}