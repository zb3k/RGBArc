#ifndef TestControlsApplication_h
#define TestControlsApplication_h

#include <BaseApplication.h>

class TestControlsApplication : public BaseApplication
{
public:
    uint8_t *icon()
    {
        static const PROGMEM uint8_t icon[] = {
            2,
            7, B00000000, B01111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
            9, B00000000, B00000000, B01111110, B01011110, B01101110, B01011110, B01111110, B00000000};
        return (uint8_t *)icon;
    };

    void start() {}

    void processing()
    {
        uint8_t note = 0;

        lcd.setScenePixel(1 + 1, 1 + 1, ctrl.btnStart.isPressed() ? COLOR_WHITE : COLOR_RED);    // START
        lcd.setScenePixel(1 + 1, 0 + 1, ctrl.btnUp.isPressed() ? COLOR_WHITE : COLOR_PURPLE);    // UP
        lcd.setScenePixel(1 + 1, 2 + 1, ctrl.btnDown.isPressed() ? COLOR_WHITE : COLOR_PURPLE);  // DOWN
        lcd.setScenePixel(0 + 1, 1 + 1, ctrl.btnLeft.isPressed() ? COLOR_WHITE : COLOR_PURPLE);  // LEFT
        lcd.setScenePixel(2 + 1, 1 + 1, ctrl.btnRight.isPressed() ? COLOR_WHITE : COLOR_PURPLE); // RIGHT
        lcd.setScenePixel(2 + 3, 2 + 1, ctrl.btnA.isPressed() ? COLOR_WHITE : COLOR_CYAN);       // A
        lcd.setScenePixel(3 + 3, 0 + 1, ctrl.btnB.isPressed() ? COLOR_WHITE : COLOR_CYAN);       // B

        if (ctrl.btnStart.isPressed())
            note = NOTE_C3;
        else if (ctrl.btnUp.isPressed())
            note = NOTE_D3;
        else if (ctrl.btnDown.isPressed())
            note = NOTE_E3;
        else if (ctrl.btnLeft.isPressed())
            note = NOTE_F3;
        else if (ctrl.btnRight.isPressed())
            note = NOTE_G3;
        else if (ctrl.btnA.isPressed())
            note = NOTE_A3;
        else if (ctrl.btnB.isPressed())
            note = NOTE_B3;

        sound.tone(note, 50);
    };
};

#endif
