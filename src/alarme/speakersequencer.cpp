#include "speakersequencer.h"


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
