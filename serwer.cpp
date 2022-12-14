#include "header.h"

using namespace std;

void ctrlc (int sig){
    exit(0);
}

int main(){

    int sd, buff1, buff2;
    socklen_t clen = sizeof (struct sockaddr_in);

    struct sockaddr_in sad, cad1, cad2;

    signal(SIGINT, ctrlc);

    if ((sd = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket() ERROR");
        exit(1);
    }

    memset((char *) &sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;

    if (inet_pton(AF_INET, IP, &sad.sin_addr) <= 0 ){
        perror("inet_pton() ERROR");
        exit(1);
    }

    sad.sin_port = htons((unsigned short) PORT);

    if (bind (sd, (struct sockaddr *) &sad, sizeof(sad)) < 0){
        perror("bind() ERROR");
        exit(1);
    }

    cout << "Wielki turniej sie rozpoczyna..." << endl;
    int pkt1 = 0, pkt2 = 0, i = 0;
    bool flag = 1;
    unsigned short klient1, klient2;
    map <unsigned short, int> wynik;
    while (true)
    {
        cout << "\t\tROZGRYWKA NR[" << ++i << "]" << endl;
        
        if (recvfrom(sd, (char *) &buff1, sizeof(int), 0,
                      (struct sockaddr *) &cad1, &clen) < 0 ) {
            perror("Client1 ERROR RECIEVING");
            cout << "PORT of Client1: " << cad1.sin_port << endl;
            exit(1);
        }

        if (recvfrom(sd, (char *) &buff2, sizeof(int), 0,
                      (struct sockaddr *) &cad2, &clen) < 0 ) {
            perror("Client2 ERROR RECIEVING");
            cout << "PORT of Client2: " << cad2.sin_port << endl;
            exit(1);
        }

        buff1 = htonl(buff1);
        buff2 = htonl(buff2);
        if (flag){
            wynik[cad1.sin_port] = 0;
            klient1 = cad1.sin_port;
            wynik[cad2.sin_port] = 0;
            klient2 = cad2.sin_port;
            flag = false;
        }

        if (buff1 == buff2) {
            buff1 = buff2 = remis;
        }else if (buff1 == kamien && buff2 == papier) {
            buff1 = przegrana;
            buff2 = wygrana;
        }else if (buff1 == kamien && buff2 == nozyczki) {
            buff1 = wygrana;
            buff2 = przegrana;
        }else if (buff1 == nozyczki && buff2 == papier) {
            buff1 = wygrana;
            buff2 = przegrana;
        }else if (buff1 == nozyczki && buff2 == kamien) {
            buff1 = przegrana;
            buff2 = wygrana;
        }else if (buff1 == papier && buff2 == kamien) {
            buff1 = wygrana;
            buff2 = przegrana;
        }else if (buff1 == papier && buff2 == nozyczki) {
            buff1 = przegrana;
            buff2 = wygrana;
        }else if (buff1 == koniec || buff2 == koniec){
            buff1 = buff2 = koniec;
            pkt1 = pkt2 = i = 0;
        }

        if(buff1 == wygrana) wynik[cad1.sin_port]+=wygrana;
        else if(buff2 == wygrana) wynik[cad2.sin_port]+=wygrana;

        cout << "\t\t\t[" << wynik[klient1] << " - " << wynik[klient2] << "]" << endl;

        if (sendto(sd, (char *) &buff1, sizeof(int), 0,
                   (struct sockaddr *) &cad1, clen) < 0 ) {
            perror("Client1 ERROR SENDING");
            cout << "PORT of Client1: " << cad2.sin_port << endl;
            exit(1);
        }

        if ( sendto(sd, (char *) &buff2, sizeof (int), 0,
                   (struct sockaddr *) &cad2, clen) < 0 ) {
            perror("Client1 ERROR SENDING");
            cout << "PORT of Client2: " << cad2.sin_port << endl;
            exit(1);
        }
    }
    exit (-1);
}
