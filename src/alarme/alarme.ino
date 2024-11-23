// Simple Alarme

#include <TM1637.h>
#include <DS3231.h>
#include <Wire.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// Defines pour les pins
#define BTN_CONF_PIN  A0
#define BTN_PLUS_PIN  A1
#define BTN_MOINS_PIN  A2
#define BTN_OK_PIN  A3

// LED PIN
#define LED_PM_PIN 5
#define LED_ALARM_PIN 6
#define LED_ALARM2_PIN 10

// SPEAKER PIN
#define SPEAKER_PIN 9


// button bit constant
#define BTN_CONF    0b00000001
#define BTN_PLUS    0b00000010
#define BTN_MOINS   0b00000100
#define BTN_OK    0b00001000

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


class LEDSequencer
{
  public:
  void setup();
  void update();

  void SetPM(bool on, int freq);
  void SetAlarm1(bool on, int freq);
  void SetAlarm2(bool on, int freq);

  protected:
    int maxBrightness;
    int pmFreq;
    int Alarm1Freq;
    int Alarm2Freq;
    bool pmOn;
    bool Alarm1On;
    bool Alarm2On;
    int frequencyCounter;
};

void LEDSequencer::setup()
{
  pinMode(LED_PM_PIN, OUTPUT);
  pinMode(LED_ALARM_PIN, OUTPUT);
  pinMode(LED_ALARM2_PIN, OUTPUT);

  maxBrightness = 255;
  pmFreq = 0;
  pmOn = false;
  Alarm1Freq = 0;
  Alarm1On = false;
  Alarm2Freq = 0;
  Alarm2On = false;
  frequencyCounter = 0;
}

void LEDSequencer::update()
{
  frequencyCounter++;
  if ( frequencyCounter > 255 ) frequencyCounter = 0;
  int count = 0;
  // Si pmOn
  if ( pmOn )
  {
    // Si 0, on selon max brightness
    if ( pmFreq == 0 )
    {
      analogWrite(LED_PM_PIN, maxBrightness);
    }
    else
    {
      // selon freq et le conteur
      count = frequencyCounter * pmFreq;
      count &= 0x00ff;
      analogWrite(LED_PM_PIN, map( count, 0, 255, 0, maxBrightness));
    }
  }
  else
  {
    analogWrite(LED_PM_PIN, 0);
  }

  // Si Alarm1On
  if ( Alarm1On )
  {
    // Si 0, on selon max brightness
    if ( Alarm1Freq == 0 )
    {
      analogWrite(LED_ALARM_PIN, maxBrightness);
    }
    else
    {
      // selon freq et le conteur
      count = frequencyCounter * Alarm1Freq;
      count &= 0x00ff;
      analogWrite(LED_ALARM_PIN, map( count, 0, 255, 0, maxBrightness));
    }
  }
  else
  {
    analogWrite(LED_ALARM_PIN, 0);
  }

  // Si Alarm2On
  if ( Alarm2On )
  {
    // Si 0, on selon max brightness
    if ( Alarm2Freq == 0 )
    {
      analogWrite(LED_ALARM2_PIN, maxBrightness);
    }
    else
    {
      // selon freq et le conteur
      count = frequencyCounter * Alarm2Freq;
      count &= 0x00ff;
      analogWrite(LED_ALARM2_PIN, map( count, 0, 255, 0, maxBrightness));
    }
  }
  else
  {
    analogWrite(LED_ALARM2_PIN, 0);
  }
}

void LEDSequencer::SetPM(bool on, int freq)
{
  pmOn = on;
  pmFreq = freq;
  if (pmFreq < 0) pmFreq = 0;
  if (pmFreq > 128) pmFreq = 128;
}

void LEDSequencer::SetAlarm1(bool on, int freq)
{
  Alarm1On = on;
  Alarm1Freq = freq;
  if (Alarm1Freq < 0) Alarm1Freq = 0;
  if (Alarm1Freq > 128) Alarm1Freq = 128;
}

void LEDSequencer::SetAlarm2(bool on, int freq)
{
  Alarm2On = on;
  Alarm2Freq = freq;
  if (Alarm2Freq < 0) Alarm2Freq = 0;
  if (Alarm2Freq > 128) Alarm2Freq = 128;
}



LEDSequencer leds;


class SpeakerSequencer
{
  public:
    void setup();
    void update();

    void Beep( int tone, int duration);
  protected:
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
}

void SpeakerSequencer::Beep(int tone, int duration)
{
  // set le tone et quand l'arreter
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
      Serial.println("Config Date");
    }
    else if ( isPressed( BTN_PLUS) )
    {
      // Change l'etat
      Serial.println("Config Alarme 1");
    }
    else if ( isPressed( BTN_MOINS) )
    {
      // Change l'etat
      Serial.println("Config Alarme 2");
    }
    else
    {
      Serial.println("Config Heure");
    }
  }
}

