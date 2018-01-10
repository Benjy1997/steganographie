//
// Created by leo.meli on 10.01.2018.
//

#include "Stegano.h"

Stegano::Stegano(){
};

/**
 * Code le message dans l'image.
 * @param fileName
 * @param message
 * @return Une erreur.
 */
bool Stegano::codeMessage(string fileName, string message) {

    // Ajoute les caractère de début de text et de fin de text.
    string messageToCode = STX + message + ETX;

    // Calcule la taille du tableau qui contiendera le message lettre par lettre en binaire.
    int arraySize = messageToCode.length();
    bitset<SIZE_OCTET> binaryMessage[arraySize];

    // Conversion du message en binaire
    convertMessage(messageToCode, binaryMessage);

    int fileSize = 0;
    fileSize = getImageSizeFile(fileName);

    if (fileSize <= 0)
        return true;

    // Vérifie si l'image peut contenir le message que l'on veut coder.
    if ((fileSize - BMP_HEADER_SIZE) < (messageToCode.length() * SIZE_OCTET))
        return true;

    unsigned char *bi = new unsigned char[fileSize];

    // Code le message dans l'image.
    getBinaryImage(bi, fileSize, fileName);
    writeBMP(bi, binaryMessage, fileSize, arraySize);

    delete[] bi;
    return false;
}

/**
 * Décode le message
 * @param fileName
 * @param fileSize
 * @return Le message décodé
 */
string Stegano::decodeMessage(string fileName) {

    int fileSize = getImageSizeFile(fileName);
    string message = "";

    // Ouvre le fichier en lecture binaire.
    FILE *pFile;
    pFile = fopen(fileName.c_str(), "rb");

    // Vérifie si le fichier est bien ouvert.
    if (pFile == NULL)
        return "Le fichier est introuvable.";

    int dataSize = fileSize - BMP_HEADER_SIZE;
    unsigned char *imageData = new unsigned char[dataSize];
    fread(imageData, sizeof(unsigned char), dataSize, pFile); // read the 54-byte header
    char charact = 0;

    int indexImage = BMP_HEADER_SIZE;

    //Parcoure toutes les lettres du message.
    do {

        bitset<SIZE_OCTET> currentCharact;

        // Parcoure tous les bits d'une lettre composant le message.
        for (int indexBit = 7; indexBit >= 0; indexBit--) {
            bitset<SIZE_OCTET> currentColor(imageData[indexImage]);
            currentCharact.set(indexBit, currentColor.test(0));

            // change l'occurence du tableau.
            indexImage++;
        }

        //convertie le caractère binaire courant en char.
        charact = (char) currentCharact.to_ulong();

        //Vérifie qu'il ya bien un message codé dans l'image.
        if (indexImage == BMP_HEADER_SIZE + SIZE_OCTET) {
            if (charact != STX)
                return "Aucun message n'est code dans cette image.";
        }

        // Ajoute le charactère courant au message.
        message += charact;
    } while (charact != ETX);

    // Efface le premier caractère et le dernier (STX et ETX).
    message = message.substr(1, message.size() - 2);

    delete[] imageData;
    return message;
}

/**
 * Convertie un message ASCII en binaire.
 * @param message < Message à convertir.
 * @return la liste des caractères convertis
 */
void Stegano::convertMessage(string message, bitset<SIZE_OCTET> binaryMessage[]) {

    char lettre;

    // Convertie les lettres ASCII en binaire.
    for (int pos = 0; pos < message.length(); pos++) {
        lettre = message[pos];
        bitset<SIZE_OCTET> binaryCaract(lettre);
        binaryMessage[pos] = binaryCaract;
    }
    return;
}

/**
 * Lit le header du BMP pour en trouver la taille du fichier.
 * @param fileName Chemin vers le fichier.
 * @return Taille du fichier.
 */
int Stegano::getImageSizeFile(string fileName) {

    // Ouvre le fichier en lecture binaire.
    FILE *pFile;
    pFile = fopen(fileName.c_str(), "rb");

    // Vérifie si le fichier est bien ouvert.
    if (pFile == NULL)
        return -1;

    // Lit le header.
    unsigned char header[BMP_HEADER_SIZE] = {0};
    fread(header, sizeof(unsigned char), BMP_HEADER_SIZE, pFile); // read the 54-byte header

    fclose(pFile);

    // Récupère la taille du fichier.
    int sizefile = *(int *) &header[2];

    return sizefile;
}

/**
 * Lit une image BMP et la copie octet par octet dans un tableau.
 * @param binaryImage Tableau d'octet de l'image
 * @return Erreur
 */
int Stegano::getBinaryImage(unsigned char binaryImage[], int sizefile, string fileName) {

    // Ouvre le fichier en lecture binaire.
    FILE *pFile;
    pFile = fopen(fileName.c_str(), "rb");

    // Vérifie si le fichier est bien ouvert.
    if (pFile == NULL)
        return 1;

    // Copie l'image dans un tableau
    fread(binaryImage, sizeof(unsigned char), sizefile, pFile);
    fclose(pFile);

    return 0;
}

/**
 * Code le message dans le tableau de données de l'image puis écrit le fichier BMP
 * @param binaryImage Le tableau comportant chaque octet de l'image à modifier
 * @param binaryMessage Le message converti en binaire devant être codé
 * @param fileSize La taille du fichier total
 * @param binaryMessageArraySize La taille du tableau comprenant le message à codé
 * @return erreur
 */
int Stegano::writeBMP(unsigned char binaryImage[], bitset<SIZE_OCTET> binaryMessage[], int fileSize, int binaryMessageArraySize) {

    //Cache le message dans le tableau de données de l'image;
    int indexImage = BMP_HEADER_SIZE;

    //Parcoure toute les lettres du message.
    for (int indexMessage = 0; indexMessage < binaryMessageArraySize; indexMessage++) {

        // Parcoure Tous les bits d'une lettre composant le message.
        for (int indexBit = 7; indexBit >= 0; indexBit--) {

            // Convertie une occurence du tableau de données de l'image en bitset.
            bitset<SIZE_OCTET> currentColor(binaryImage[indexImage]);

            // Modifie le dernier bit de l'occurence converti avec la valeur du bit de la lettre parcouru.
            currentColor.set(0, binaryMessage[indexMessage].test(indexBit));

            // Ajoute les données modifiées au tableau de données de l'image.
            binaryImage[indexImage] = (char) currentColor.to_ulong();

            // Change d'occurence dans le tableau de données de l'image.
            indexImage++;
        }
    }

    // Créé un fichier image2.bmp
    FILE *pFile;
    pFile = fopen("image2.bmp", "wb");

    // Ecrit le tableau de données modifié dans la nouvelle image.
    fwrite(binaryImage, sizeof(unsigned char), fileSize, pFile);

    fclose(pFile);

    return 0;
}

