// Simple Alarme

#include <DS3231.h>
#include <Wire.h>
#include <inttypes.h>
#include "alarmconfig.h"
#include "commandstring.h"
#include "ledsequencer.h"
#include "speakersequencer.h"
#include "displaysequencer.h"
#include "gestbouton.h"

/// command string
const char settime_cmdstr[] PROGMEM = {"settime"};
const char setdate_cmdstr[] PROGMEM = {"setdate"};
const char gettimedate_cmdstr[] PROGMEM = {"gettimedate"};
const char getalarm1_cmdstr[] PROGMEM = {"getalarm1"};
const char setalarm1_cmdstr[] PROGMEM = {"setalarm1"};
const char setalarm1on_cmdstr[] PROGMEM = {"setalarm1on"};
const char clearalarm1_cmdstr[] PROGMEM = {"clearalarm1"};
const char getalarm2_cmdstr[] PROGMEM = {"getalarm2"};
const char setalarm2_cmdstr[] PROGMEM = {"setalarm2"};
const char ledon_cmdstr[] PROGMEM = {"ledon"};
const char temp_cmdstr[] PROGMEM = {"temp"};
const char beep_cmdstr[] PROGMEM = {"beep"};
const char play_cmdstr[] PROGMEM = {"play"};
const char stop_cmdstr[] PROGMEM = {"stop"};
const char song_cmdstr[] PROGMEM = {"song"};
const char clearconf_cmdstr[] PROGMEM = {"clearconfig"};
const char showconf_cmdstr[] PROGMEM = {"showconfig"};

const char lundi_str[] PROGMEM = {"lundi"};
const char mardi_str[] PROGMEM = {"Hardi"}; 
const char mercredi_str[] PROGMEM = {"Hercredi"}; 
const char jeudi_str[] PROGMEM = {"jeudi"}; 
const char vendredi_str[] PROGMEM = {"uendredi"}; 
const char samedi_str[] PROGMEM = {"saHedi"}; 
const char dimanche_str[] PROGMEM = {"diHanche"};
const char janvier_str[] PROGMEM = {"januier"}; 
const char fevrier_str[] PROGMEM = {"feurier"}; 
const char mars_str[] PROGMEM = {"Hars"}; 
const char avril_str[] PROGMEM = {"auril"}; 
const char mai_str[] PROGMEM = {"Hai"}; 
const char juin_str[] PROGMEM = {"juin"}; 
const char juillet_str[] PROGMEM = {"juillet"}; 
const char aout_str[] PROGMEM = {"aout"};
const char septembre_str[] PROGMEM = {"septeHbre"};     ; 
const char octobre_str[] PROGMEM = {"octobre"}; 
const char novembre_str[] PROGMEM = {"nouehbre"}; 
const char decembre_str[] PROGMEM = {"deceHbre"}; 

