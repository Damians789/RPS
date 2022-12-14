#include "header.h"

using namespace std;

void wynik(int w, int l){
    if (w > l)
        cout << "Wygrales te zacieta walke!" << endl;
    else if (l > w)
        cout << "Nie wygrales tej przecietnej potyczki" << endl;
    else
        cout << "Moze nie udalo Ci sie wygrac, ale Twojemu przeciwnikowi rowniez" << endl;
    
    exit(0);
}


int main(){

    int w = 0, l = 0, buff = 0, sd;
    socklen_t slen;

    struct sockaddr_in sad, cad;

    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket() ERROR");
        exit(1);
    }

    memset((char *) &sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;

    if (inet_pton(AF_INET, IP, &sad.sin_addr) <= 0){
        perror("inet_pton() ERROR");
        exit(1);
    }

    sad.sin_port = htons((unsigned short) PORT);
    cad.sin_family = AF_INET;
    cad.sin_port = htons(0);

    slen = sizeof(sad);

    while (true){
        cout << "\t\tWybierz mądrze" << endl << "PAPIER(1)\tNOZYCZKI(2)\tKAMIEN(3)\tKONIEC(9)" << endl;
        cin >> buff;
        // if (buff == koniec) wynik(w, l);

        buff = ntohl(buff);

        if (sendto(sd, (char *) &buff, sizeof(int), 0,
                   (struct sockaddr *) &sad, slen) < 0){
            perror("Client ERROR SENDING");
            cout << "PORT: " << cad.sin_port << endl;
            exit(1);
        }
        // cout << "Oczekiwanie na ruch przeciwnika" << endl;

        if (recvfrom(sd, (char *) &buff, sizeof(int), 0,
                      (struct sockaddr *) &sad, &slen) < 0){
            perror("Client ERROR RECIEVING");
            cout << "PORT: " << cad.sin_port << endl;
            exit(1);
        }

        switch(buff){
            case przegrana:
                ++l;
                cout << "Nie wygrales [" << w << " - " << l << "]" << endl;
                break;
            case remis:
                cout << "Nie wygrales, ani nie przegrales [" << w << " - " << l << "]" << endl;
                break;
            case wygrana:
                ++w;
                cout << "Wygrales [" << w << " - " << l << "]" << endl;
                break;
            case koniec:
                wynik(w, l);
                break;
            default:
                cout << "Wrong data [" << buff << "] ERROR" << endl;
                break;
        }
    }

    return 0;
}