/*
 * TimeStamp.cpp
 * CREATED on TUESDAY 16 AUGUST 2018
 * by CHANSIGAUD YANN
 */


#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <inttypes.h>

#include "TimeStamp.h"

TimeStamp::TimeStamp(){}

void TimeStamp::getTimeFromLastCheck( uint16_t *Annee, uint16_t *Jour, uint16_t *Heure, uint16_t *Minute, uint16_t *Seconde, uint16_t *Reste ){
    compute();  *Annee = pass[ANNEE];  *Jour = pass[JOUR];  *Heure = pass[HEURE];  *Minute = pass[MINUTE];  *Seconde = pass[SECONDE];  *Reste = pass[RESTE];
}

void TimeStamp::getTimeFromBeginning( uint16_t *Annee, uint16_t *Jour, uint16_t *Heure, uint16_t *Minute, uint16_t *Seconde, uint16_t *Reste ){
    compute();  *Annee = time[ANNEE];  *Jour = time[JOUR];  *Heure = time[HEURE];  *Minute = time[MINUTE];  *Seconde = time[SECONDE];  *Reste = time[RESTE];
}

void TimeStamp::split( unsigned long T, uint16_t *Annee, uint16_t *Jour, uint16_t *Heure, uint16_t *Minute, uint16_t *Seconde, uint16_t *Reste ){
    split( T );  *Annee = pass[ANNEE];  *Jour = pass[JOUR];  *Heure = pass[HEURE];  *Minute = pass[MINUTE];  *Seconde = pass[SECONDE];  *Reste = pass[RESTE];
}

uint8_t TimeStamp::getNbLoop(){
	return( nbLoop );
}

void TimeStamp::compute(){                                                               // FONCTION DE CALCUL PRINCIPALE
	TNOW = millis();                                                                     // On récupère millis()
	if( TNOW < TPREV ){                                                                  // On le compare au test précédent. Si inférieur 
		has_loop = true;                                                                 //   alors millis() a rebouclé à zéro
		nbLoop++;                                                                        //   on incrémente le compteur de boucle
	}                                                                                    // 
	else{                                                                                // Sinon
		has_loop = false;                                                                //   on déclare false = "non, pas de rebouclage"
	}                                                                                    // 
	if( has_loop ){    TPASS = TNOW - 0 + ( MAX_ULONG - TPREV );    }                    // Si on a fait un tour, il faut compte le temps précédent jusqu'à la fin PLUS le temps courant depuis 0
	else{              TPASS = TNOW - TPREV;                        }	                 // Sinon, on compte simplement le temps depuis le précédent jusqu'à maintenant
	split( TPASS );                                                                      // On découpe le temps calculé en AJHMSR
	add();                                                                               // On ajoute le temps passé calculé au répertoire de temps 
	TPREV = TNOW;                                                                        // On enregistre le repère temps de CETTE vérification
}                                                                                        // 

void TimeStamp::add(){                                                                   // Ajout séquenciel du temps passé au répertoire de temps
	uint16_t maxi=0;                                                                     // Maxi à ne pas dépasser par échelon
	for( byte i=0 ; i<5 ; i++ ){                                                         //
        switch( 5-i ){                                                                   // Suivant l'échelon calculé
            case JOUR    : {	maxi =  364;    break;	}                                // Jour    : maximum 364 jours par an                365 jours    = 1 an
            case HEURE   : {	maxi =   23;    break;	}                                // Heure   : maximum 23 heures par jour               24 heures   = 1 jour
            case MINUTE  : {	maxi =   59;    break;	}                                // Minute  : maximum 59 minutes par heure             60 minutes  = 1 heure
            case SECONDE : {	maxi =   59;    break;	}                                // Seconde : maximum 59 secondes par minute           60 secondes = 1 minute
            case RESTE   : {	maxi =  999;    break;	}                                // Reste   : maximum 999 millisecondes par seconde  1000 ms       = 1 seconde
        }                                                                                // 
        time[5-i] += pass[5-i] ;                                                         // On ajoute le temps passé au répertoire de temps
        if( time[5-i] > maxi ){                                                          // Si le temps ajouté dépasse le maxi      ex : M = 60
            time[5-i-1] ++;                                                              //   on incrémente l'échelon au dessus     ex : H = H + 1
            time[5-i] -= (maxi+1);                                                       //   on enlève à l'échelon son maximum+1   ex : M = 60-60 = 0
        }                                                                                // 
	}                                                                                    // 
}                                                                                        // 