void GestionAfficheHeure::HandleState()
{
  // affiche l'heure
  if ( curUpdateTime - lastUpdate > 1000 )
  {
    lastUpdate = curUpdateTime;
    bool h12Flag;
    bool pmFlag;
    int hour = myRTC.getHour(h12Flag, pmFlag);
    int minute = myRTC.getMinute();
    int temp = hour * 100 + minute;
    tm.display(temp); 
  }
}

GestionAfficheHeure EtatAfficheHeure;

class CommandString
{
public:
  void clear();
  void addChar(char c);

  // assignation a partir d'une chaine

  // retourne le nom de la commande
  // compare si c'est la commande
  int cmpCmd( const char* cmd)
  {
    return strncmp(buffer, cmd, cmdlen);
  }
  // retourne le nombre d'argument
  int nbrArg()
  {
    return nbrarg;
  }

  // compare l'argument a l'index
  int cmpArg( int index, const char* arg)
  {
    if (index >= nbrarg) return 1;
    return strncmp(buffer + argstart[index], arg, arglen[index]);
  }

  // retourne l'argument en tant que int
  int getArgInt( int index)
  {
    if (index >= nbrarg) return 0;
    return atoi(buffer + argstart[index]);
  }

  void debugPrint();

protected:
  char buffer[64];
  int index;
  int cmdlen;
  int nbrarg;
  int argstart[4];
  int arglen[4];
};

void CommandString::clear()
{
  cmdlen = 0;
  nbrarg = 0;
  for (int i = 0; i < 64; i++)
  {
    buffer[i] = 0;
  }
  index = 0;
  for (int i = 0; i < 4; i++)
  {
    argstart[i] = 0;
    arglen[i] = 0;
  }
}

void CommandString::addChar( char c)
{
  // index 63 doit etre un 0 pour la fin de la chaine
  if ( index < 62 )
  {
    buffer[index] = c;
    index++;

    // si c'est une virgule, ajoute un argument, sinon augmente la taille de nom de la commande ou de l'argument courant
    if (c == ',')
    {
      buffer[index-1] = '\0';
      argstart[nbrarg] = index;
      nbrarg++;
    }
    else
    {
      if (nbrarg > 0)
      {
        arglen[nbrarg-1] = index - argstart[nbrarg-1];
      }
      else
      {
        cmdlen = index;
      }
    }
  }
}

void CommandString::debugPrint()
{
  Serial.print("cmd: ");
  for (int i = 0; i < cmdlen; i++)
  {
    Serial.print(buffer[i]);
  }
  Serial.println();
  for (int i = 0; i < nbrarg; i++)
  {
    Serial.print("arg");
    Serial.print(i);
    Serial.print(": ");
    for (int j = 0; j < arglen[i]; j++)
    {
      Serial.print(buffer[argstart[i]+j]);
    }
    Serial.println();
  }
}

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
  if (cmd.cmpCmd("showtime")==0)
  {
    bool h12Flag;
    bool pmFlag;
    int hour = myRTC.getHour(h12Flag, pmFlag);
    int minute = myRTC.getMinute();
    tm.display(hour * 100 + minute); 
  }
  else if (cmd.cmpCmd("settime")==0)
  {
    // set date
    myRTC.setClockMode(false);  // set to 24h
    myRTC.setYear(24);
    myRTC.setMonth(11);
    myRTC.setDate(18);
    myRTC.setDoW(2);
    myRTC.setHour(20);
    myRTC.setMinute(10);
    myRTC.setSecond(0);
  }
  else if (cmd.cmpCmd("gettimedate")==0)
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
  else if (cmd.cmpCmd("setalarm1")==0)
  {
    // set alarm 1
    Serial.println("commande setalarm1");
  }
  else if (cmd.cmpCmd("fade")==0)
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
  else if (cmd.cmpCmd("setalarm2")==0)
  {
    // set alarm 2
    Serial.println("commande setalarm2");
  }
  else if (cmd.cmpCmd("ledon")==0)
  {
    // config
    leds.SetPM( true, 1);
    leds.SetAlarm1( true, 10);
    leds.SetAlarm2( true, 128);
  }
  else if (cmd.cmpCmd("ledoff")==0)
  {
    // state
    leds.SetPM( false, 0);
    leds.SetAlarm1( false, 0);
    leds.SetAlarm2( false, 0);
  }
  else if (cmd.cmpCmd("showtext")==0)
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
    tm.display("septeHbre"); 
    delay(1000);
    tm.display("octobre"); 
    delay(1000);
    tm.display("nouehbre"); 
    delay(1000);
    tm.display("deceHbre"); 
    delay(1000);
  }
  else if (cmd.cmpCmd("temp")==0)
  {
    // state
    int temp = myRTC.getTemperature();
    tm.display(temp);
  }
  else
  {
    // unknown command
    Serial.print("commande inconnue: ");
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
  tm.setBrightnessPercent(90);
  display.setup();
  
  // Initialisation module RTC
  Wire.begin();
  int temp = myRTC.getTemperature();
  tm.display(temp); 

  // Etat initiale
  EtatCourant = &EtatAfficheHeure;

  Serial.println("Initialisation complete");
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
