#include <iostream>
#include "PGM.h"
using namespace std;

void Menu()
{
    cout << "===============================" << endl;
    cout << "PGM Image Compression Software" << endl;
    cout << "===============================" << endl;
    cout << "1. Load PGM Image" << endl;
    cout << "2. Compress Image" << endl;
    cout << "3. Decompress Image" << endl;
    cout << "4. Exit" << endl;
    cout << "Please choose an option (1-4): ";
}

int main()
{
    PGM pgm;
    int choice;
    string fileName;

    while (true)
    {
        Menu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter the file name of the PGM image to load: ";
            cin >> fileName;
            cout << endl;
            pgm.readPGM(fileName);
            break;
        case 2:
            cout << "Compressing image..." << endl;
            pgm.compressor();
            break;
        case 3:
            {
                ifstream file("Compressed.ak", ios::binary);
                if (file.is_open())
                {
                    file.seekg(0, ios::end);
                    int fileSize = file.tellg();
                    file.seekg(0, ios::beg);

                    int Count = fileSize / (2 * sizeof(short));

                    short* reps = new short[Count];
                    short* values = new short[Count];
                    file.read((char*)reps, Count * sizeof(short));
                    file.read((char*)values, Count * sizeof(short));
                    file.close();
                    cout << "Decompressing image..." << endl;
                    pgm.deCompressor(reps, values, Count);
                }
                else
                {
                    cout << "Failed to open compressed file." << endl;
                }
            }
            break;
        case 4:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid option. Please try again." << endl;
            break;
        }
    }

    return 0;
}