// nombre de jour par mois, sans etre une annee bissextile
const byte nbrjourmois[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Information de version
#define MAJOR_VERSION 0
#define MINOR_VERSION 4
#define REVISON_VERISON 0
//const char device_str[] PROGMEM = {"Reveil matin maison chose"}; 
//const char version_str[] PROGMEM = {"Version: "}; 

// Retourne le nombre de jour pour un mois d'une annee donnee
byte getNombreJourMois(int mois, int annee)
{
  if (mois > 12) return 0;
  if (mois == 1)
  {
    // si divisible par 4 sauf divisible par 100, mais commme le support passee 2099 est pas pour cette version bien...
    if ( (annee % 4) == 0)
    {
      return pgm_read_byte_near(nbrjourmois + 1) + 1;
    }
    return pgm_read_byte_near(nbrjourmois + 1);
  }
  return pgm_read_byte_near(nbrjourmois + mois);
}

void GenereDateString(int annee, int mois, int jour, int dow, int heure, int minute, char *buffer)
{
    // doit remettre les tableau de jour de semaine et de mois en tant qu'array en progmem
}


class GestionEtat
{
  public:
    virtual void EnterState() = 0;
    virtual void HandleButtons() = 0;
    virtual void HandleState() = 0;
};


unsigned long lastUpdateTime = 0;
unsigned long curUpdateTime = 0;
DS3231 myRTC;
GestionEtat* EtatCourant = nullptr;

AlarmConfig config;
GestionBoutons boutons;
DisplaySequencer display;
LEDSequencer leds;
SpeakerSequencer speaker;

// methode utilitaire hors etat


class GestionAfficheHeure : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();

    // nethode pour verifier si snooze et clear flag alarme
    bool isInSnooze1()
    {
      return alarme1on && alarme1snooze;
    }

    bool isInSnooze2()
    {
      return alarme2on && alarme2snooze;
    }

    void setSnooze1()
    {
      // ajout x minute selon le config
      snooze1count = (long)config.getSnoozeDelay() * (1000L * 60L);
      alarme1snooze = true;
      speaker.Stop();
    }

    void setSnooze2()
    {
      snooze2count = (long)config.getSnoozeDelay() * (1000L * 60L);
      alarme2snooze = true;
      speaker.Stop();
    }

    void clearAlarme(byte alarme );

  protected:
    unsigned long lastUpdate = 0; 
    long snooze1count = 0;
    long snooze2count = 0;
    int presedconf = 0;
    bool alarme1on = false;
    bool alarme2on = false;
    bool alarme1snooze = false;
    bool alarme2snooze = false;

    void updateAlarmes( long difftime)
    {
      if (alarme1on)
      {
        if (alarme1snooze)
        {
          snooze1count -= difftime;
          if (snooze1count <= 0)
          {
            snooze1count = 0;
            alarme1snooze = false;
            // declenche la musique
            speaker.Stop();
            speaker.setSong(config.getAlarm1Song());
            speaker.Start();
          }
        }
      }
      else
      {
        if (myRTC.checkAlarmEnabled(1))
        {
          // si alarme enable, verifie si est atteinte
          if (myRTC.checkIfAlarm(1, false) )
          {
            alarme1on = true;
            // declenche la musique
            speaker.Stop();
            speaker.setSong(config.getAlarm1Song());
            speaker.Start();
        }
      }

      if (alarme2on)
      {
        if (alarme2snooze)
        {
          snooze2count -= difftime;
          if (snooze2count <= 0)
          {
            snooze2count = 0;
            alarme2snooze = false;
            // declenche la musique
            speaker.Stop();
            speaker.setSong(config.getAlarm2Song());
            speaker.Start();
          }
        }
      }
      else
      {
        if (myRTC.checkAlarmEnabled(2))
        {
          // si alarme enable, verifie si est atteinte
          if (myRTC.checkIfAlarm(2, false) )
          {
            alarme2on = true;
            // declenche la musique
            speaker.Stop();
            speaker.setSong(config.getAlarm2Song());
            speaker.Start();
          }
        }
      }
    }
  }

};

class GestionConfigHeure : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();
  protected:
    int nouvelleheure = 0;
    int nouvelleminte = 0;
    byte heureouminute = 0;
};

class GestionConfigDate : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();
  protected:
    int nouvelleannee = 0;
    int nouveaumois = 0;
    int nouveaujour = 0;
    int nouveaudow = 0;
    byte etapeconfig = 0;
};

class GestionConfigAlarme : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();

    void setAlarme(byte alarm);
  protected:
    int nouvelleheure = 0;
    int nouvelleminte = 0;
    byte heureouminute = 0;
    byte curAlarme = 1;
    bool onoff = false;
};

class GestionConfigOption : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();
  protected:
    byte etapeoption = 0;
    byte newlum;
    byte newsnooze;
    byte newa1song;
    byte newa2song;
};

class GestionJukebox : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();
  protected:
    byte cursong;
};

GestionAfficheHeure EtatAfficheHeure;
GestionConfigHeure EtatConfigHeure;
GestionConfigAlarme EtatConfigAlarme;
GestionConfigDate EtatConfigDate;
GestionConfigOption EtatConfigOption;
GestionJukebox EtatJukebox;

void GestionAfficheHeure::EnterState()
{
  
}

void GestionAfficheHeure::clearAlarme( byte alarme )
{
  // arrete la musique
  if (alarme == 0)
  {
    speaker.Stop();
    // clear  
    myRTC.checkIfAlarm(1);
    alarme1on = false;
    alarme1snooze = false;
  }
  else if (alarme == 1)
  {
    speaker.Stop();
    // clear  
    myRTC.checkIfAlarm(2);
    alarme2on = false;
    alarme2snooze = false;
  }
}

