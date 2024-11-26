#include "Arduino.h"

// SPEAKER PIN
#define SPEAKER_PIN 9

class SpeakerSequencer
{
  public:
    void setup();
    void update();

    void Beep( int tone, int duration);
    void Start();
    void Stop();
    void setSong(int song);
    bool isPlaying()
    {
      return playing;
    }
  protected:
    bool playing = false;
    int curnoteidx = 0;
    int totalnotes = 0;
    int wholenote = 0;
    int *songptr;
    int tempo;
    unsigned long nextnote;
};
