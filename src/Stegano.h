//
// Created by leo.meli on 10.01.2018.
//
#ifndef STEGANOGRAPHIE_STEGANO_H
#define STEGANOGRAPHIE_STEGANO_H

#include <cstdio>
#include <bitset>

using namespace std;

class Stegano {

public:
    Stegano();
    bool codeMessage(string fileName, string message);
    string decodeMessage(string fileName);

private:
    static const int  SIZE_OCTET         = 8;     /*!< La taille d'un octet en bits. */
    static const int  BMP_HEADER_SIZE    = 54;    /*!< La taille du header d'une image BMP. */
    static const char STX                = 0x02;  /*!< Caractère ASCII de début de text. */
    static const char ETX                = 0x03;  /*!< Caractère ASCII de fin de text. */

    int readImageSizeFile(string fileName);

    int getBinaryImage(unsigned char binaryImage[], int sizefile, string fileName);

    void convertMessage(string message, bitset<SIZE_OCTET> binaryMessage[]);

    int writeBMP(unsigned char binaryImage[], bitset<SIZE_OCTET> binaryMessage[], int fileSize, int binaryMessageArraySize);
};

#endif //STEGANOGRAPHIE_STEGANO_H
