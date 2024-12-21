#include <Arduino.h>
#include <EEPROM.h>

#define EEPROMADDR_BRIGHTNESS 0
#define EEPROMADDR_SNOOZE 1
#define EEPROMADDR_ALARM1SONG 2
#define EEPROMADDR_ALARM2SONG 3
#define EEPROMADDR_VERSION 4

class AlarmConfig
{
    public:
        void loadconfig();
        void saveconfig();
        void clearconfig();

        void debugPrint();

        byte getBrightness();
        void setBrightness(byte brightess);

        byte getSnoozeDelay();
        void setSnoozeDelay(byte snoozedelay);

        byte getAlarm1Song();
        void setAlarm1Song(byte song);

        byte getAlarm2Song();
        void setAlarm2Song(byte song);

        byte getVersionConfig();
    
    protected:
        byte versionconfig;
        byte brightess;
        byte snoozedelay;
        byte alarm1song;
        byte alarm2song;
        // autre info a propos des alarmes
};
