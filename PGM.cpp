#include "PGM.h"

PGM::PGM() : magicNumber(""), width(0), height(0), maxPx(0), pixels(nullptr) {}

PGM::~PGM() 
{
    if (pixels != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }
}

string PGM::getMagicNumber()
{
    return magicNumber;
}

void PGM::setMagicNumber(string &magic)
{
    magicNumber = magic;
}

int PGM::getWidth(void)
{
    return width;
}

void PGM::setWidth(int w)
{
    width = w;
}

int PGM::getHeight()
{
    return height;
}

void PGM::setHeight(int h)
{
    height = h;
}

int PGM::getMaxPx()
{
    return maxPx;
}

void PGM::setMaxPx(int m)
{
    maxPx = m;
}

int PGM::getTotalPixels()
{
    int totalPixel = width * height;
    return totalPixel;
}

short** PGM::getPixels()
{
    return pixels;
}

short* PGM::getCompressedPixels()
{
    return compressedPgm;
}

int PGM::toInt(const string& str)
{
    int result = 0;
    for (char c : str)
    {
        if (c >= '0' && c <= '9')
        {
            result = result * 10 + (c - '0');
        }
        else
        {
            cerr << "Invalid character encountered in toInt";
        }
    }
    return result;
} 

void PGM::readPGM(const string &fileName)
{
    ifstream file(fileName, ios::binary);
    if (file.is_open()) 
    {
        streampos fileSize = file.tellg();
        file.seekg(0, ios::end);
        fileSize = file.tellg() - fileSize;
        file.seekg(0, ios::beg);

        if (fileSize <= 0) 
        {
            cerr << "Issue with the file size" << endl;
            return;
        }

        char* mBlock = new char[fileSize];
        file.read(mBlock, fileSize);
        file.close();
        cout << "File has been opened successfully" << endl;
        
        int bytesRead = 0;
        char* data = mBlock;

        while (*data != '\n' && data < mBlock + fileSize)
        {
            magicNumber += *data;
            data += sizeof(unsigned char);
            bytesRead++;
            if (data >= mBlock + fileSize) 
            {
                cerr << "Unexpected end of file while reading magic number" << endl;
            }
        }
        data += sizeof(unsigned char);
        bytesRead++;
        
        string num;
        while (*data != ' ')
        {
            num += *data;
            data += sizeof(unsigned char);
            bytesRead++;
        }
        width = toInt(num);
        num.clear();
        data += sizeof(unsigned char);
        bytesRead++;

        while (*data != '\n')
        {
            num += *data;
            data += sizeof(unsigned char);
            bytesRead++;
        }
        height = toInt(num);
        num.clear();
        data += sizeof(unsigned char);
        bytesRead++;

        while (*data != '\n')
        {
            num += *data;
            data += sizeof(unsigned char);
            bytesRead++;
        }
        maxPx = toInt(num);
        num.clear();
        data += sizeof(unsigned char);
        bytesRead++;
        
        pixels = new (nothrow) short*[height];
        for (int i = 0; i < height; i++)
        {
            pixels[i] = new (nothrow) short[width];
            for (int j = 0; j < width; j++)
            {
                unsigned char pixelValue = *data;
                file.read((char*)&pixelValue, sizeof(unsigned char));
                pixels[i][j] = pixelValue;
                data++;
                bytesRead++;
            }
        }

        cout << "Magic Number: " << magicNumber << endl;
        cout << "Height: " << height << endl;
        cout << "Width: " << width << endl;
        cout << "Max pixel value: " << maxPx << endl;
        int totalPixel = width * height;
        cout << "Total pixels: " << totalPixel << endl;

        for (int i = 0; i < height; ++i) 
        {
            for (int j = 0; j < width; j++)
            {
                cout << pixels[i][j] << " ";
            }
            cout << endl;
        }

        if (bytesRead != fileSize) {
            cerr << "The bytes don't match with the size" << endl;
        }

        delete[] mBlock; 
    }
    else
    {
        cout << "File " << fileName << " could not be opened" << endl;
    }
}

void PGM::compressor()
{
    short currentPixel = pixels[0][0];
    int k = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            if (currentPixel == pixels[i][j])
            {
                continue;
            }
            else
            {
                k++; 
                currentPixel = pixels[i][j];
            }
        } 
    }
    k++;

    short* reps = new short[k];
    short* values = new short[k];
    currentPixel = pixels[0][0];
    int count = 0;
    int rep = 1;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            if (currentPixel == pixels[i][j])
            {
                rep++;
            }
            else
            {
                reps[count] = rep;
                values[count] = currentPixel;
                count++;
                currentPixel = pixels[i][j];
                rep = 1;
            }
        } 
    }
    reps[count] = rep;
    values[count] = currentPixel;

    for (int i = 0; i < k; i++)
    {
        cout << reps[i] << " " << values[i] << " ";
    }
    cout << endl << "Done" << endl;
    
    ofstream file("Compressed.ak", ios::binary); 
    if (file.is_open()) 
    {
        cout << "Transferring to file" << endl;
        file.write((char*)reps, sizeof(short) * k);
        file.write((char*)values, sizeof(short) * k);
        file.close();
        cout << "Transfer completed" << endl;
    }
    else 
    {
        cerr << "Failed to open file for writing" << endl;
    }

    delete[] reps;
    delete[] values;
}

void PGM::deCompressor(short *reps, short *values, int Count)
{
    int width = this->width;
    int height = this->height;

    short** decompressedPgm = new (nothrow) short*[height];
    for (int i = 0; i < height; ++i) 
    {
        decompressedPgm[i] = new (nothrow) short[width];
    }

    int i = 0;
    int j = 0;

    for (int k = 0; k < Count; k++)
    {
        short creps = reps[k];
        short cValues = values[k];
        for (int l = 0; l < creps; l++)
        {
            decompressedPgm[i][j] = cValues;
            j++;
            if (j == width)
            {
                j = 0;
                i++;
                if (i == height)
                {
                    break;
                }
            }
        }
    }

    cout << "Decompression completed" << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << decompressedPgm[i][j] << " ";
        }
        cout << endl;
    }
    
    for (int i = 0; i < height; i++)
    {
        delete[] decompressedPgm[i];
    }
    delete[] decompressedPgm;
}