void GestionAfficheHeure::HandleButtons()
{
  // si presentement alarm on, fait la gestion du clear ou snooze

  if ( boutons.justPressed(BTN_CONF) )
  {
    if (alarme2on & !isInSnooze2())
    {
      setSnooze2();
    }
    else if (alarme1on & !isInSnooze1())
    {
      setSnooze1();
    }
    else
    {
      presedconf = 1;
    }
  }
  else if ( boutons.stillPressed(BTN_CONF) )
  {
    presedconf++;
  }
  else
  {
    presedconf = 0;  
  }

  
  // si alarme n'est pas active, change luminosit, sinon ferme alarme
  if (presedconf == 0 )
  {
    if (boutons.justPressed(BTN_PLUS) )
    {
      if (alarme1on)
      {
        clearAlarme(0);
      }
      else if (alarme2on & !isInSnooze2())
      {
        setSnooze2();
      }
      else 
      {
        display.ChangeLuminosite(1);
        leds.ChangeLuminosite(1);
      }
    }
    if (boutons.justPressed(BTN_MOINS) )
    {
      if (alarme2on)
      {
        clearAlarme(1);
      }
      else if (alarme1on & !isInSnooze1())
      {
        setSnooze1();
      }
      else 
      {
        display.ChangeLuminosite(-1);
        leds.ChangeLuminosite(-1);
      }
    }
    if (boutons.justPressed(BTN_OK) )
    {
      if (alarme2on & !isInSnooze2())
      {
        setSnooze2();
      }
      else if (alarme1on & !isInSnooze1())
      {
        setSnooze1();
      }
    }
  }

  if( presedconf == 200 )
  {
    // si OK, Plus ou moins sont aussi appuyer pour la configuration
    presedconf = 0;  
    if ( boutons.isPressed( BTN_OK) && boutons.isPressed( BTN_PLUS) && boutons.isPressed( BTN_MOINS))
    {
      EtatJukebox.EnterState();
      EtatCourant = &EtatJukebox;
    }
    else if ( boutons.isPressed( BTN_PLUS) && boutons.isPressed( BTN_MOINS))
    {
      EtatConfigOption.EnterState();
      EtatCourant = &EtatConfigOption;
    }
    else if (  boutons.isPressed( BTN_OK) )
    {
      EtatConfigDate.EnterState();
      EtatCourant = &EtatConfigDate;
    }
    else if ( boutons.isPressed( BTN_PLUS) )
    {
      // Change l'etat
      EtatConfigAlarme.setAlarme(0);
      EtatConfigAlarme.EnterState();
      EtatCourant = &EtatConfigAlarme;
    }
    else if ( boutons.isPressed( BTN_MOINS) )
    {
      // Change l'etat
      EtatConfigAlarme.setAlarme(1);
      EtatConfigAlarme.EnterState();
      EtatCourant = &EtatConfigAlarme;
    }
    else
    {
      EtatConfigHeure.EnterState();
      EtatCourant = &EtatConfigHeure;
    }
  }
}

void GestionAfficheHeure::HandleState()
{
  // affiche l'heure
  if ( curUpdateTime - lastUpdate > 500 )
  {
    updateAlarmes(curUpdateTime - lastUpdate);

    // sinon update l'heure 
    lastUpdate = curUpdateTime;
    bool h12Flag;
    bool pmFlag;
    int hour = myRTC.getHour(h12Flag, pmFlag);
    int minute = myRTC.getMinute();
    int seconde = myRTC.getSecond();
    if ( (seconde % 2) == 0)
    {
      display.DeuxPointsOn(false);
    } 
    else
    {
      display.DeuxPointsOn(true);
    }
    display.AfficheHeure(hour, minute);

    // set la led pm
    if (pmFlag && h12Flag)
    {
      leds.SetPM(true, 0);
    }
    else
    {
      leds.SetPM(false, 0);
    }

    // set les led selon l'alarme
    if (isInSnooze1())
    {
      leds.SetAlarm1(true, 4);
    }
    else if (alarme1on)
    {
      leds.SetAlarm1(true, 24);
    }
    else if (myRTC.checkAlarmEnabled(1))
    {
      leds.SetAlarm1(true, 0);
    }
    else
    {
      leds.SetAlarm1(false, 0);
    }

    if (isInSnooze2())
    {
      leds.SetAlarm2(true, 4);
    }
    else if (alarme2on)
    {
      leds.SetAlarm2(true, 24);
    }
    else if (myRTC.checkAlarmEnabled(2))
    {
      leds.SetAlarm2(true, 0);
    }
    else
    {
      leds.SetAlarm2(false, 0);
    }

  }
}

void GestionConfigHeure::EnterState()
{
  Serial.println(F("Etat config heure"));
  heureouminute  = 0;
  bool h12Flag;
  bool pmFlag;
  nouvelleheure = myRTC.getHour(h12Flag, pmFlag);
  nouvelleminte = myRTC.getMinute();
  display.DeuxPointsOn(true);
}

