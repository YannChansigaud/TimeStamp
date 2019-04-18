#include <Wire.h>
#include <TimeStamp.h>

TimeStamp objTimeStamp = TimeStamp();
unsigned long top  =  1000;               // top qui sera incrémenté de 1000ms à chaque fois

unsigned int Annee=0;
unsigned int Jour=0;
unsigned int Heure=0;
unsigned int Minute=0;
unsigned int Seconde=0;
unsigned int Reste=0;


unsigned long top2 = 77350;               // top de 1min 17s 350c et qui sera multiplié par i
unsigned int  i    =     1;
unsigned int tab[6];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  objTimeStamp.compute();
  objTimeStamp.getTimeFromBeginning( &Annee, &Jour, &Heure, &Minute, &Seconde, &Reste );
}

void loop() {
  if( objTimeStamp.compare( top ) ){
    Serial.print( "top " );
    top+=1000;
    objTimeStamp.getTimeFromBeginning( &Annee, &Jour, &Heure, &Minute, &Seconde, &Reste );
    SerialPrintAJHMSR();
  }
  objTimeStamp.split( top2*i, &(tab[0]), &(tab[1]), &(tab[2]), &(tab[3]), &(tab[4]), &(tab[5])  );
  if( objTimeStamp.compare( tab[0], tab[1], tab[2], tab[3], tab[4], tab[5] ) ){                // sera vrai à 1min 17sec 350c
    i++;                                                                                        // puis        2min 34sec 700c
    Serial.print( "test " );                                                                    // puis        3min 52sec  50c
    objTimeStamp.getTimeFromBeginning( &Annee, &Jour, &Heure, &Minute, &Seconde, &Reste );      // puis        5min  9sec 400c
    SerialPrintAJHMSR();                                                                        // puis        6min 26sec 750c
  }
}

void SerialPrintAJHMSR(){
  Serial.print( Annee ); Serial.print( "an " );
  Serial.print( Jour ); Serial.print( "jrs " );
  Serial.print( Heure ); Serial.print( "h " );
  Serial.print( Minute ); Serial.print( "m " );
  Serial.print( Seconde ); Serial.print( "s et " );
  Serial.print( Reste ); Serial.println( "ms" );
}

