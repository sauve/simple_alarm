# simple_alarm

Version simplifiee d'un reveil matin

## Portee

+ boitier imprime
+ 4 boutons
+ Afficheur 4 chiffres 7 segments
+ Arduino Nano MCU
+ speaker
+ LED Alarm et PM
+ Module RTC avec baterie


## Fonctionnalite

+ Heure
+ Date
+ Alarme 1 et Alarme 2
+ 12 ou 24 heures

## Code

+ C++
+ Arduino IDE

## Boitier

+ 4 boutons sur le dessus
+ Afficheur 4 digit 7 segment 0.9cm avant centre
+ 3 DEL a cote 7 segment ( PM, Alarm 1 et 2)
+ Espace arriere pour fil USB vers arduino
+ Espace sur le cote pour le hau-parleur ( multi-trour et socle)
+ riser pour visser le protoboard dans le bas


## Librairies Arduino

+ RTC :
+ 7 Segments :


## Configuration

Les configuration sont soit stocker dans le module RTC ou en EEPROM. Les heures et activation des alarmes sont celle au module RTC alors que la luminosite, la duree du snooze et les informations a propos du son sont en EEPROM

## Snooze

Afin de facilte la gestion du snooze tout en conservant les 2 alarmes distincte, la verification est faite selon un comtpeur interne et non selon une date/heure. En cas de perte de courant, le snooze est perdu. Cependant, comme le snooze ne ferme pas l'alarme, au retour du courant, l'alarme sonnera.