void GestionConfigHeure::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
  {
    EtatAfficheHeure.EnterState();
    EtatCourant = &EtatAfficheHeure;
  }
  else if ( boutons.justPressed(BTN_OK) )
  {
    heureouminute += 1;
    if ( heureouminute > 1 )
    {
      // set l'heure acutelle avec les nouvelles valeurs
      myRTC.setClockMode(false);  // set to 24h
      myRTC.setHour(nouvelleheure);
      myRTC.setMinute(nouvelleminte);
      myRTC.setSecond(0);
      EtatAfficheHeure.EnterState();
      EtatCourant = &EtatAfficheHeure;
    }
  }
  else if ( boutons.justPressed(BTN_PLUS) )
  {
    if ( heureouminute == 0 )
    {
      nouvelleheure += 1;
      if (nouvelleheure > 23)
      {
        nouvelleheure = 0;
      }
    }
    else
    {
      nouvelleminte += 1;
      if (nouvelleminte > 59)
      {
        nouvelleminte = 0;
      }
    }
  }
  else if ( boutons.justPressed(BTN_MOINS) )
  {
    if ( heureouminute == 0 )
    {
      nouvelleheure -= 1;
      if (nouvelleheure < 0)
      {
        nouvelleheure = 23;
      }
    }
    else
    {
      nouvelleminte -= 1;
      if (nouvelleminte < 0)
      {
        nouvelleminte = 59;
      }
    }
  }
}


void GestionConfigHeure::HandleState()
{
  char outstr[5];
  outstr[4] = 0;
  if (heureouminute == 0)
  {
    outstr[2] = 'h';
    outstr[3] = 'h';
    outstr[0] = '0' + ((nouvelleheure % 100)/10);
    outstr[1] = '0' + (nouvelleheure % 10);
    display.Affiche(outstr);
  }
  else
  {
    outstr[0] = 'H';
    outstr[1] = 'H';
    outstr[2] = '0' + ((nouvelleminte % 100)/10);
    outstr[3] = '0' + (nouvelleminte % 10);
    display.Affiche(outstr);
  }
}


void GestionConfigAlarme::EnterState()
{
  bool pmFlag;
  byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
  bool alarmDy, alarmH12Flag, alarmPmFlag;
 
  heureouminute = 0;
  if ( curAlarme == 0)
  {
    myRTC.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
    nouvelleheure = alarmHour;
    nouvelleminte = alarmMinute;
    onoff = myRTC.checkAlarmEnabled(1);
    leds.SetAlarm1(true, 4);
  }
  else
  {
    myRTC.getA2Time(alarmDay, alarmHour, alarmMinute, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
    nouvelleheure = alarmHour;
    nouvelleminte = alarmMinute;
    onoff = myRTC.checkAlarmEnabled(2); 
    leds.SetAlarm2(true, 4);
  }
  display.DeuxPointsOn(true);
}


void GestionConfigAlarme::setAlarme(byte alarm)
{
  curAlarme = alarm;
  if (curAlarme >1)
  {
    curAlarme = 0;
  }
}

void GestionConfigAlarme::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
  {
    EtatAfficheHeure.EnterState();
    EtatCourant = &EtatAfficheHeure;
  }
  else if ( boutons.justPressed(BTN_OK) )
  {
    heureouminute += 1;
    if ( heureouminute > 2 )
    {
      // set l'alarm avec les nouvelles valeurs
      if ( curAlarme == 0)
      {
        myRTC.setA1Time(1, nouvelleheure, nouvelleminte, 0, 0b00001000, true, false, false);
        if (onoff)
        {
          myRTC.turnOnAlarm(1);
        }
        else
        {
          myRTC.turnOffAlarm(1);
        }
      }
      else
      {
        myRTC.setA2Time(1, nouvelleheure, nouvelleminte, 0b01000000, true, false, false);
        if (onoff)
        {
          myRTC.turnOnAlarm(2);
        }
        else
        {
          myRTC.turnOffAlarm(2);
        }
      }
      
      EtatAfficheHeure.EnterState();
      EtatCourant = &EtatAfficheHeure;
    }
  }
  else if ( boutons.justPressed(BTN_PLUS) )
  {
    if ( heureouminute == 0 )
    {
      nouvelleheure += 1;
      if (nouvelleheure > 23)
      {
        nouvelleheure = 0;
      }
    }
    else if (heureouminute == 1)
    {
      nouvelleminte += 1;
      if (nouvelleminte > 59)
      {
        nouvelleminte = 0;
      }
    }
    else
    {
      // on ou off
      onoff = true;
    }
  }
  else if ( boutons.justPressed(BTN_MOINS) )
  {
    if ( heureouminute == 0 )
    {
      nouvelleheure -= 1;
      if (nouvelleheure < 0)
      {
        nouvelleheure = 23;
      }
    }
    else if (heureouminute == 1)
    {
      nouvelleminte -= 1;
      if (nouvelleminte < 0)
      {
        nouvelleminte = 59;
      }
    }
    else
    {
      // on ou off
      onoff = false;
    }
  }
}

