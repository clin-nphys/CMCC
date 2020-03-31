#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "CMCController.h"
#include "Utils.h"
using namespace std;

struct chisqData
{
    double col[13];
};

CMCC::CMCC(double n1, double n2, double n3, double n4, int n5)
{
    log_mx      = n1;
    log_sv_min  = n2;
    log_sv_max  = n3;
    log_sv_step = n4;
    channelNum  = n5;
}

void CMCC::printParam()
{
    cout << "  :: Pringint parameters" << endl;
    cout << "       log_mx      = " << log_mx << endl;
    cout << "       log_sv_min  = " << log_sv_min << endl;
    cout << "       log_sv_max  = " << log_sv_max << endl;
    cout << "       log_sv_step = " << log_sv_step << endl;
    cout << "       channelNum  = " << channelNum << endl;
}

void CMCC::genIniFile(double log_mx, double log_sv)
{
    Utils extFunc;
    string iniFileDir = "./ini_files/2018/";
    //string iniFileDir = "./";
    string iniFileName = iniFileDir + extFunc.getFileTitle(log_mx, log_sv) + ".ini";
    string iniStandard = iniFileDir + "standard_2018.ini";
    string tmpLine;
    if( !extFunc.fileExists(iniFileName.c_str()) ){
        ifstream fin;
        ofstream fout;

        fin.open(iniStandard.c_str());
        fout.open(iniFileName.c_str());

        while (getline(fin, tmpLine)){
            fout << tmpLine << endl;
        }
        fout << endl;
        fout << "param[DM_mass] = " << fixed << setprecision(5) << pow(10, log_mx) << endl;
        fout << "param[DM_log_sigmav] = " << fixed << setprecision(5) << log_sv << endl;


        fin.close();
        fout.close();
    }
}

void CMCC::startCalc()
{
    int CMCreturn = -999;
    Utils extFunc;
    double scan_log_mx = log_mx;
    double scan_log_sv;
    double prev_log_sv;
    string cmd;
    string iniFileName;
    stringstream msg;

    for (scan_log_sv = log_sv_min; scan_log_sv < log_sv_max - a_small_num; scan_log_sv += log_sv_step){
        iniFileName = extFunc.getFileTitle(scan_log_mx, scan_log_sv) + ".ini";

        cmd = "./cosmomc ./ini_files/2018/" + iniFileName;
        cout << endl << endl;
        cout << "  :: ==========================================================================" << endl;
        cout << "  ::  cmd = " << cmd << endl;
        cout << "  :: ==========================================================================" << endl << endl;
        msg.str("");
        msg << "  :: m_X = " << pow(10, scan_log_mx) << ", sigmav = " << pow(10, scan_log_sv) << " starts...";
        extFunc.writeLogfile(msg.str());
        
        CMCC::genIniFile(scan_log_mx, scan_log_sv);
        CMCreturn = system(cmd.c_str());
        
        msg.str("");
        msg << "      done. CosmoMC = " << CMCreturn << "." << endl;
        extFunc.writeLogfile(msg.str());
        
        prev_log_sv = scan_log_sv;
    }

    scan_log_sv = log_sv_max;
    if (extFunc.tooSimilar(pow(10, prev_log_sv), pow(10, scan_log_sv))) {
        msg.str("");
        msg << "  !> m_X = " << pow(10, scan_log_mx) << ", " << prev_log_sv << " and " << scan_log_sv << " too similar. Skip." << endl;
        extFunc.writeLogfile(msg.str());
    } else {
        iniFileName = extFunc.getFileTitle(scan_log_mx, scan_log_sv) + ".ini";

        cmd = "./cosmomc ./ini_files/2018/" + iniFileName;
        cout << endl << endl;
        cout << "  :: ==========================================================================" << endl;
        cout << "  ::  cmd = " << cmd << endl;
        cout << "  :: ==========================================================================" << endl << endl;
        msg.str("");
        msg << "  :: m_X = " << pow(10, scan_log_mx) << ", sigmav = " << pow(10, scan_log_sv) << " starts...";
        extFunc.writeLogfile(msg.str());
        
        CMCC::genIniFile(scan_log_mx, scan_log_sv);
        CMCreturn = system(cmd.c_str());
        
        msg.str("");
        msg << " done. CosmoMC = " << CMCreturn << "." << endl;
        extFunc.writeLogfile(msg.str());
        
        prev_log_sv = scan_log_mx;
    }

}

void CMCC::buildMinTable()
{
    Utils extFunc;
    chisqData minData[50];
    int minDataLineNum = 0;

    for (int i = 0; i < 50; i ++){
        for (int j = 0; j < 12; j ++) minData[i].col[j] = a_big_num;
    }

    chisqData singleLine;
    int minLineAt;

    ifstream fin;

    string minTableName = "chisqmin_" + extFunc.num2ch(channelNum) + "_" + to_string(pow(10, log_mx)) + "gev.dat";
    

    
    if ( extFunc.fileExists(minTableName.c_str()) ){
        fin.open(minTableName.c_str());
        while (fin >> minData[minDataLineNum].col[0]) {
            for (int i = 1; i < 12; i ++) {
                fin >> minData[minDataLineNum].col[i];
            }
            minDataLineNum ++;
        }
        fin.close();
    }

    fin.open("chisq.dat");
    bool found_sv;
    while (fin >> singleLine.col[0]){
        for (int i = 1; i < 12; i++)
            fin >> singleLine.col[i];

        found_sv = 0;
        for (int i = 0; i < minDataLineNum; i++){
            if ( extFunc.tooSimilar(singleLine.col[1], minData[i].col[1]) ){
                found_sv = 1;
                if ( singleLine.col[5] < minData[i].col[5] ){
                    for (int j = 0; j < 12; j++){
                        minData[i].col[j] = singleLine.col[j];
                    }
                }
                break;
            }
        }
        if (!found_sv) {
            for (int j = 0; j < 12; j++){
                minData[minDataLineNum].col[j] = singleLine.col[j];
            }
            minDataLineNum ++;
        }
    }
    fin.close();

    ofstream fout;
    fout.open(minTableName.c_str(), std::ofstream::trunc);

    for (int i = 0; i < minDataLineNum; i++){
        for (int j = 0; j < 12; j++){
            if ( j == 1 ) {
                fout << scientific << minData[i].col[j] << " ";
            } else {
                fout << fixed << setprecision(8) << minData[i].col[j] << " ";
            }
        }
        fout << endl;
    }
    fout.close();
    extFunc.writeLogfile("\n  :: Chisqmin updated.\n");
}