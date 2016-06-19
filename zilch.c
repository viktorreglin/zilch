#include <stdio.h>
#include <stdlib.h> //für srand()
#include <time.h> //für time()
#include <unistd.h> //für sleep()
#include <string.h> //für strtok()


// ZILCH the game


//Deklarationen
int legalDice( int dice, int *savedDice, int *points );


//kurz Pause
void takeBreak(  ){
    sleep( 1 );
}


//zeigt den Namen des Spiels
void startScreen(  ){
    takeBreak( );
    printf( "\n#################\n" );
    takeBreak( );
    printf( "##    ZILCH    ##\n" );
    takeBreak( );
    printf( "#################\n" );    
}


//würfelt alle ungespeicherten Würfel
void rollDice( int *rolls, int *savedDice ){
    int i;
    
    for( i = 0; i < 6; i++ ){
        if( savedDice[i] == 0 ) rolls[i] = rand() % 6 + 1;
    }
}


//gibt 1..6 und die dazugehörigen Würfe aus, und noch viel mehr
void printRolls( int *rolls, int *points, int *savedDice, int totalPoints ){
    int i;
    
    printf( "Würfel:     1   2   3   4   5   6\n");
    
    printf( "Ergebnis: " );
    for( i = 0; i < 6; i++ ){
        printf( "%3d ", rolls[i] );
    }
    
    printf( "\n" );
    
    printf( "Punkte:   " );
    for( i = 0; i < 6; i++ ){
        printf( "%3d ", points[i] );
    }
    
    
    printf( "\n" );
    
    printf( "gesp.:    " );
    for( i = 0; i < 6; i++ ){
        printf( "%3d ", savedDice[i] );
    }
    
    printf( "\n\n" );
    
    printf( "Gesamtpunkte: %d\n", totalPoints );
    
    
    printf( "\n\n" );
}


//berechnet Punkte eines Wurfs
int dicePoints( int dice ){
    switch( dice ){
        case 1:
            return 100;
        case 5:
            return 50;
        default:
            return 0;
    }
}


//weist Würfen Punkte zu
void allocatePoints( int *dice, int *points ){
    int i;
    
    for( i = 0; i < 6; i++ ){
        points[i] = dicePoints( dice[i] );
    }
}


//addiert neue Punkte zum Punktestand
int addPoints( int *totalPoints, int newPoints ){
    totalPoints += newPoints;
    //return *totalPoints;
}


//speichert Würfel
void keepDice( int *savedDice, int *points, int *totalPoints ){
    const int LINELENGTH = 80;
    char line[LINELENGTH];
    char *dice;
    char *delimiter = " ";
    int enteredDice;
        
    printf( "Welche Würfel sollen gespeichert werden? Mit Leerzeichen eingeben (z.B. 2 5 6): " );
    fgets( line, sizeof( line ), stdin );
    
    strtok( line, "\n" ); //Zeilenumbruch am Ende loswerden
    
    dice = strtok( line, delimiter );
    
    while( dice != NULL ) {
        enteredDice = atoi( dice ); //String in Integer umwandeln
        if( legalDice( enteredDice, savedDice, points ) ) {
            printf( "Erkannt: %d, legal\n", enteredDice );
            savedDice[ enteredDice - 1 ] = 1;
            *totalPoints += points[ enteredDice - 1 ];
        } else {
            printf( "Erkannt: %d, nicht legal\n", enteredDice );
        }
        dice = strtok( NULL, delimiter );
    }
}


//darf der Würfel gespeichert werden?
int legalDice( int dice, int *savedDice, int *points ){
    int legal;
    
    if( dice < 1 || dice > 6 ) return 0;
    if( points[ dice - 1 ] == 0 ) return 0;
    
    legal = savedDice[ dice - 1 ] == 0 ? 1 : 0;
        
    return legal;
}


//addiert die Punkte der nichtgespeicherten Würfel nach einem Wurf. Falls die Summe 0 ist, ist das Spiel vorbei.
int gameOver( int *points, int *savedDice ){
    int i, sum = 0;
    
    for( i = 0; i < 6; i++ ){
        if( savedDice[ i ] == 0 ) sum += points[ i ];
    }
    
    return sum;
}


//falls alle Würfel gespeichert wurden, wird die Speicherung wieder zurückgesetzt
void fullSave( int *savedDice ){
    int i, sum = 0;
    
    for( i = 0; i < 6; i++ ){
        sum += savedDice[ i ];
    }
    
    if( sum == 6 ){
        for( i = 0; i < 6; i++ ){
            savedDice[ i ] = 0;
        }
    }
}


int main( ){
    int rolls[6] = { 0, 0, 0, 0, 0, 0 };
    int savedDice[6] = { 0, 0, 0, 0, 0, 0 };
    int points[6] = { 0, 0, 0, 0, 0, 0 };
    int totalPoints = 0;
    int wurfNr = 0;
    int c;
    char nochEinWurf[80];
    
    startScreen();

    srand( time( NULL ) ); //Zufallszahlengenerator initialisieren
    
    
    do{
        wurfNr++;
        takeBreak();
        printf( "\nWURF NUMMER %d\n\n", wurfNr );
        rollDice( rolls, savedDice );
        allocatePoints( rolls, points );
        takeBreak();
        printRolls( rolls, points, savedDice, totalPoints );
        if( !gameOver( points, savedDice ) ){
            totalPoints = 0;
            printf( "Leider verloren.\n" );
            break;
        }
        keepDice( savedDice, points, &totalPoints );
        printf( "\n" );
        takeBreak();
        printRolls( rolls, points, savedDice, totalPoints );
        fullSave( savedDice );
        takeBreak();
        printf( "\nErneut würfeln? (j/n) " );
        scanf( "%s", &nochEinWurf[0] );
        while ((c = getchar()) != '\n' && c != EOF); //stdin leeren, sonst nimmt fgets das \n vom scanf
    } while( *nochEinWurf == 'j' );
    
    takeBreak();
    printf( "\nEndpunktzahl: %d\n\n", totalPoints );
        
    return 0;
}
