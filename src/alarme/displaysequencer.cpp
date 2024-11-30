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
  curluminosite = 5;
  for(int i=0; i < 32; i++)
  {
    buffer[i] = 0;
  }
}

void DisplaySequencer::update()
{
  // valid flash
  // si mise a jour
  if (needUodate)
  {
    // update display
    needUodate = false;
  }
}


void DisplaySequencer::ChangeLuminosite(int luminosite)
{
  // update selon difference en parametre
  // set en EEPROM
  // set update
  int newlum = (int)curluminosite + luminosite;
  if (newlum < 1 ) newlum = 1;
  if (newlum > 8) newlum = 8;
  curluminosite = newlum;
  tm.setBrightness(curluminosite);
}

void DisplaySequencer::setLuminosite(byte luminosite)
{
  curluminosite = luminosite;
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

void DisplaySequencer::DisplayOnOff(bool on)
{
  if (on)
  {
    tm.onMode();
  }
  else
  {
    tm.offMode();
  }
}

void DisplaySequencer::AfficheHeure(int heure, int minute)
{
    // set les valeurs
    // set le flag pour mise a jour
    buffer[0] = '0' + (heure / 10);
    buffer[1] = '0' + (heure % 10);
    buffer[2] = '0' + (minute / 10);
    buffer[3] = '0' + (minute % 10);
    buffer[4] = 0;
    tm.display((const char *)buffer);
    needUodate = true;
}

void DisplaySequencer::Affiche(int valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    tm.display(valeur);
    needUodate = true;
}

void DisplaySequencer::Affiche(const char* valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    tm.display(valeur);
    needUodate = true;
}

void DisplaySequencer::Affiche_P(const char* valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    strcpy_P(buffer, valeur);
    tm.display((const char*)buffer);
    needUodate = true;
}

void DisplaySequencer::Clear()
{
    // clear le display
    // set le flag pour mise a jour
    tm.clearScreen();
    needUodate = true;
}
