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
const char showtime_cmdstr[] PROGMEM = {"showtime"};
const char settime_cmdstr[] PROGMEM = {"settime"};
const char setdate_cmdstr[] PROGMEM = {"setdate"};
const char gettimedate_cmdstr[] PROGMEM = {"gettimedate"};
const char getalarm1_cmdstr[] PROGMEM = {"getalarm1"};
const char setalarm1_cmdstr[] PROGMEM = {"setalarm1"};
const char setalarm1on_cmdstr[] PROGMEM = {"setalarm1on"};
const char clearalarm1_cmdstr[] PROGMEM = {"clearalarm1"};
const char fade_cmdstr[] PROGMEM = {"fade"};
const char getalarm2_cmdstr[] PROGMEM = {"getalarm2"};
const char setalarm2_cmdstr[] PROGMEM = {"setalarm2"};
const char ledon_cmdstr[] PROGMEM = {"ledon"};
const char ledoff_cmdstr[] PROGMEM = {"ledoff"};
const char showtext_cmdstr[] PROGMEM = {"showtext"};
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


// Information de version
#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define REVISON_VERISON 0
//const char device_str[] PROGMEM = {"Reveil matin maison chose"}; 
//const char version_str[] PROGMEM = {"Version: "}; 

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

void clearAlarme( byte alarme )
{
  // arrete la musique
  if (alarme == 0)
  {
    speaker.Stop();
    // clear  
    myRTC.checkIfAlarm(1);
  }
  else if (alarme == 1)
  {
    speaker.Stop();
    // clear  
    myRTC.checkIfAlarm(2);
  }
}



class GestionAfficheHeure : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();

  protected:
    int presedconf = 0;
    unsigned long lastUpdate = 0; 
};

class GestionTestEtat : public GestionEtat
{
  public:
    void EnterState();
    void HandleButtons();
    void HandleState();
  protected:
    int valeuraffiche = 8888;
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

GestionAfficheHeure EtatAfficheHeure;
GestionTestEtat EtatTestEtat;
GestionConfigHeure EtatConfigHeure;
GestionConfigAlarme EtatConfigAlarme;

void GestionAfficheHeure::EnterState()
{
  Serial.println(F("Etat Affiche Heure"));
}

void GestionAfficheHeure::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
  {
    presedconf = 1;
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
      if (myRTC.checkIfAlarm(1, false))
      {
        clearAlarme(0);
      }
      else 
      {
        display.ChangeLuminosite(1);
      }
    }
    if (boutons.justPressed(BTN_MOINS) )
    {
      if (myRTC.checkIfAlarm(2, false))
      {
        clearAlarme(1);
      }
      else 
      {
        display.ChangeLuminosite(-1);
      }
    }
  }

  if( presedconf == 200 )
  {
    // si OK, Plus ou moins sont aussi appuyer
    presedconf = 0;  
    if (  boutons.isPressed( BTN_OK) )
    {
      // Change l'etat
      Serial.println(F("Config Date"));
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
    // regarde si les alarme ont ete activer

    // Si oui, change l'eta et appel update

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

    // set les les selon l'alarme
    if (myRTC.checkAlarmEnabled(1))
    {
      // Si snooze, update la led freq de l'alarme
      if (myRTC.checkIfAlarm(1, false) )
      {
          leds.SetAlarm1(true, 24);
          if ( speaker.isPlaying() == false )
          {
            speaker.Start();
          }
      }
      else
      {
        leds.SetAlarm1(true, 0);
      }
    }
    else
    {
      leds.SetAlarm1(false, 0);
    }
    
    if (myRTC.checkAlarmEnabled(2))
    {
      if (myRTC.checkIfAlarm(2, false) )
      {
          leds.SetAlarm2(true, 24);
      }
      else
      {
        leds.SetAlarm2(true, 0);
      }
    }
    else
    {
      leds.SetAlarm2(false, 0);
    }
    
  }
}

void GestionTestEtat::EnterState()
{
  Serial.println(F("Test etat"));
  valeuraffiche = 8888;
}

void GestionTestEtat::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
  {
    EtatCourant = &EtatAfficheHeure;
  }
  else if ( boutons.justPressed(BTN_PLUS) )
  {
    valeuraffiche += 1;
  }
  else if ( boutons.justPressed(BTN_MOINS) )
  {
    valeuraffiche -= 1;
  }
}

