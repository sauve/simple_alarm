#include <Arduino.h>

// LED PIN
#define LED_PM_PIN 5
#define LED_ALARM_PIN 6
#define LED_ALARM2_PIN 10

class LEDSequencer
{
  public:
  void setup(byte luminosite);
  void update();

  void SetPM(bool on, int freq);
  void SetAlarm1(bool on, int freq);
  void SetAlarm2(bool on, int freq);

  void ChangeLuminosite(int luminosite);
  void setLuminosite(byte luminosite);

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
