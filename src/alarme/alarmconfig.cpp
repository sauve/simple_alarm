#include "alarmconfig.h"

void AlarmConfig::loadconfig()
{
    brightess = EEPROM.read(EEPROMADDR_BRIGHTNESS);
    brightess = constrain(brightess, 1, 8);
    snoozedelay = EEPROM.read(EEPROMADDR_SNOOZE);
    snoozedelay = constrain(snoozedelay, 4, 15);
    alarm1song = EEPROM.read(EEPROMADDR_ALARM1SONG);
    alarm1song = constrain(alarm1song, 0, 4);
    alarm2song = EEPROM.read(EEPROMADDR_ALARM2SONG);
    alarm2song = constrain(alarm2song, 0, 4);
    versionconfig = EEPROM.read(EEPROMADDR_VERSION);
}

void AlarmConfig::saveconfig()
{
  EEPROM.write(EEPROMADDR_BRIGHTNESS, brightess);
  EEPROM.write(EEPROMADDR_SNOOZE, snoozedelay);
  EEPROM.write(EEPROMADDR_ALARM1SONG, alarm1song);
  EEPROM.write(EEPROMADDR_ALARM2SONG, alarm2song);
  // put ou update?
}

void AlarmConfig::clearconfig()
{
  // ecrit en eeprom les valeur par defaut, force ecriture
  EEPROM.write(EEPROMADDR_BRIGHTNESS, 5);
  EEPROM.write(EEPROMADDR_SNOOZE, 9);
  EEPROM.write(EEPROMADDR_ALARM1SONG, 0);
  EEPROM.write(EEPROMADDR_ALARM2SONG, 1);
  EEPROM.write(EEPROMADDR_VERSION, 1);
}

void AlarmConfig::debugPrint()
{
    Serial.print(F("Version: "));
    Serial.println(versionconfig);
    Serial.print(F("Brightness: "));
    Serial.println(brightess);
    Serial.print(F("SnoozeDelay: "));
    Serial.println(snoozedelay);
    Serial.print(F("Alarme 1 song: "));
    Serial.println(alarm1song);
    Serial.print(F("Alarme 2 song: "));
    Serial.println(alarm2song);
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

byte AlarmConfig::getVersionConfig()
{
  return versionconfig;
}
