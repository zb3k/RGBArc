#ifndef TimerApplication_h
#define TimerApplication_h

#include <BaseApplication.h>

class TimerApplication : public BaseApplication
{
public:
    uint8_t time = 0;
    uint32_t secondTick = 0;

    uint8_t *icon()
    {
        static const PROGMEM uint8_t icon[] = {
            2,
            7, B00000000, B00001110, B00001010, B00001010, B00001010, B00001010, B00001110, B00000000,
            9, B00000000, B01110000, B01010000, B01010000, B01010000, B01010000, B01110000, B00000000};
        return (uint8_t *)icon;
    };

    // Инициализация
    void start()
    {
        this->time = 0;
        this->secondTick = 0;
    };

    void processing()
    {
        // static uint8_t decimals = 0;

        // app.lcdMode = LCD_FADE_MODE;

        if (ctrl.btnA.wasPressed())
        {
            this->time = 0;
            // decimals = 0;
        }

        if (this->isPeriodComplete(PERIOD_1_S))
        {
            if (this->time == 60)
            {
                this->time = 0;
                Serial.println("Test");
                // decimals++;
            }
            // uint8_t tens;
            // uint8_t ones;

            // tens = time / 10;
            // ones = time % 10;
            sound.tick();
            lcd.setScenePixel(this->time, COLOR_RED);
            // lcdChar(1, 2, tens, COLOR_PINK);
            // lcdChar(4, 2, ones, COLOR_DEEP_BLUE);

            // for (uint8_t i = 0; i < LCD_WIDTH; i++)
            // {
            //     lcd.setScenePixel(i, 4, decimals > i ? COLOR_PINK : COLOR_BLUE);
            // }

            Serial.print('.');

            this->time++;
        }
    };

private:
    bool isPeriodComplete(uint16_t periodTime)
    {
        uint32_t now = millis();

        if (this->secondTick == 0)
        {
            this->secondTick = now;
        }

        if (now >= this->secondTick)
        {
            this->secondTick += periodTime;
            return true;
        }

        return false;
    };
};

#endif
