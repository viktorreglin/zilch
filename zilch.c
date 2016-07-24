#include <stdio.h>
#include <stdlib.h> //für srand()
#include <time.h> //für time()
#include <unistd.h> //für sleep()
#include <string.h> //für strtok()


// ZILCH the game


//Deklarationen

#define NUMBER_OF_DICE 6
#define LINELENGTH 80

typedef struct{
   int roll;
   int points;
   int saved;

} DIE;

int legalDice( int dice, DIE * wuerfel );


#define NANO_SECOND_MULTIPLIER 1000000


//kurz Pause
void takeBreak( int milliseconds ){
    struct timespec sleepValue;
    sleepValue.tv_sec = 0;
    sleepValue.tv_nsec = milliseconds * NANO_SECOND_MULTIPLIER;
    nanosleep( &sleepValue, NULL);
}


//zeigt den Namen des Spiels
void startScreen(  ){
    takeBreak( 800 );
    printf( "\n#################\n" );
    takeBreak( 800 );
    printf( "##    ZILCH    ##\n" );
    takeBreak( 800 );
    printf( "#################\n" );    
}


//würfelt alle ungespeicherten Würfel
void rollDice( DIE * wuerfel ){
    int i;
    
    for( i = 0; i < NUMBER_OF_DICE; i++ ){
        if( wuerfel[i].saved == 0 )
           wuerfel[i].roll = rand() % 6 + 1;
    }
}


//gibt 1..6 und die dazugehörigen Würfe aus, und noch viel mehr
void printRolls( DIE * wuerfel, int totalPoints ){
    int i;
    takeBreak( 500 );
    printf( "Würfel:  " );
    fflush(stdout);
    for( i = 1; i <= NUMBER_OF_DICE; i++ ){
       takeBreak( 200 );
       printf( "%7d", i );
       fflush(stdout);
    }
    printf( "\n" );
    
    printf( "Ergebnis: " );
    fflush(stdout);
    for( i = 0; i < NUMBER_OF_DICE; i++ ){
       takeBreak( 200 );
       printf( "%6d ", wuerfel[i].roll );
       fflush(stdout);
    }
    
    printf( "\n" );
    
    printf( "Punkte:   " );
    fflush(stdout);
    for( i = 0; i < NUMBER_OF_DICE; i++ ){
       takeBreak( 200 ); 
       printf( "%6d ", wuerfel[i].points );
       fflush(stdout);
    }
    
    
    printf( "\n" );
    
    printf( "gesp.:    " );
    for( i = 0; i < NUMBER_OF_DICE; i++ ){
       takeBreak( 200 );
       printf( "%6d ", wuerfel[i].saved );
       fflush(stdout);
    }
    
    printf( "\n\n" );
    takeBreak( 200 );
    printf( "Gesamtpunkte: %d\n", totalPoints );
    fflush(stdout);
    
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
void allocatePoints( DIE * wuerfel ){
    int i;
    
    for( i = 0; i < NUMBER_OF_DICE; i++ ){
        wuerfel[i].points = dicePoints( wuerfel[i].roll );
    }
}


//speichert Würfel
void keepDice( DIE * wuerfel, int * totalPoints ){
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
        if( legalDice( enteredDice, wuerfel ) ) {
            printf( "Erkannt: %d, legal\n", enteredDice );
            wuerfel[enteredDice - 1].saved = 1;
            *totalPoints += wuerfel[enteredDice - 1].points;
        } else {
            printf( "Erkannt: %d, nicht legal\n", enteredDice );
        }
        dice = strtok( NULL, delimiter );
    }
}


//darf der Würfel gespeichert werden?
int legalDice( int dice, DIE * wuerfel ){
    int legal;
    
    if( dice < 1 || dice > 6 ) return 0;
    if( wuerfel[dice - 1].points == 0 ) return 0;
    
    legal = wuerfel[dice - 1].saved == 0 ? 1 : 0;
        
    return legal;
}


//addiert die Punkte der nichtgespeicherten Würfel nach einem Wurf. Falls die Summe 0 ist, ist das Spiel vorbei.
int gameOver( DIE * wuerfel ){
    int i, sum = 0;
    
    for( i = 0; i < NUMBER_OF_DICE; i++ ){
        if( wuerfel[i].saved == 0 ) sum += wuerfel[i].points;
    }
    
    return sum;
}


//falls alle Würfel gespeichert wurden, wird die Speicherung wieder zurückgesetzt
void fullSave( DIE * wuerfel ){
    int i, sum = 0;
    
    for( i = 0; i < NUMBER_OF_DICE; i++ ){
        sum += wuerfel[i].saved;
    }
    
    if( sum == NUMBER_OF_DICE ){
        for( i = 0; i < NUMBER_OF_DICE; i++ ){
            wuerfel[i].saved = 0;
        }
    }
}



//schreibt Punktzahl in Datei
void exportScores( int totalPoints ){
   FILE *scoresFile;
   time_t now = time( NULL );
   
   scoresFile = fopen( "highscores.txt", "a" );
   fprintf( scoresFile, "%d %s %s", totalPoints, getenv("USER"), asctime(gmtime(&now)) );
   fclose( scoresFile );
}


//gibt bisherigen Highscore zurück
int currentHighscore(){
   FILE *scoresFile;
   char line[LINELENGTH];
   int score = 0;
   int highscore = 0;
   
   scoresFile = fopen( "highscores.txt", "ab+" );
   
   while( !feof( scoresFile ) ){
      fgets( line, LINELENGTH, scoresFile );
      strtok( line, " " );
      score = atoi( line );
      if( score > highscore ){
         highscore = score;
      }
   }
   
   fclose( scoresFile );
   
   return highscore;
}



int main( ){  
   DIE wuerfel[NUMBER_OF_DICE] = {};
   char nochEinWurf[LINELENGTH];
   int c, wurfNr = 0, totalPoints = 0;
   int highscore = 0;
   
   srand( time( NULL ) ); //Zufallszahlengenerator initialisieren


   
   startScreen();
   
   highscore = currentHighscore();
   takeBreak( 800 );
   printf( "\nHighscore: %d\n", highscore );
   takeBreak( 800 );

    do{
        wurfNr++;
        
        takeBreak( 500 );
        
        printf( "\nWURF NUMMER %d\n\n", wurfNr );
        
        rollDice( wuerfel );
        
        allocatePoints( wuerfel );
        
        takeBreak( 500 );
        
        printRolls( wuerfel, totalPoints );
        
        if( !gameOver( wuerfel ) ){
            totalPoints = 0;
            printf( "Leider verloren.\n" );
            break;
        }
        
        keepDice( wuerfel, &totalPoints );
        
        printf( "\n" );
        
        takeBreak( 500 );
        
        printRolls( wuerfel, totalPoints );
        
        fullSave( wuerfel );
        
        takeBreak( 500 );
        
        printf( "\nErneut würfeln? (j/n) " );
        
        scanf( "%s", &nochEinWurf[0] );
        
        while ((c = getchar()) != '\n' && c != EOF); //stdin leeren, sonst nimmt fgets das \n vom scanf
        
    } while( *nochEinWurf == 'j' );
    
    takeBreak( 500 );
    printf( "\nEndpunktzahl: %d\n\n", totalPoints );
    
    if( totalPoints > highscore ) {
       takeBreak( 500 );
       printf( "NEUER HIGHSCORE!\n\n" );
    }
    
    exportScores( totalPoints );
        
    return 0;
}
