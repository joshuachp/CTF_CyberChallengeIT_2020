#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define A "Ehi Ciao!\0"
#define B "Scusami il disturbo\0"
#define C "Sto solo cercando di inizializzare cose\0"
#define D "Non fare SIGINT\0"
#define E "Abbi pietà per i figli\0"
#define F "https://loveforquotes.com/i/2132e323f0eb41908fd2d513ca1b9a6a\0"
#define G "Carina no?!\0"
#define H "Passando alle cose serie...\0"
#define I "Sei pronto?\0"
#define J "E allora let's go!\0"

char* msg[] = {A, B, C, D, E, F, G, H, I, J};
char* url_link = "https://joshuachp.github.io/CTF/CCIT_challenge/\0";

int main() {
    printf(" #####  ####### #######\n"); 
    printf("#     #    #    #\n");
    printf("#          #    #\n");       
    printf("#          #    #####\n");   
    printf("#          #    #\n");       
    printf("#     #    #    #\n");       
    printf(" #####     #    #\n"); 

    printf("\n\nBenvenuto alla nostra challenge!");
    printf("\nInizializzando alcune cose prima dell'inizio!");
    
    printf("\n-------- Starting --------");

    printf("\n||");
    for(int i=1;i<=10;i++) {
        printf("*****");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    char* msg[] = {A, B, C, D, E, F, G, H, I, J};
    for(int i=0; i<10; i++) {

        int child_pid = fork();
        if(child_pid == -1) {
            exit(1);
        }

        if(child_pid == 0) {
            execlp("xterm", "xterm", NULL);
            exit(1);
        }

        printf("%s\n", *(msg+i));
        sleep(2);
    }

    printf("\n--------------------------------\n");
    printf("Quello che dovrete affrontare è un viaggio che vi porterà diverse dimensioni...\n");
    printf("Il tutto inizia qui: %s\n", url_link);
    printf("Attenti a non dimenticare niente, mi ricordo solo di aver usato admin com user!\n");
    printf("See you soon!!\n");
    
    sleep(10);
    printf("\n\nDai non sono cosi cattivo... adesso li chiudo i terminali ...\n");
    system("pkill bash");

    return 0;
}