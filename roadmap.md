# Roadmap


## Proto 3

+ Gestion des 4 boutons
  + Etat pour assignation de l'heure
  + Etat pour assignation de la date
  + Etat pour assignation alarme 1
+ Gestion de la config EEPROM
  + luminosite avec changement sur + et -
  + GEstion LED selon luminosite
+ Gestion animation et controle separe de l'affichage
  + scrolling du texte
  + flash gauche, droite, complet

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