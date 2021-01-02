#ifndef CONTROLS_h
#define CONTROLS_h

#include "header.h"
#include <JC_Button.h>

class Controls
{
public:
    Controls()
        : btnStart(BTN_START), btnUp(BTN_UP), btnDown(BTN_DOWN), btnLeft(BTN_LEFT), btnRight(BTN_RIGHT), btnA(BTN_A), btnB(BTN_B) {}

    Button btnStart;
    Button btnUp;
    Button btnDown;
    Button btnLeft;
    Button btnRight;
    Button btnA;
    Button btnB;

    // Инициализация
    void setup();

    void read();

    /**
     * Возвращает true если кнопка нажата каждые LONG_PRESS_INTERVAL
     * @return {bool}  :
     */
    bool isLongPressTick();

    // private:
};

#endif