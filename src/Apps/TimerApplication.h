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

        if (this->isSecondTick())
        {
            if (this->time == 60)
            {
                this->time = 0;
                // decimals++;
            }
            // uint8_t tens;
            // uint8_t ones;

            // tens = time / 10;
            // ones = time % 10;

            lcd.setScenePixel(this->time, COLOR_RED);
            // lcdChar(1, 2, tens, COLOR_PINK);
            // lcdChar(4, 2, ones, COLOR_DEEP_BLUE);

            // for (uint8_t i = 0; i < LCD_WIDTH; i++)
            // {
            //     lcd.setScenePixel(i, 4, decimals > i ? COLOR_PINK : COLOR_BLUE);
            // }

            this->time++;
        }
    };

    bool isSecondTick()
    {
        unsigned long currentMillis = millis();
        uint16_t DIFF1000 = currentMillis - this->secondTick; //DIFF=1001 -0 =1001
        if (DIFF1000 >= 1000)
        {
            this->secondTick = currentMillis - (DIFF1000 - 1000);
            return true;
            //1001+1, make next time we get in here w/ a little fast
        }
        return false;

        // // uint32_t static timer = 0;
        // uint32_t now = millis();

        // if (this->secondTick == 0)
        // {
        //     this->secondTick = now;
        // }

        // if (now >= this->secondTick)
        // {
        //     Serial.print(now);
        //     Serial.print("   ");
        //     Serial.println(this->secondTick);
        //     this->secondTick += 1000;
        //     return true;
        // }

        // return false;
    };
};

#endif
