#ifndef PGM_H
#define PGM_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class PGM
{
private:
    string magicNumber;
    int width;
    int height;
    int maxPx;
    int totalPixel;
    short** pixels;
    short* compressedPgm;

public:
    PGM();
    ~PGM();

    // getters and setters
    string getMagicNumber();
    void setMagicNumber(string &magic);
    int getWidth();
    void setWidth(int w);
    int getHeight();
    void setHeight(int h);
    int getMaxPx();
    void setMaxPx(int m);
    int getTotalPixels();
    short** getPixels();
    short* getCompressedPixels();

    // Other functions
    int toInt(const string& str);
    void readPGM(const string &fileName);
    void compressor();
    void deCompressor(short *compressedReps, short *compressedVals, int runCount);
};

#endif
