#include <Controls/Controls.h>

// Инициализация
void Controls::setup()
{
    this->btnStart.begin();
    this->btnUp.begin();
    this->btnDown.begin();
    this->btnLeft.begin();
    this->btnRight.begin();
    this->btnA.begin();
    this->btnB.begin();
}

void Controls::read()
{
    this->btnStart.read();
    this->btnUp.read();
    this->btnDown.read();
    this->btnLeft.read();
    this->btnRight.read();
    this->btnA.read();
    this->btnB.read();
}

/**
 * Возвращает true если кнопка нажата каждые LONG_PRESS_INTERVAL
 * @return {bool}  :
 */
bool Controls::isLongPressTick()
{
    static uint32_t longPressTimer = 0;

    if (longPressTimer)
    {
        if (millis() >= longPressTimer)
        {
            longPressTimer = 0;
        }
    }
    else
    {
        longPressTimer = millis() + LONG_PRESS_INTERVAL;
    }

    return longPressTimer ? false : true;
}
