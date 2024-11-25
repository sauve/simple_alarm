// Simple Alarme

#include <TM1637.h>
#include <DS3231.h>
#include <Wire.h>
#include <inttypes.h>
#include "commandstring.h"
#include "ledsequencer.h"

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// Defines pour les pins
#define BTN_CONF_PIN  A0
#define BTN_PLUS_PIN  A1
#define BTN_MOINS_PIN  A2
#define BTN_OK_PIN  A3


// SPEAKER PIN
#define SPEAKER_PIN 9


// button bit constant
#define BTN_CONF    0b00000001
#define BTN_PLUS    0b00000010
#define BTN_MOINS   0b00000100
#define BTN_OK    0b00001000


/// command string
const char showtime_cmdstr[] PROGMEM = {"showtime"};
const char settime_cmdstr[] PROGMEM = {"settime"};
const char setdate_cmdstr[] PROGMEM = {"setdate"};
const char gettimedate_cmdstr[] PROGMEM = {"gettimedate"};
const char getalarm1_cmdstr[] PROGMEM = {"getalarm1"};
const char setalarm1_cmdstr[] PROGMEM = {"setalarm1"};
const char fade_cmdstr[] PROGMEM = {"fade"};
const char getalarm2_cmdstr[] PROGMEM = {"getalarm2"};
const char setalarm2_cmdstr[] PROGMEM = {"setalarm2"};
const char ledon_cmdstr[] PROGMEM = {"ledon"};
const char ledoff_cmdstr[] PROGMEM = {"ledoff"};
const char showtext_cmdstr[] PROGMEM = {"showtext"};
const char temp_cmdstr[] PROGMEM = {"temp"};
const char beep_cmdstr[] PROGMEM = {"beep"};



class GestionEtat
{
  public:
    virtual void HandleButtons() = 0;
    virtual void HandleState() = 0;
};


unsigned long lastUpdateTime = 0;
unsigned long curUpdateTime = 0;
TM1637 tm(CLK, DIO);
DS3231 myRTC;
GestionEtat* EtatCourant = nullptr;


// Variables pouir la gestion des boutons
uint8_t btnlastpressed;
uint8_t btnpressed;
uint8_t btnchanged;

void updateButtons()
{
  // Update 
  btnlastpressed = btnpressed;
  // btnpressed
  btnpressed = 0;
  if ( digitalRead(BTN_CONF_PIN) == 0 )
  {
    btnpressed += BTN_CONF;
  }
  if ( digitalRead(BTN_PLUS_PIN) == 0 )
  {
    btnpressed += BTN_PLUS;
  }
  if ( digitalRead(BTN_MOINS_PIN) == 0 )
  {
    btnpressed += BTN_MOINS;
  }
  if ( digitalRead(BTN_OK_PIN) == 0 )
  {
    btnpressed += BTN_OK;
  }
  // update les inputs
  btnchanged = btnpressed ^ btnlastpressed;
}

bool justPressed( uint8_t button)
{
  return ((btnpressed & btnchanged) & button ) != 0;
}

bool stillPressed( uint8_t button)
{
  return ((btnpressed & ~btnchanged) & button ) != 0;
}

bool isPressed( uint8_t button)
{
  return (btnpressed & button) != 0;
}

class DisplaySeuqencer
{
  public:
    void setup();
    void update();

    
    void ChangeLuminosite();
    void FlashDeuxPoints();
    void FlashChiffreDroit();
    void FlashChiffreGauche();


    void AfficheHeure();
    
  protected:
    int lastFlash = 0;
    bool flashDroit = false;
    bool flashGauche = false;
    bool flashCentre = false;
    
    // valeur affichage
    // devrait avoir le display ici, tout passe par le sequenceur
    // Flash pourrait etre plus un fade ou hard flash
};

void DisplaySeuqencer::setup()
{
  // va lire la luminosite en EEPROM
  // initialise le display et la comm
  // clear display par defaut
}

void DisplaySeuqencer::update()
{
  // valid flash
  // si mise a jour
  // update display
}


void DisplaySeuqencer::ChangeLuminosite()
{
  // update selon difference en parametre
  // set en EEPROM
  // set update
}


void DisplaySeuqencer::FlashDeuxPoints()
{}


void DisplaySeuqencer::FlashChiffreDroit()
{}

void DisplaySeuqencer::FlashChiffreGauche()
{}


void DisplaySeuqencer::AfficheHeure()
{
  // selon 12/24 ou non
  // set les valeur d'affichage
  // set le flag pour mise a jour
}


DisplaySeuqencer display;

LEDSequencer leds;


class SpeakerSequencer
{
  public:
    void setup();
    void update();

    void Beep( int tone, int duration);
  protected:
    bool hascommand;
    int curtone;
    int stopTime;
};


void SpeakerSequencer::setup()
{
  pinMode(SPEAKER_PIN, OUTPUT);
}

void SpeakerSequencer::update()
{
  // regarde le temps ecouler et set le tone selon la commande
  // avance a la prochaine commande
  if (hascommand)
  {
    hascommand = false;
    tone(SPEAKER_PIN, curtone, 500);
  }
}

void SpeakerSequencer::Beep(int tone, int duration)
{
  // set le tone et quand l'arreter
  hascommand = true;
  curtone = tone;
  stopTime = millis() + duration;
}