bool TimeStamp::compare( unsigned long TtC, bool ack ){                                  // Permet de comparer un repère temps en millisecondes vis-à-vis du répertoire de temps;
	if( ack ){    compute();    }                                                        // Si on a demandé d'actualisé time[6], on actualise le répertoire de temps
	split( TtC );                                                                        // On découpe les millisecondes dans temp[6]
	return( compare( pass[ANNEE],  pass[JOUR],    pass[HEURE],                           // 
	                 pass[MINUTE], pass[SECONDE], pass[RESTE], false ) );                // On relay la comparaison sans actualisation de temps (pour ne pas écraser temp)
}                                                                                        // 

bool TimeStamp::compare( uint16_t Ans, uint16_t Jrs, uint16_t Hrs,                       // Permet de comparer AJHMSR au répertoire de temps
                         uint16_t Min, uint16_t Sec, uint16_t Rst, bool ack ){           // 
	if( ack ){    compute();    }                                                        // Si on a demandé d'actualisé time[6], on actualise le répertoire de temps
	if(      Ans > time[ANNEE]   ){        return( false );    }                         // 
	else if( Jrs > time[JOUR]    ){        return( false );    }                         // 
	else if( Hrs > time[HEURE]   ){        return( false );    }                         // 
	else if( Min > time[MINUTE]  ){        return( false );    }                         // 
	else if( Sec > time[SECONDE] ){        return( false );    }                         // 
	else if( Rst > time[RESTE]   ){        return( false );    }                         // 
	else{                                  return( true  );    }                         // 
}                                                                                        // 
void TimeStamp::split( unsigned long TtS ){                                              // Permet de découper un nombre de milliseconde en JHMSR
	pass[JOUR]    = uint16_t( TtS / J_MILLIS );    TtS -= pass[JOUR] * J_MILLIS;         // On calcul le nombre de jour       et on soustrait à TtS
	pass[HEURE]   = uint16_t( TtS / H_MILLIS );    TtS -= pass[HEURE] * H_MILLIS;        // On calcul le nombre d'heure       et on soustrait à TtS
	pass[MINUTE]  = uint16_t( TtS / M_MILLIS );    TtS -= pass[MINUTE] * M_MILLIS;       // On calcul le nombre de minute     et on soustrait à TtS
	pass[SECONDE] = uint16_t( TtS / S_MILLIS );    TtS -= pass[SECONDE] * S_MILLIS;      // On calcul le nombre de seconde    et on soustrait à TtS
	pass[RESTE]   = TtS;	                                                             // Le reste est en milliseconde
}

/*
void TimeStamp::debug( bool ack ){
	if( ack ){    compute();    } 
	Serial.print( "time[] ");
	Serial.print( " A:");  Serial.print( time[ANNEE] );
	Serial.print( " J:");  Serial.print( time[JOUR] );
	Serial.print( " H:");  Serial.print( time[H] );
	Serial.print( " M:");  Serial.print( time[MINUTE] );
	Serial.print( " S:");  Serial.print( time[SECONDE] );
	Serial.print( " R:");  Serial.println( time[RESTE] );
	Serial.print( "pass[] ");
	Serial.print( " A:");  Serial.print( pass[ANNEE] );
	Serial.print( " J:");  Serial.print( pass[JOUR] );
	Serial.print( " H:");  Serial.print( pass[H] );
	Serial.print( " M:");  Serial.print( pass[MINUTE] );
	Serial.print( " S:");  Serial.print( pass[SECONDE] );
	Serial.print( " R:");  Serial.println( pass[RESTE] );
}
*/