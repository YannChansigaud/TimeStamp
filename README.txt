Librairie permettant de compter et de comparer une notion de temps au delà de la limitation de millis() sur Arduino
- getTimeFromLastCheck() fonction permettant de récupérer le temps écoulé depuis le dernier appel de cette fonction
- getTimeFromBeginning() fonction permettant de récupérer le temps écoulé depuis le démarrage du microcontroleur
- compare() fonction permettant de comparer une notion de temps au temps stocké
- getNbLoop() fonction permettant de connaitre le nombre de fois où millis() a rebouclé à zéro
- split() fonction permettant de récupérer une valeur exprimée en milliseconde sous forme A-J-H-M-S-MS
- compute() fonction d'actualisation des valeurs.

ATTENTION : la méthode compute() doit être appelée au moins 1 fois par millis() soit au moins 1 fois tout les 49 jours environ
