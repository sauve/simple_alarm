#include "Arduino.h"
#include <TM1637.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

class DisplaySequencer
{
  public:
    DisplaySequencer() : tm(CLK, DIO) {}
    void setup(byte luminosite);
    void update();

    
    void ChangeLuminosite(int luminosite);
    void setLuminosite(byte luminosite);
    void FlashDeuxPoints();
    void FlashChiffreDroit();
    void FlashChiffreGauche();
    void DeuxPointsOn(bool on);
    void DisplayOnOff(bool on);


    void AfficheHeure(int heure, int minute); 
    void Affiche(int valeur);
    void Affiche(const char* valeur);
    void Affiche_P(const char* valeur);
    void Clear();

  protected:
    TM1637 tm;
    int lastFlash = 0;
    bool flashDroit = false;
    bool flashGauche = false;
    bool flashCentre = false;

    char buffer[32];
    int bufferlen = 0;
    int bufferidx = 0;
    int scrollspeed = 0;
    int scrolldir = 1;
    byte curluminosite;
    bool needUpdate = false;
    
    
    // valeur affichage
    // devrait avoir le display ici, tout passe par le sequenceur
    // Flash pourrait etre plus un fade ou hard flash
};
