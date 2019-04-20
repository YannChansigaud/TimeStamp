# TimeStamp
Librairie permettant de comparer une notion de temps au delà de la limitation de millis() sur Arduino
Cette librairie se comporte comme un métronome ou un minuteur.
Utile pour déclancher une ou des opération(s) à intervale régulier (AutoLoop) comme l'actualisation d'une valeur sur un LCD
ou pour déclancher une seule fois une opération à un instant donné sans pour autant bloquer l'execution du reste du code

Cette classe n'est pas bloquante

set() permet de définir la période ou le délai d'attente
ack() permet de savoir si le délai est arrivé à terme
vars() permet de définir les variables d'état et de temps hors de la classe
restart() permet de remettre le compteur à zéro
setmode() permet de définir le fonctionnement
- AutoLoop : Fonctionne comme un métronome.
  ack() retourne vrai quand le temps sera révolu au moment de l'appel mais retournera faux dès l'appel suivant.
  Pour déclancher plusieurs éléments à des points distincts de l'execution, utiliser des variables externes
- TimeOut : Fonctionne comme un minuteur
  ack() retourne vrai quand le temps sera révolu au moment de l'appel ainsi qu'à chaque appel suivant
