/*
 TimeStamp.h
 CREATED on WEDNESDAY 17 APRIL 2019
 by CHANSIGAUD YANN

TimeStamp désigne un repère de temps

- La classe ne remet PAS à zéro millis().
- Millis() se remet à zéro tout seul quand il reboucle après avoir atteint son max de 4294967295

- Il y a 2 "répertoire de temps"
- Les répertoires de temps sont des tableaux AJHMSR : Années, Jours, Heures, Minutes, Secondes, Reste
- time[] : Répertoire de temps depuis le début de l'exécution du controleur
- pass[] : Répertoire de temps temporaire. Contient :
           - soit le temps passé depuis le dernier control découpé par split()
		   - soit un temps en ms découpé par split() pour compare()

*/
#ifndef TimeStamp_h
#define TimeStamp_h
#include <inttypes.h>

#define MAX_ULONG   4294967295        // correspond au maximum de millis();
#define J_MILLIS      86400000        // correspond au nombre de millisecondes sur une journée
#define H_MILLIS       3600000        // correspond au nombre de millisecondes sur une heure
#define M_MILLIS         60000        // correspond au nombre de millisecondes sur une minute
#define S_MILLIS          1000        // correspond au nombre de millisecondes sur une seconde

#define ANNEE   0      // année
#define JOUR    1      // jour
#define HEURE   2      // heure
#define MINUTE  3      // minute
#define SECONDE 4      // seconde
#define RESTE   5      // reste (en millisecondes)

class TimeStamp{
  public :
    /*!
    @method
    @abstract   Class constructor. 
    @discussion Constructeur par défaut
    @param      aucun
    */
    TimeStamp();
	/*!
    @method
    @abstract   Fonction accesseur. 
    @discussion Permet de récupérer le temps passé depuis la précédente vérification
    @param      adresse des variables de stockage pour année, jour, heure, minute, seconde et le reste en milliseconde
    */
    void getTimeFromLastCheck( uint16_t *Annee=0, uint16_t *Jour=0, uint16_t *Heure=0, uint16_t *Minute=0, uint16_t *Seconde=0, uint16_t *Reste=0 );
	/*!
    @method
    @abstract   Fonction accesseur. 
    @discussion Permet de récupérer le temps passé depuis le début de l'exécution du microprocesseur
    @param      adresse des variables de stockage pour année, jour, heure, minute, seconde et le reste en milliseconde
    */
    void getTimeFromBeginning( uint16_t *Annee=0, uint16_t *Jour=0, uint16_t *Heure=0, uint16_t *Minute=0, uint16_t *Seconde=0, uint16_t *Reste=0 );
    /*!
    @method
    @abstract   Fonction de comparaison de temps. 
    @discussion Permet de comparer AJHMSR au répertoire de temps
	@discussion Par défaut, le répertoire de temps est actualisé
	@discussion Permet d'effectuer un test qui découlera sur un VRAI que si le repère de temps testé est inférieur au répertoire de temps
	@discussion donc quand le repère de temps testé sera passé
    @param      Annee, Jour, Heure, Minute, Seconde, Reste [uint16_t]
	@param      ack [bool] true  = permet de faire une comparaison AVEC actualisation du répertoire de temps
    @param      ack [bool] false = permet de faire une comparaison SANS actualisation du répertoire de temps
	@return     TRUE  = AJHMSR est inférieur au répertoire de temps
	@return     FALSE = AJHMSR est supérieur au répertoire de temps
	*/
	bool compare( uint16_t Annee, uint16_t Jour, uint16_t Heure, uint16_t Minute, uint16_t Seconde, uint16_t Reste, bool ack=true );
	/*!
    @method
    @abstract   Fonction de comparaison de temps. 
    @discussion Permet de comparer un nombre de millisecondes au répertoire de temps
	@discussion Permet d'effectuer un test qui découlera sur un VRAI que si le repère de temps testé est inférieur au répertoire de temps
	@discussion donc quand le repère de temps testé sera passé
    @param      TtC : TimeToCompare [unsigned long] exprimé en millisecondes
	@param      ack [bool] true  = permet de faire une comparaison AVEC actualisation du répertoire de temps
    @param      ack [bool] false = permet de faire une comparaison SANS actualisation du répertoire de temps
    @return     TRUE  = TtC en millisecondes est inférieur au répertoire de temps
	@return     FALSE = TtC en millisecondes est supérieur au répertoire de temps
	*/
	bool compare( unsigned long TtC, bool ack=true );
    /*!
    @method
    @abstract   Fonction accesseur. 
    @discussion Permet de récupérer le nombre de fois que millis() est repassé par zéro
    @param      aucun
    */
	uint8_t getNbLoop();
    /*!
    @method
    @abstract   Fonction accesseur. 
    @discussion Permet de récupérer le temps passé depuis le début de l'exécution du microprocesseur
    @param      T [unsigned long] timestamp en millisecondes dont le resultat sera retourné dans les paramètres
    @param      adresse des variables de stockage pour année, jour, heure, minute, seconde et le reste en milliseconde
    */
    void split( unsigned long T, uint16_t *Annee, uint16_t *Jour, uint16_t *Heure, uint16_t *Minute, uint16_t *Seconde, uint16_t *Reste );
  	/*!
    @method
    @abstract   Fonction de calcul. 
    @discussion Permet de calculer le temps écoulé depuis la précédente vérification.
	@discussion Verifie si millis() est repassé à zéro
	@discussion Calcul d'abort le temps écoulé et le range dans pass[]
	@discussion Ajoute pass[] à time[] 
    @param      aucun
    */
    void compute();

//    void debug( bool ack=true );
	

