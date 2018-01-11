// Auteur : Meli Léo
// Date   : 15.11.2017
// But    : Cache un message dans une image. Stéganographie

#include <iostream>
#include "Stegano.h"

using namespace std;

int main() {

    string message = "";
    string fileName = "";
    char selection = 0;
    bool endLoop = false;
    bool error = false;

    Stegano stegano;

    // Interface Utilisateur.
    while (!endLoop) {
        cout << "Voulez-vous : " << endl;
        cout << "1) Coder un message dans une image bitmap." << endl;
        cout << "2) Decoder une image bitmap." << endl;
        cout << "3) Quitter le programme." << endl;
        cin >> selection;
        switch (selection) {
            // choix de la fonction de décodage.
            case '1' : {
                cout << "Entrez le chemin menant a l'image a coder : ";
                cin.ignore();
                getline(cin, fileName);
                cout << "Entrez le message que vous voulez coder : ";
                getline(cin, message);
                error = stegano.codeMessage(fileName, message);
                if (error)
                    cout << "L'image est introuvable.\n";
                else
                    cout << "L'image a ete codee avec succes!\n";
                endLoop = true;
                break;
            }

                // Choix de la fonction de codage.
            case '2' : {
                cout << "Entrez le chemin menant a l'image a decoder : " << endl;
                cin.ignore();
                getline(cin, fileName);
                message = stegano.decodeMessage(fileName);
                cout << "Message decode : " << message << endl;
                endLoop = true;
                break;
            }

                // Choix de quitter le programme.
            case '3' : {
                endLoop = true;
                break;
            }
                // Entrée non-valide.
            default : {
                cout << "Aucune entree n'est valide. Tapez 1, 2 ou 3." << endl;
                break;
            }
        }
    }
    return 0;
}