void GestionConfigAlarme::HandleState()
{
  char outstr[5];
  outstr[4] = 0;
  if (heureouminute == 0)
  {
    outstr[0] = '0' + ((nouvelleheure % 100)/10);
    outstr[1] = '0' + (nouvelleheure % 10);
    outstr[2] = 'h';
    outstr[3] = 'h';
    display.Affiche(outstr);
  }
  else if (heureouminute == 1)
  {
    outstr[0] = 'H';
    outstr[1] = 'H';
    outstr[2] = '0' + ((nouvelleminte % 100)/10);
    outstr[3] = '0' + (nouvelleminte % 10);
    display.Affiche(outstr);
  }
  else
  {
    if (onoff)
    {
      display.Affiche("On  ");
    }
    else
    {
      display.Affiche("Off ");
    }
  }
}


void GestionConfigDate::EnterState()
{
  bool century = false;
  nouvelleannee = myRTC.getYear();
  nouveaumois = myRTC.getMonth(century);
  nouveaujour = myRTC.getDate();
  nouveaudow = myRTC.getDoW();
  etapeconfig = 0;
  display.DeuxPointsOn(false);
}

void GestionConfigDate::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
  {
    EtatAfficheHeure.EnterState();
    EtatCourant = &EtatAfficheHeure;
  }
  else if ( boutons.justPressed(BTN_OK) )
  {
    etapeconfig += 1;
    if ( etapeconfig > 3 )
    {
      // set l'alarm avec les nouvelles valeurs
      myRTC.setYear(nouvelleannee);
      myRTC.setMonth(nouveaumois);
      myRTC.setDate(nouveaujour);
      myRTC.setDoW(nouveaudow);
      
      EtatAfficheHeure.EnterState();
      EtatCourant = &EtatAfficheHeure;
    }
  }
  else if ( boutons.justPressed(BTN_PLUS) )
  {
    if ( etapeconfig == 0 )
    {
      nouvelleannee += 1;
      if (nouvelleannee > 99)
      {
        nouvelleannee = 0;
      }
    }
    else if (etapeconfig == 1)
    {
      nouveaumois += 1;
      if (nouveaumois > 12)
      {
        nouveaumois = 1;
      }
    }
    else if (etapeconfig == 2)
    {
      nouveaujour += 1;
      if (nouveaujour > getNombreJourMois(nouveaumois, nouvelleannee))
      {
        nouveaujour = 1;
      }
    }
    else if (etapeconfig == 3)
    {
      nouveaudow += 1;
      if (nouveaudow > 6)
      {
        nouveaudow = 0;
      }
    }
  }
  else if ( boutons.justPressed(BTN_MOINS) )
  {
    if ( etapeconfig == 0 )
    {
      nouvelleannee -= 1;
      if (nouvelleannee < 0)
      {
        nouvelleannee = 99;
      }
    }
    else if (etapeconfig == 1)
    {
      nouveaumois -= 1;
      if (nouveaumois < 1)
      {
        nouveaumois = 12;
      }
    }
    else if (etapeconfig == 2)
    {
      nouveaujour -= 1;
      if (nouveaujour < 1)
      {
        nouveaujour = getNombreJourMois(nouveaumois, nouvelleannee);
      }
    }
    else if (etapeconfig == 3)
    {
      nouveaudow -= 1;
      if (nouveaudow < 0)
      {
        nouveaudow = 6;
      }
    }
  }
}

void GestionConfigDate::HandleState()
{
  char outstr[5];
  outstr[4] = 0;
  if (etapeconfig == 0)
  {
    outstr[0] = 'y';
    outstr[1] = 'y';
    outstr[2] = '0' + ((nouvelleannee % 100)/10);
    outstr[3] = '0' + (nouvelleannee % 10);
    display.Affiche(outstr);
  }
  else if (etapeconfig == 1)
  {
    outstr[0] = 'H';
    outstr[1] = 'H';
    outstr[2] = '0' + (nouveaumois /10);
    outstr[3] = '0' + (nouveaumois % 10);
    display.Affiche(outstr);
  }
  else if (etapeconfig == 2)
  {
    outstr[0] = 'd';
    outstr[1] = 'd';
    outstr[2] = '0' + (nouveaujour /10);
    outstr[3] = '0' + (nouveaujour % 10);
    display.Affiche(outstr);
  }
  else
  {
    outstr[0] = 'J';
    outstr[1] = 'o';
    outstr[2] = ' ';
    outstr[3] = '0' + (nouveaudow % 10);
    display.Affiche(outstr);
  }
}


