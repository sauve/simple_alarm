#include "Arduino.h"

// SPEAKER PIN
#define SPEAKER_PIN 9

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
