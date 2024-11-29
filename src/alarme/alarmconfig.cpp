#include "alarmconfig.h"

void AlarmConfig::loadconfig()
{
    brightess = EEPROM.read(EEPROMADDR_BRIGHTNESS);
    snoozedelay = EEPROM.read(EEPROMADDR_SNOOZE);
    alarm1song = EEPROM.read(EEPROMADDR_ALARM1SONG);
    alarm2song = EEPROM.read(EEPROMADDR_ALARM2SONG);
}

void AlarmConfig::saveconfig()
{
  // put ou update?
}

void AlarmConfig::clearconfig()
{
  // ecrit en eeprom les valeur par defaut
  EEPROM.write(EEPROMADDR_BRIGHTNESS, 5);
  EEPROM.write(EEPROMADDR_SNOOZE, 9);
  EEPROM.write(EEPROMADDR_ALARM1SONG, 0);
  EEPROM.write(EEPROMADDR_ALARM2SONG, 1);
}

void AlarmConfig::degbugPrint()
{
    Serial.print(F("Brightness: "));
    Serial.println(brightess);
    Serial.print(F("SnoozeDelay: "));
    Serial.println(snoozedelay);
}

byte AlarmConfig::getBrightness()
{
    return brightess;
}

void AlarmConfig::setBrightness(byte brightess)
{
    this->brightess = brightess;
}

byte AlarmConfig::getSnoozeDelay()
{
    return snoozedelay;
}

void AlarmConfig::setSnoozeDelay(byte delay)
{
    snoozedelay = delay;
}

byte AlarmConfig::getAlarm1Song()
{
  return alarm1song;
}

void AlarmConfig::setAlarm1Song(byte song)
{
  alarm1song = song;
}

byte AlarmConfig::getAlarm2Song()
{
  return alarm2song;
}

void AlarmConfig::setAlarm2Song(byte song)
{
  alarm2song = song;
}