void GestionTestEtat::HandleState()
{
  display.Affiche(valeuraffiche);
}


void GestionConfigHeure::EnterState()
{
  Serial.println(F("Etat config heure"));
  heureouminute  = 0;
  bool h12Flag;
  bool pmFlag;
  nouvelleheure = myRTC.getHour(h12Flag, pmFlag);
  nouvelleminte = myRTC.getMinute();
  display.DeuxPointsOn(false);
}

void GestionConfigHeure::HandleButtons()
{
  if ( boutons.justPressed(BTN_CONF) )
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
  if (heureouminute == 0)
  {
    display.Affiche(nouvelleheure);
  }
  else
  {
    display.Affiche(nouvelleminte);
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
  display.DeuxPointsOn(false);
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
  if (heureouminute == 0)
  {
    display.Affiche(nouvelleheure);
  }
  else if (heureouminute == 1)
  {
    display.Affiche(nouvelleminte);
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

  strcpy_P(buffer, showtime_cmdstr);
  Serial.println(buffer);
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
}

void ConsoleManager::processCmd()
{
  cmd.debugPrint();
  if (cmd.cmpCmd_P(showtime_cmdstr)==0)
  {
    bool h12Flag;
    bool pmFlag;
    int hour = myRTC.getHour(h12Flag, pmFlag);
    int minute = myRTC.getMinute();
    display.AfficheHeure(hour, minute);
  }
  else if (cmd.cmpCmd_P(settime_cmdstr)==0)
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
    clearAlarme(0);
  }
  else if (cmd.cmpCmd_P(fade_cmdstr)==0)
  {
    // set alarm 1
    for (int i = 8; i >= 0; i -= 1)
    {
      display.ChangeLuminosite(i);
      display.Affiche("fade");
      delay(100);
      
    }
    display.ChangeLuminosite(5);
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
  else if (cmd.cmpCmd_P(ledoff_cmdstr)==0)
  {
    // state
    leds.SetPM( false, 0);
    leds.SetAlarm1( false, 0);
    leds.SetAlarm2( false, 0);
  }
  else if (cmd.cmpCmd_P(showtext_cmdstr)==0)
  {
    // state
    display.Affiche_P(lundi_str); 
    delay(1000);
   display.Affiche_P(mardi_str); 
    delay(1000);
    display.Affiche_P(mercredi_str); 
    delay(1000);
    display.Affiche_P(jeudi_str); 
    delay(1000);
    display.Affiche_P(vendredi_str); 
    delay(1000);
    display.Affiche_P(samedi_str); 
    delay(1000);
    display.Affiche_P(dimanche_str); 
    delay(1000);
    display.Affiche_P(janvier_str); 
    delay(1000);
    display.Affiche_P(fevrier_str); 
    delay(1000);
    display.Affiche_P(mars_str); 
    delay(1000);
    display.Affiche_P(avril_str); 
    delay(1000);
    display.Affiche_P(mai_str); 
    delay(1000);
    display.Affiche_P(juin_str); 
    delay(1000);
    display.Affiche_P(juillet_str); 
    delay(1000);
    display.Affiche_P(aout_str); 
    delay(1000);
    display.Affiche_P(septembre_str)     ; 
    delay(1000);
    display.Affiche_P(octobre_str); 
    delay(1000);
    display.Affiche_P(novembre_str); 
    delay(1000);
    display.Affiche_P(decembre_str); 
    delay(1000); 
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
    Serial.println(F("Configuration courante"));
    Serial.print(F("luminosite: "));
    Serial.println(config.getBrightness());
    Serial.print(F("snooze: "));
    Serial.println(config.getSnoozeDelay());
    Serial.print(F("Alarme 1: "));
    Serial.println(config.getAlarm1Song());
    Serial.print(F("Alarme 2: "));
    Serial.println(config.getAlarm2Song());
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
  leds.setup();
  leds.SetPM( false, 1);
  leds.SetAlarm1( false, 10);
  leds.SetAlarm2( false, 128);

  // inititalisation speaker
  speaker.setup();
  
  display.setup();
  
  // Initialisation module RTC
  Wire.begin();
  int temp = myRTC.getTemperature();
  display.Affiche(temp); 

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
  Serial.println(F("Initialisation complete"));
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
