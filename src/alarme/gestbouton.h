#include "Arduino.h"

// Defines pour les pins
#define BTN_CONF_PIN  A0
#define BTN_PLUS_PIN  A1
#define BTN_MOINS_PIN  A2
#define BTN_OK_PIN  A3



// button bit constant
#define BTN_CONF    0b00000001
#define BTN_PLUS    0b00000010
#define BTN_MOINS   0b00000100
#define BTN_OK    0b00001000


class GestionBoutons
{
  public:
    void setup();
    void Update();
    bool justPressed( uint8_t button);
    bool stillPressed( uint8_t button);
    bool isPressed( uint8_t button);
  protected:
    int btnpressed = 0;
    int btnlastpressed = 0;
    int btnchanged = 0;
};
