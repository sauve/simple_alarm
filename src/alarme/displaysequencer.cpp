#include "displaysequencer.h"


void DisplaySequencer::setup(byte luminosite)
{
  // initialise le display et la comm
  // clear display par defaut
  // initialisation Afficheur 7 segment
  tm.begin();
  tm.colonOn();
  tm.setBrightness(luminosite);
  curluminosite = luminosite;
  for(int i=0; i < 32; i++)
  {
    buffer[i] = 0;
  }
  for(int i=0; i < 5; i++)
  {
    pushbuffer[i] = 0;
  }
  tm.display((const char*)pushbuffer);
}

void DisplaySequencer::update()
{
  // valid flash
  // si mise a jour
  if (needUpdate)
  {
    // update display
    needUpdate = false;
  }

  if (scrollspeed != 0)
  {
    // si temps de deplacer le texte
    unsigned long elapsetime = (unsigned long)millis() - lastUpdate;
    // deplace idx selon direction
    if ( elapsetime > (unsigned long)scrollspeed )
    {
      // si atteit fin, si bounce sinon reduit count, sinon termine
      if (scrolldir < 0 )
      {
        if (bufferidx == 0)
        {
          if (scrollcount > 0)
          {
            scrollcount -= 1;
          }
          else
          {
            scrollspeed = 0;
          }
        }
        else
        {
          bufferidx -= 1;
        }
      }
      else
      {
        // avance de 1 si idx+4 < lenght
        if ( bufferidx + 4 < bufferlen )
        {
          bufferidx += 1;
        }
        else
        {
          // sinon, si bounce, change dir, sinon count - 1
          if (scrollbounce)
          {
            scrolldir -= 1;
          }
          else
          {
            if (scrollcount > 0)
            {
              scrollcount -= 1;
            }
            else
            {
              scrollspeed = 0;
            }
          }
        }
        
      }
      // update affichage
      lastUpdate = millis();
    }
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

// Scroll le texte tant que la valeur ne change pas
void DisplaySequencer::Scroll( int speed, bool bounce, int nbr)
{
  bufferidx = 0;
  scrollspeed = speed;
  scrolldir = 1;
  scrollcount = nbr;
  lastUpdate = millis();
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
    needUpdate = true;
}

void DisplaySequencer::Affiche(int valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    tm.display(valeur);
    needUpdate = true;
}

void DisplaySequencer::Affiche(const char* valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    tm.display(valeur);
    needUpdate = true;
}

void DisplaySequencer::Affiche_P(const char* valeur)
{
    // set les valeurs
    // set le flag pour mise a jour
    strcpy_P(buffer, valeur);
    tm.display((const char*)buffer);
    needUpdate = true;
}

void DisplaySequencer::Clear()
{
    // clear le display
    // set le flag pour mise a jour
    tm.clearScreen();
    needUpdate = true;
}

void DisplaySequencer::displaypushbuffer()
{
  // copie a partir de bufferidx les 4 ou fin de caractere vers le display
  // remplace de ' ' pour implementer le flash
  // Pad avec des fillcar si n'arrive pas a faire 4 car
  // defini si doit justifier a droite ou gauche
}