void GestionConfigOption::EnterState()
{
  etapeoption = 0;
  newlum = config.getBrightness();
  newsnooze = config.getSnoozeDelay();
  newa1song = config.getAlarm1Song();
  newa2song = config.getAlarm2Song();
}

void GestionConfigOption::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
  {
    EtatAfficheHeure.EnterState();
    EtatCourant = &EtatAfficheHeure;
  }
  else if ( boutons.justPressed(BTN_OK) )
  {
    etapeoption += 1;
    if ( etapeoption > 3 )
    {
      // set les configs avec les nouvelles valeures
     config.setBrightness(newlum);
     config.setSnoozeDelay(newsnooze);
     config.setAlarm1Song(newa1song);
     config.setAlarm2Song(newa2song);
     config.saveconfig();
      
      EtatAfficheHeure.EnterState();
      EtatCourant = &EtatAfficheHeure;
    }
  }
  else if ( boutons.justPressed(BTN_PLUS) )
  {
    if ( etapeoption == 0 )
    {
      newlum += 1;
      if (newlum > 8)
      {
        newlum = 1;
      }
    }
    else if (etapeoption == 1)
    {
      newsnooze += 1;
      if (newsnooze > 15)
      {
        newsnooze = 4;
      }
    }
    else if (etapeoption == 2)
    {
      newa1song += 1;
      if (newa1song > 4)
      {
        newa1song = 0;
      }
    }
    else if (etapeoption == 3)
    {
      newa2song += 1;
      if (newa2song > 4)
      {
        newa2song = 0;
      }
    }
  }
  else if ( boutons.justPressed(BTN_MOINS) )
  {
    if ( etapeoption == 0 )
    {
      newlum -= 1;
      if (newlum < 1)
      {
        newlum = 8;
      }
    }
    else if (etapeoption == 1)
    {
      newsnooze -= 1;
      if (newsnooze < 4)
      {
        newsnooze = 15;
      }
    }
    else if (etapeoption == 2)
    {
      if (newa1song == 0)
      {
        newa1song = 4;
       }
      else
      {
        newa1song -= 1;
      }
    }
    else if (etapeoption == 3)
    {
      if (newa2song == 0)
      {
        newa2song = 4;
       }
      else
      {
        newa2song -= 1;
      }
    }
  }
}

void GestionConfigOption::HandleState()
{
  char outstr[5];
  outstr[4] = 0;
  switch (etapeoption)
  {
    case 0:
      outstr[0] = 'L';
      outstr[1] = 'u';
      outstr[2] = ' ';
      outstr[3] = '0' + (newlum % 10);
      display.Affiche(outstr);
      break;
    case 1:
      outstr[0] = 'S';
      outstr[1] = 'n';
      outstr[2] = '0'+ (newsnooze / 10);
      outstr[3] = '0' + (newsnooze % 10);
      display.Affiche(outstr);
      break;
    case 2:
      outstr[0] = 'A';
      outstr[1] = '1';
      outstr[2] = ' ';
      outstr[3] = '0' + (newa1song % 10);
      display.Affiche(outstr);
      break;
    case 3:
      outstr[0] = 'A';
      outstr[1] = '2';
      outstr[2] = ' ';
      outstr[3] = '0' + (newa2song % 10);
      display.Affiche(outstr);
      break;  
  }
}


void GestionJukebox::EnterState()
{
  cursong = 0;
  speaker.setSong(cursong);
  //speaker.Start();
}

void GestionJukebox::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
  {
    EtatAfficheHeure.EnterState();
    EtatCourant = &EtatAfficheHeure;
  }
  else if ( boutons.justPressed(BTN_OK) )
  {
    // Play / stop
    if (speaker.isPlaying())
    {
      speaker.Stop();
    }
    else
    {
      speaker.Start();
    }
  }
  else if ( boutons.justPressed(BTN_PLUS) )
  {
    cursong += 1;
    if (cursong > 4)
    {
      cursong = 0;
    }
    speaker.setSong(cursong);
  }
  else if ( boutons.justPressed(BTN_MOINS) )
  {
    if (cursong == 0)
    {
      cursong = 4;
    }
    else
    {
      cursong -= 1;
    }
    speaker.setSong(cursong);
  }
}

void GestionJukebox::HandleState()
{
  // Affiche le numero de la chanson
  // Animation si play
  char outstr[5];
  outstr[4] = 0;
  outstr[0] = 'S';
  outstr[1] = 'n';
  outstr[2] = '0'+ (cursong / 10);
  outstr[3] = '0' + (cursong % 10);
  display.Affiche(outstr);
}


class ConsoleManager
{
public:
  void Setup();
  void Update();
  void printCommandes();
  
protected:
  // liste de commande supporte
  // set heure, date, alarmes, config
  // change state
  CommandString cmd;

