#ifndef CMCC_H
#define CMCC_H

class CMCC
{
private:
// ---- DM params ---- //    
    double log_mx;
    double log_sv_min;
    double log_sv_max;
    double log_sv_step;
    int    channelNum;
// ---- Other variables/constants ---- //
    const double a_small_num = 0.000000000000001;
    const double a_big_num   = 1e80;

public:
    CMCC(double n1, double n2, double n3, double n4, int n5); // initialise DM params
    void printParam();
    void genIniFile(double log_mx, double log_sv);
    void startCalc();
    void buildMinTable();
    void sortMinTable();
};


#endif
