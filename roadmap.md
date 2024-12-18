# Roadmap


## Alpha 2 
+ Optimisation de la memoire Flash
  + Utiliser 16 bits par note + duree (signed byte + tableau pour frequence en eeprom)
+ Ajouter de le son d'alarme standard (Beep repetitif)


## Alpha 1

+ Gestion du snooze
  + Plus ou moins sont le Clear de la bonne alarme, le reste c'est le snooze
+ Gestion de la config EEPROM
  + luminosite avec changement sur + et -
  + Gestion LED selon luminosite ( max lum)
  + longeur du snooze
  + musique associe a l'alarme
  + Configuration via conf + Plus + moins
+ Gestion animation et controle separe de l'affichage
  + scrolling du texte
    - Date complete a la fin de set date et a minuit ou sur OK
  + flash gauche, droite, complet
+ help retourne la liste de commande USB
  + Menage sur les commande de test


## Proto 3

+ Gestion des 4 boutons
  + Etat pour assignation de l'heure
  + Etat pour assignation de la date
  + Etat pour assignation alarme 1

## Proto 2

+ Meilleure librairie pour le 7 segments
  + Utilisation de la meme finalement
+ Commande via serial
  + Permettre la configuration et la lecture d'information via des commande USB
    + time, date, alarne 1 et 2
    + test speaker
  + Utiliser des comparaison en PROGMEM pour optimiser l'utilisation de la RAM
+ Multiple handler
  + Ajout du setting de l'heure
+ Gestion simple du speaker
  + beep selon un tone et une durée


# Terminé 

## Proto 1
  + Connecter via breadboard et UNO les composants
+ Valider la communication 
  + Lecture de l'heure du RTC
  + Update des boutons
+ Implemeter version test des classe de gestion de composants
  + LED
  + Speaker
  + 7 segment
+ Test classe handler
  + une seule classe
  + Permet des changements sur les gestionnaires de composants