  void processCmd();
};

void ConsoleManager::Setup()
{
  cmd.clear();
}

void ConsoleManager::Update()
{
  // regarde s'il y a du data dans Serial
  // attend d'avoir une fin de ligne
  if (Serial.available() > 0) {
    // read the incoming byte:
    char inChar = Serial.read();
    // say what you got:
    if ( inChar == '\n' )
    {
      // fin de ligne, execute la commande
      processCmd();
      // clear buffer
      cmd.clear();
    }
    else
    {
      cmd.addChar(inChar);
    }
  }

  // si oui, execute la commande
  
  // si non, rien a faire

}


void ConsoleManager::printCommandes()
{
  char buffer[32];

  strcpy_P(buffer, settime_cmdstr);
  Serial.println(buffer);
  strcpy_P(buffer, setdate_cmdstr);
  Serial.println(buffer);
  strcpy_P(buffer, gettimedate_cmdstr);
  Serial.println(buffer);
  strcpy_P(buffer, getalarm1_cmdstr);
  Serial.println(buffer);
  strcpy_P(buffer, setalarm1_cmdstr);
  Serial.println(buffer);
  strcpy_P(buffer, setalarm1on_cmdstr);
  Serial.println(buffer);
  strcpy_P(buffer, clearalarm1_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, getalarm2_cmdstr);
  Serial.println(buffer);
  strcpy_P(buffer, setalarm2_cmdstr);
  Serial.println(buffer);


  strcpy_P(buffer, ledon_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, temp_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, beep_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, play_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, stop_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, song_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, clearconf_cmdstr);
  Serial.println(buffer);

  strcpy_P(buffer, showconf_cmdstr);
  Serial.println(buffer);
}

