#include "alarmconfig.h"

void AlarmConfig::loadconfig()
{
    brightess = 5;
    snoozedelay = 9;
}

void AlarmConfig::saveconfig()
{

}

void AlarmConfig::degbugPrint()
{
    Serial.print(F("Brightness: "));
    Serial.println(brightess);
    Serial.print(F("SnoozeDelay: "));
    Serial.println(snoozedelay);
}

int AlarmConfig::getBrightness()
{
    return brightess;
}

void AlarmConfig::setBrightness(int brightess)
{
    this->brightess = brightess;
}

int AlarmConfig::getSnoozeDelay()
{
    return snoozedelay;
}

void AlarmConfig::setSnoozeDelay(int snoozedelay)
{
    this->snoozedelay = snoozedelay;
}