SpeakerSequencer speaker;

class GestionAfficheHeure : public GestionEtat
{
  public:
    void HandleButtons();
    void HandleState();

  protected:
    int presedconf = 0;
    unsigned long lastUpdate = 0; 
};

void GestionAfficheHeure::HandleButtons()
{
  if ( justPressed(BTN_CONF) )
  {
    presedconf = 1;
  }
  else if ( stillPressed(BTN_CONF) )
  {
    presedconf++;
  }

  if( presedconf > 10 )
  {
    // si OK, Plus ou moins sont aussi appuyer
    if (  isPressed( BTN_OK) )
    {
      // Change l'etat
      Serial.println(F("Config Date"));
    }
    else if ( isPressed( BTN_PLUS) )
    {
      // Change l'etat
      Serial.println(F("Config Alarme 1"));
    }
    else if ( isPressed( BTN_MOINS) )
    {
      // Change l'etat
      Serial.println(F("Config Alarme 2"));
    }
    else
    {
      Serial.println(F("Config Heure"));
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
    int temp = hour * 100 + minute;
    if ( (seconde % 2) == 0)
    {
      tm.colonOff();
    } 
    else
    {
      tm.colonOn();
    }
    tm.display(temp); 

    // set la led pm
    if (pmFlag)
    {
      leds.SetPM(true, 0);
    }

    // set les les selon l'alarme
    if (myRTC.checkAlarmEnabled(1))
    {
      // Si snooze, update la led freq de l'alarme
      leds.SetAlarm1(true, 0);
    }
    
    if (myRTC.checkAlarmEnabled(2))
    {
      leds.SetAlarm2(true, 0);
    }
    
  }
}

GestionAfficheHeure EtatAfficheHeure;

class ConsoleManager
{
public:
  void Setup();
  void Update();
  
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

void ConsoleManager::processCmd()
{
  cmd.debugPrint();
  if (cmd.cmpCmd_P(showtime_cmdstr)==0)
  {
    bool h12Flag;
    bool pmFlag;
    int hour = myRTC.getHour(h12Flag, pmFlag);
    int minute = myRTC.getMinute();
    tm.display(hour * 100 + minute); 
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
  }
  else if (cmd.cmpCmd_P(fade_cmdstr)==0)
  {
    // set alarm 1
    for (int i = 90; i >= 0; i -= 10)
    {
      tm.setBrightnessPercent(i);
      tm.display("fade");
      delay(100);
      
    }
    tm.setBrightnessPercent(90);
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
    tm.display("lundi"); 
    delay(1000);
   tm.display("Hardi"); 
    delay(1000);
    tm.display("Hercredi"); 
    delay(1000);
    tm.display("jeudi"); 
    delay(1000);
    tm.display("uendredi"); 
    delay(1000);
    tm.display("saHedi"); 
    delay(1000);
    tm.display("diHanche"); 
    delay(1000);
    tm.display("januier"); 
    delay(1000);
    tm.display("feurier"); 
    delay(1000);
    tm.display("Hars"); 
    delay(1000);
    tm.display("auril"); 
    delay(1000);
    tm.display("Hai"); 
    delay(1000);
    tm.display("juin"); 
    delay(1000);
    tm.display("juillet"); 
    delay(1000);
    tm.display("aout"); 
    delay(1000);
    tm.display("septeHbre")     ; 
    delay(1000);
    tm.display("octobre"); 
    delay(1000);
    tm.display("nouehbre"); 
    delay(1000);
    tm.display("deceHbre"); 
    delay(1000); 
  }
  else if (cmd.cmpCmd_P(temp_cmdstr)==0)
  {
    // state
    int temp = myRTC.getTemperature();
    tm.display(temp);
  }
  else if (cmd.cmpCmd_P(beep_cmdstr)==0)
  {
    speaker.Beep(1000, 500);
  }
  else
  {
    // unknown command
    Serial.print(F("commande inconnue: "));
  }
}
  
ConsoleManager console;

void setup() {
  // pinmode pour les boutons
  pinMode(BTN_CONF_PIN, INPUT_PULLUP);
  pinMode(BTN_PLUS_PIN, INPUT_PULLUP);
  pinMode(BTN_MOINS_PIN, INPUT_PULLUP);
  pinMode(BTN_OK_PIN, INPUT_PULLUP);

  console.Setup();
  Serial.begin(9600);

  // pinmode pour les LED
  leds.setup();
  leds.SetPM( false, 1);
  leds.SetAlarm1( false, 10);
  leds.SetAlarm2( false, 128);

  // inititalisation speaker
  speaker.setup();

  // initialisation Afficheur 7 segment
  tm.begin();
  tm.colonOn();
  for (int i=0; i<9; i++)
  {
    tm.setBrightness(i);
    tm.display(i);
    delay(500);
  }
  tm.setBrightness(5);
  
  display.setup();
  
  // Initialisation module RTC
  Wire.begin();
  int temp = myRTC.getTemperature();
  tm.display(temp); 

  // Etat initiale
  EtatCourant = &EtatAfficheHeure;

  Serial.println(F("Initialisation complete"));
}


void loop() {

  lastUpdateTime = curUpdateTime;
  curUpdateTime = millis();

  // Mise a jour bouton
  updateButtons();

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
