#include "ledsequencer.h"

void LEDSequencer::setup(byte luminosite)
{
  pinMode(LED_PM_PIN, OUTPUT);
  pinMode(LED_ALARM_PIN, OUTPUT);
  pinMode(LED_ALARM2_PIN, OUTPUT);

  maxBrightness = (255 / 8) * luminosite;
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


void LEDSequencer::ChangeLuminosite(int luminosite)
{
  maxBrightness += (255 / 8) * luminosite;
  if ( maxBrightness < (255 / 8) )
  {
    maxBrightness = 255 / 8;
  }
  else if ( maxBrightness > 255)
  {
    maxBrightness = 255;
  }
  
}

void LEDSequencer::setLuminosite(byte luminosite)
{
  maxBrightness = (255 / 8) * luminosite;
  if ( maxBrightness < (255 / 8) )
  {
    maxBrightness = 255 / 8;
  }
  else if ( maxBrightness > 255)
  {
    maxBrightness = 255;
  }
}
