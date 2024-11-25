#include "displaysequencer.h"


void DisplaySequencer::setup()
{
  // va lire la luminosite en EEPROM
  // initialise le display et la comm
  // clear display par defaut
  // initialisation Afficheur 7 segment
  tm.begin();
  tm.colonOn();
  tm.setBrightness(5);

  for(int i=0; i < 32; i++)
  {
    buffer[i] = 0;
  }
}

void DisplaySequencer::update()
{
  // valid flash
  // si mise a jour
  // update display
}


void DisplaySequencer::ChangeLuminosite(byte luminosite)
{
  // update selon difference en parametre
  // set en EEPROM
  // set update
  tm.setBrightness(luminosite);
}


void DisplaySequencer::FlashDeuxPoints()
{}


void DisplaySequencer::FlashChiffreDroit()
{}

void DisplaySequencer::FlashChiffreGauche()
{}


void DisplaySequencer::DeuxPointsOn(bool on)
{
  // set les deux points
  // set le flag pour mise a jour
  if (on)
    tm.colonOn();
  else
    tm.colonOff();
}

void DisplaySequencer::AfficheHeure(int heure, int minute)
{
    // set les valeurs
    // set le flag pour mise a jour
    tm.display(heure * 100 + minute);
}

void DisplaySequencer::Affiche(int valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    tm.display(valeur);
}

void DisplaySequencer::Affiche(const char* valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    tm.display(valeur);
}

void DisplaySequencer::Affiche_P(const char* valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    strcpy_P(buffer, valeur);
    tm.display((const char*)buffer);
}

void DisplaySequencer::Clear()
{
    // clear le display
    // set le flag pour mise a jour
    //tm.clear();
}
