#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "Utils.h"
using namespace std;

string Utils::getFileTitle(double mx, double sv)
{
    string title = "";
    string mx_str = to_string(mx);
    string sv_str = to_string(sv);

    title = "standard_" + mx_str + "_" + sv_str;

    return title;
}

void Utils::writeLogfile(string msg)
{
    ofstream fout;
    fout.open("CMCCLogfile.log", std::ios_base::app);

    fout << msg;
    fout.close();
}

bool Utils::tooSimilar(double a, double b)
{
    double frac_diff = abs( abs(a - b) / a);
    if (frac_diff < thresh) return 1;
    else return 0;
}

bool Utils::fileExists(string filename)
{
    ifstream f(filename.c_str());
    return f.good();
}

string Utils::num2ch(int ch)
{
    switch(ch){
        case 1:{
            return "ee";
            break;
        }
        case 2:{
            return "bb";
            break;
        }
    }
}