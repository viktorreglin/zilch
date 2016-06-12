#include <stdio.h>
#include <stdlib.h> //für srand( )
#include <time.h> //für time( )
#include <unistd.h> //für sleep()

// ZILCH the game


//Deklarationen
int legalDice( int dice, int *savedDice );


//kurz Pause
void takeBreak(  ){
    sleep( 1 );
}


//zeigt den Namen des Spiels
void startScreen(  ){
    takeBreak;
    printf( "#################\n" );
    takeBreak;
    printf( "##    ZILCH    ##\n" );
    takeBreak;
    printf( "#################\n" );
    takeBreak;
    
}


//würfelt count mal
void rollDice( int *buf, int count ){
    int i;
    
    for( i = 0; i < count; i++ ){
        buf[i] = rand() % 6 + 1;
    }
}


//gibt 1..6 und die dazugehörigen Würfe aus
void printRolls( int *rolls, int *savedDice ){
    int i;
    
    printf( "Würfel:   1 2 3 4 5 6\n");
    
    printf( "Ergebnis: " );
    for( i = 0; i < 6; i++ ){
        printf( "%d ", rolls[i] );
    }
    printf( "\n" );
    
    printf( "gesp.:    " );
    for( i = 0; i < 6; i++ ){
        printf( "%d ", savedDice[i] );
    }
    
    
    printf( "\n" );
}


//weist einem Wurf Punkte zu
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


//addiert neue Punkte zum Punktestand
int addPoints( int *totalPoints, int newPoints ){
    totalPoints += newPoints;
    return *totalPoints;
}


//fragt welche WÜrfel gespeichert werden sollen und speichert diese
int saveADice( int *savedDice ){
    int wuerfel;
    
    for( ;; ){
        printf( "Welcher Würfel soll gespeichert werden? " );
        scanf( "%d", &wuerfel);
        
        if( legalDice( wuerfel, savedDice) ) break;
        else printf( "Würfel kann nicht gespeichert werden.\n" );
    }
    
    savedDice[wuerfel-1] = 1;
    return wuerfel - 1;
}


//speichert Würfel
void keepDice( int *savedDice ){
    const int LINELENGTH = 80;
    char line[LINELENGTH];
    char *dice;
    char delimiter = " ";
    
    
    printf( "Welche Würfel sollen gespeichert werden? Mit Leerzeichen eingeben (z.B.: 2 5 6):\n" );
    fgets( line, sizeof(line), stdin );
    
    strtok( line, "\n" ); //Zeilenumbruch am Ende loswerden
    
    dice = strtok( line, delimiter );
    savedDice[ atoi( dice ) - 1 ] = 1;
    
    while( dice != NULL ) {
        savedDice[ atoi( dice ) - 1 ] = 1;
        dice = strtok( NULL, delimiter );
    }

}


//darf der Würfel gespeichert werden?
int legalDice( int dice, int *savedDice ){
    int legal;
    
    legal = savedDice[dice-1] == 0 ? 1 : 0;
        
    return legal;
}


int main( ){
    int rolls[6] = { 0, 0, 0, 0, 0, 0 };
    int savedDice[6] = { 0, 0, 0, 0, 0, 0 };
    int nrDice = 6;
    int totalPoints = 0;
    int currentWuerfel, wuerfelPunkte;
    
    startScreen();

    srand( time( NULL ) ); //Zufallszahlengenerator initialisieren
    
    printf( "\nErster Wurf: \n" );
    takeBreak;
    
    rollDice( rolls, nrDice );
    takeBreak;
    
    for( ;; ){
        printRolls( rolls, savedDice );
        currentWuerfel = saveADice( savedDice );
        
        wuerfelPunkte = dicePoints( rolls[currentWuerfel] );
        printf( "Würfelpunkte: %d \n", wuerfelPunkte );
        
        totalPoints += wuerfelPunkte;
        
        printf( "Gesamtpunkte: %d \n", totalPoints );
    }
    
    return 0;
}
