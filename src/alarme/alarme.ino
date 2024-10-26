// Simple Alarme

#include <TM1637Display.h>
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


// button bit constant
#define BTN_CONF    0b00000001
#define BTN_PLUS    0b00000010
#define BTN_MOINS   0b00000100
#define BTN_OK    0b00001000


TM1637Display display(CLK, DIO);
DS3231 myRTC;


// Variables pouir la gestion des boutons
uint8_t btnlastpressed;
uint8_t btnpressed;
uint8_t btnchanged;

void setup() {
  // pinmode pour les boutons

  // pinmode pour les LED

  // inititalisation speaker

  // initialisation Afficheur 7 segment

  // Initialisation module RTC

}


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

void loop() {
  // Mise a jour bouton

  // handle selon l'etat

  // Affiche selon l'etat

}
