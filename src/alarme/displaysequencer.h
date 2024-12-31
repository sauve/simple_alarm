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

    void Scroll( int speed, bool bounce, int nbr);


    void AfficheHeure(int heure, int minute); 
    void Affiche(int valeur);
    void Affiche(const char* valeur);
    void Affiche_P(const char* valeur);
    void Clear();

  protected:
    void displaypushbuffer();
  
    TM1637 tm;
    unsigned long lastUpdate;
    int lastFlash = 0;

    char buffer[32];
    int bufferlen = 0;
    int bufferidx = 0;
    int scrollspeed = 0;
    int scrolldir = 1;
    int scrollcount = 0;
    char fillchar = ' ';
    char pushbuffer[5];
    byte curluminosite;
    bool needUpdate = false;
    bool scrollbounce = false;
    bool flashDroit = false;
    bool flashGauche = false;
    bool flashCentre = false;
    
    
    
    // valeur affichage
    // Flash pourrait etre plus un fade ou hard flash
};
