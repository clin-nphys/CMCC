#ifndef UTILS_H
#define UTILS_H

class Utils
{
private:
    const double thresh = 0.0001;
public:
    std::string getFileTitle(double mx, double sv);
    void writeLogfile(std::string msg);
    bool tooSimilar(double a, double b);
    bool fileExists(std::string filename);
    std::string num2ch(int ch);
};


#endif
