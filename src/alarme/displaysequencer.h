#include "Arduino.h"
#include <TM1637.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

class DisplaySeuqencer
{
  public:
    void setup();
    void update();

    
    void ChangeLuminosite(byte luminosite);
    void FlashDeuxPoints();
    void FlashChiffreDroit();
    void FlashChiffreGauche();
    void DeuxPointsOn(bool on);


    void AfficheHeure(int heure, int minute); 
    void Affiche(int valeur);
    void Affiche(const char* valeur);
    void Affiche_P(const char* valeur);
    void Clear();

  protected:
    TM1637 tm(CLK, DIO);
    int lastFlash = 0;
    bool flashDroit = false;
    bool flashGauche = false;
    bool flashCentre = false;

    char buffer[32];
    
    // valeur affichage
    // devrait avoir le display ici, tout passe par le sequenceur
    // Flash pourrait etre plus un fade ou hard flash
};