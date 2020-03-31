#include <iostream>
#include <fstream>
#include <string>
#include "CMCController.h"
#include "Utils.h"
using namespace std;

double log_mx;
double log_sv_min, log_sv_max, log_sv_step;

int channelNum;

bool checkChannelSetting(int ch)
{
    bool ans = 0;
    string line;
    ifstream fin;
    //fin.open("testfile.txt");
    fin.open("./camb/LikeDM/Gen_dNdE.ini");

    while (getline(fin, line) && ans == 0){
        switch(ch){
            case 1:{
                if (line.find("BR_3=1.0") != string::npos) ans = 1;
                break;
            }
            case 2:{
                if (line.find("BR_12=1.0") != string::npos) ans = 1;
                break;
            }
        }
    }

    fin.close();
    return ans;
}

int main(int argc, char* argv[]) // log_mx, log_sv_min, log_sv_max, log_sv_step, channel(1:ee, 2:bb)
{
    Utils extFunc;
    extFunc.writeLogfile("\n// ---- CMCC START ---- //\n\n");
    

// ---- Check argument count ---- //
	if (argc != 6) {
        cout << "  !> Argument count error, exit." << endl;
        return 1;
    }

// ---- Assigning basic arguments ---- //
    log_mx      = stod(argv[1]);
    log_sv_min  = stod(argv[2]);
    log_sv_max  = stod(argv[3]);
    log_sv_step = stod(argv[4]);
    channelNum  = stoi(argv[5]);

    double tmp;
    if (log_sv_max < log_sv_min) {
        tmp = log_sv_max;
        log_sv_max = log_sv_min;
        log_sv_min = tmp;
    }

    extFunc.writeLogfile("  (./CMCC.o " + to_string(log_mx) + " " + to_string(log_sv_min) + " " + to_string(log_sv_max) + " " + to_string(log_sv_step) + " " + to_string(channelNum) + ")\n\n");

// ---- Check channel setting in Gen_dnde.ini file ---- //
    if ( channelNum != 1 && channelNum != 2 ){
        cout << "  !> Unrecognised channel number, exit." << endl;
        return 2;
    }
    if ( !checkChannelSetting(channelNum) ) {
        cout << "  !> Channel setting error, exit." << endl;
        return 3;
    }

    CMCC extObj(log_mx, log_sv_min, log_sv_max, log_sv_step, channelNum);
    extObj.startCalc();
    extObj.buildMinTable();

// ---- End ---- //
    extFunc.writeLogfile("\n// ---- CMCC END ---- //\n");
    return 0;
}