void ConsoleManager::processCmd()
{
  cmd.debugPrint();
  if (cmd.cmpCmd_P(settime_cmdstr)==0)
  {
    // set date
    if ( cmd.nbrArg() == 3 )
    {
      int hour = cmd.getArgInt(0);
      int minute = cmd.getArgInt(1);
      int seconde = cmd.getArgInt(2);
      myRTC.setClockMode(false);  // set to 24h
      myRTC.setHour(hour);
      myRTC.setMinute(minute);
      myRTC.setSecond(seconde);
    }
    else
    {
      Serial.println(F("pas assez arg pour settime"));
    }
  }
  else if (cmd.cmpCmd_P(setdate_cmdstr)==0)
  {
    // set date
    if ( cmd.nbrArg() == 4 )
    {
      int year = cmd.getArgInt(0);
      int month = cmd.getArgInt(1);
      int day = cmd.getArgInt(2);
      int dow = cmd.getArgInt(3);
      myRTC.setYear(year);
      myRTC.setMonth(month);
      myRTC.setDate(day);
      myRTC.setDoW(dow);
    }
    else
    {
      Serial.println(F("pas assez arg pour setdate"));
    }
  }
  else if (cmd.cmpCmd_P(gettimedate_cmdstr)==0)
  {
    // get datetime
    bool h12Flag;
    bool pmFlag;
    bool century = false;
    int year = myRTC.getYear();
    int month = myRTC.getMonth(century);
    int date = myRTC.getDate();
    int dow = myRTC.getDoW();
    int hour = myRTC.getHour(h12Flag, pmFlag);
    int minute = myRTC.getMinute();
    int second = myRTC.getSecond();
    Serial.print("Date: ");
    Serial.print(year);
    Serial.print("-");
    Serial.print(month);
    Serial.print("-");
    Serial.print(date);
    Serial.print(" ");
    Serial.print(dow);
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.println();
  }
  else if (cmd.cmpCmd_P(getalarm1_cmdstr)==0)
  {
    bool pmFlag;
    byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
    bool alarmDy, alarmH12Flag, alarmPmFlag;
    myRTC.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
    Serial.print("Alarm 1: ");
    if (myRTC.checkAlarmEnabled(1)) {
      Serial.print(" on ");
    }
    Serial.print(alarmHour);
    Serial.print(":");
    Serial.print(alarmMinute);
    Serial.print(":");
    Serial.print(alarmSecond);
    if (alarmH12Flag)
    {
      if(alarmPmFlag)
        Serial.print(" PM ");
      else
        Serial.print(" AM ");
    }
    if (myRTC.checkIfAlarm(1)) {
      Serial.print(" A1!");
    }
    Serial.println();
  }
  else if (cmd.cmpCmd_P(setalarm1_cmdstr)==0)
  {
    // set alarm 1
    Serial.println(F("commande setalarm1"));
    int hour = cmd.getArgInt(0);
    int minute = cmd.getArgInt(1);
    myRTC.setA1Time(1, hour, minute, 0, 0b00001000, true, false, false);
  }
  else if (cmd.cmpCmd_P(setalarm1on_cmdstr)==0)
  {
    // set alarm 1
    Serial.println(F("commande setalarm1on"));
    int onoff = cmd.getArgInt(0);
    if (onoff == 0)
    {
      myRTC.turnOffAlarm(1);
    }
    else
    {
      myRTC.turnOnAlarm(1);
    }
  }
  else if (cmd.cmpCmd_P(clearalarm1_cmdstr)==0)
  {
    // clear alarm 1
    Serial.println(F("commande clearalarm1"));
    EtatAfficheHeure.clearAlarme(0);
  }
  else if (cmd.cmpCmd_P(getalarm2_cmdstr)==0)
  {
    bool pmFlag;
    byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
    bool alarmDy, alarmH12Flag, alarmPmFlag;
    myRTC.getA2Time(alarmDay, alarmHour, alarmMinute, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
    Serial.print("Alarm 2: ");
    if (myRTC.checkAlarmEnabled(2)) {
      Serial.print(" on ");
    }
    Serial.print(alarmHour);
    Serial.print(":");
    Serial.print(alarmMinute);
    Serial.print(":");
    Serial.print(alarmSecond);
    if (alarmH12Flag)
    {
      if(alarmPmFlag)
        Serial.print(" PM ");
      else
        Serial.print(" AM ");
    }
    if (myRTC.checkIfAlarm(1)) {
      Serial.print(" A2!");
    }
    Serial.println();
  }
  else if (cmd.cmpCmd_P(setalarm2_cmdstr)==0)
  {
    // set alarm 2
    Serial.println(F("commande setalarm2"));
  }
  else if (cmd.cmpCmd_P(ledon_cmdstr)==0)
  {
    // config
    leds.SetPM( true, 1);
    leds.SetAlarm1( true, 10);
    leds.SetAlarm2( true, 128);
  }
  else if (cmd.cmpCmd_P(temp_cmdstr)==0)
  {
    // state
    int temp = myRTC.getTemperature();
    display.Affiche(temp);
  }
  else if (cmd.cmpCmd_P(beep_cmdstr)==0)
  {
    speaker.Beep(1000, 500);
  }
  else if (cmd.cmpCmd_P(play_cmdstr)==0)
  {
    speaker.Start();
  }
  else if (cmd.cmpCmd_P(stop_cmdstr)==0)
  {
    speaker.Stop();
  }
  else if (cmd.cmpCmd_P(song_cmdstr)==0)
  {
    int song = cmd.getArgInt(0);
    speaker.setSong(song);
  }
  else if (cmd.cmpCmd_P(clearconf_cmdstr)==0)
  {
    config.clearconfig();
    Serial.println(F("Configuration remise a neuf"));
  }
  else if (cmd.cmpCmd_P(showconf_cmdstr)==0)
  {
    config.debugPrint();
  }
  else if (cmd.cmpCmd("help")==0)
  {
    printCommandes();
  }
  else
  {
    // unknown command
    Serial.print(F("commande inconnue: "));
    
  }
}
  
ConsoleManager console;

void setup() {
  boutons.setup();

  config.loadconfig();
  
  Serial.begin(57600);
  console.Setup();
  
  // pinmode pour les LED
  leds.setup(config.getBrightness());
  leds.SetPM( false, 0);
  leds.SetAlarm1( false, 0);
  leds.SetAlarm2( false, 0);

  // inititalisation speaker
  speaker.setup();
  
  display.setup(config.getBrightness());
  
  // Initialisation module RTC
  Wire.begin(); 

  // Etat initiale
  EtatCourant = &EtatAfficheHeure;

  // affiche la version
  Serial.println(F("Reveil matin maison chose"));
  Serial.print(F("Version: "));
  Serial.print(MAJOR_VERSION);
  Serial.print('.');
  Serial.print(MINOR_VERSION);
  Serial.print('.');
  Serial.println(REVISON_VERISON);
  Serial.println(F("Initialisation completee"));
}


void loop() {

  lastUpdateTime = curUpdateTime;
  curUpdateTime = millis();

  // Mise a jour bouton
  boutons.Update();

  // handle selon l'etat
  EtatCourant->HandleButtons();
  EtatCourant->HandleState();

  // mise a jour du son
  speaker.update();

  // Affiche selon l'etat
  leds.update();

  // 7 segment update
  display.update();

  // gestion des commande via serial
  console.Update();

  // delai selon peut-etre millis ?
  delay(10);
}
