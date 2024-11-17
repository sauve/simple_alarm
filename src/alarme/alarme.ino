// Simple Alarme

#include <TM1637.h>
#include <DS3231.h>
#include <Wire.h>
#include <inttypes.h>

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


class LEDSequencer
{
  public:
  void setup();
  void update();

  protected:
    int maxBrightness;
    int pmFreq;
    int Alarm1Freq;
    int Alarm2Freq;
    bool pmOn;
    bool Alarm1On;
    bool Alarm2On;
};

void LEDSequencer::setup()
{
  pinMode(LED_PM_PIN, OUTPUT);
  pinMode(LED_ALARM_PIN, OUTPUT);
  pinMode(LED_ALARM2_PIN, OUTPUT);
}

void LEDSequencer::update()
{
  // Si pmOn
  // selon Freq
  // Si 0 = On selon max brightness
  // Sinon, selon Freq et le conteur
}

LEDSequencer leds;


class SpeakerSequencer
{
  public:
    void setup();
    void update();

    void Beep();
  protected:

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

void SpeakerSequencer::Beep()
{
  // set le tone et quand l'arreter
}


SpeakerSequencer speaker;

class GestionAfficheHeure : public GestionEtat
{
  public:
    void HandleButtons();
    void HandleState();
};

void GestionAfficheHeure::HandleButtons()
{
}

void GestionAfficheHeure::HandleState()
{
}

GestionAfficheHeure EtatAfficheHeure;

void setup() {
  // pinmode pour les boutons

  // pinmode pour les LED
  leds.setup();

  // inititalisation speaker
  speaker.setup();

  // initialisation Afficheur 7 segment

  // Initialisation module RTC

  // Etat initiale
  EtatCourant = &EtatAfficheHeure;

}


void loop() {
  // Mise a jour bouton
  updateButtons();

  // handle selon l'etat
  EtatCourant->HandleButtons();
  EtatCourant->HandleState();

  speaker.update();

  // Affiche selon l'etat
  leds.update();

  // 7 segment update

}