  private :
	/*!
    @method
    @abstract   Fonction de calcul. 
    @discussion Permet d'ajouter séquentiellement pass[] à time[] en incrémentant l'échelon supérieur si on dépasse le max de chaque échelon
    @param      aucun
    */
    void add();
	/*!
    @method
    @abstract   Fonction de calcul. 
    @discussion Permet de diviser séquentiellement un nombre de milliseconde en AJHMSR
    @param      TtS : TimeToSplit [unsigned long] exprimé en ms
    */
    void split( unsigned long TtS );
	
	/*!
    @variable
    @abstract   Variable de temps. 
    @discussion Permet de mémoriser la valeur de millis() durant la précédente vérification
    */
    unsigned long TPREV = 0;
	/*!
    @variable
    @abstract   Variable de temps. 
    @discussion Permet de mémoriser la valeur de millis() durant la vérification la 
    */
    unsigned long TNOW  = 0;
	/*!
    @variable
    @abstract   Variable de temps. 
    @discussion Permet de mémoriser la valeur du calcul du temps passé entre TPREV et TNOW
    */
    unsigned long TPASS = 0;
	/*!
    @array
    @abstract   Tableau répertoire de temps
    @discussion Permet de stocker les valeurs AJHMSR du temps écoulé depuis le début de l'exécution du controleur
    */
    unsigned int time[6] = {0,0,0,0,0,0};
	/*!
    @array
    @abstract   Tableau répertoire de temps
    @discussion Permet de stocker les valeurs AJHMSR du temps écoulé depuis la précédente vérification
    */
    unsigned int pass[6] = {0,0,0,0,0,0};
	/*!
    @variable
    @abstract   Variable compteur. 
    @discussion Permet de mémoriser le nombre de fois que millis() est repassé par zéro (incrémenté par compute() )
    */
    unsigned int nbLoop   = 0;
	/*!
    @variable
    @abstract   Variable de vérification. 
    @discussion Variable témoin du teste de vérification si millis() a rebouclé ou pas
	@value      TRUE  = millis() a reblouclé à zéro depuis la précédente vérification
	@value      FALSE = millis() n'a pas reblouclé à zéro
    */
    bool has_loop = false;
};

#endif