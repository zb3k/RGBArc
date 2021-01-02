#ifndef DrawApplication_h
#define DrawApplication_h

#include <BaseApplication.h>

class DrawApplication : public BaseApplication
{
public:
    uint8_t *icon()
    {
        static const PROGMEM uint8_t icon[] = {
            2,
            7, B00000000, B01000010, B00000000, B00011000, B00011000, B00000000, B01000010, B00000000,
            9, B00000000, B00011000, B00100100, B01000010, B01000010, B00100100, B00011000, B00000000};
        return (uint8_t *)icon;
    };

    // Инициализация
    void start()
    {
        cursor.type = CURSOR_TYPE_PIXEL;
        cursor.color = PALETTE_COLOR_FIRST;
    };

    void processing()
    {
        static uint8_t rememberColor;

        if (ctrl.btnStart.isPressed())
        {
            lcd.clearScene();
        }
        else
        {
            if (ctrl.btnA.isPressed())
            {
                lcd.setScenePixel(cursor.vector, cursor.color);
            }

            if (ctrl.btnB.pressedFor(200))
            {
                lcd.setScenePixel(cursor.vector, COLOR_BLACK);
                if (cursor.color != COLOR_WHITE)
                {
                    sound.beep();
                    rememberColor = cursor.color;
                }
                cursor.color = COLOR_WHITE;
            }
            else if (ctrl.btnB.wasReleased())
            {
                if (rememberColor > COLOR_WHITE)
                {
                    cursor.color = rememberColor - 1;
                }
                cursor.color = cursor.color >= PALETTE_COLOR_LAST ? PALETTE_COLOR_FIRST : cursor.color + 1;
                rememberColor = 0;
            }
        }
    };
};

#endif