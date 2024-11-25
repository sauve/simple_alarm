#include <Arduino.h>

class AlarmConfig
{
    public:
        void loadconfig();
        void saveconfig();

        void degbugPrint();

        int getBrightness();
        void setBrightness(int brightess);

        int getSnoozeDelay();
        void setSnoozeDelay(int snoozedelay);
    
    protected:
        int brightess;
        int snoozedelay;
        // autre info a propos des alarmes
};
