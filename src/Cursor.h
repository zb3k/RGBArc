#ifndef CURSOR_h
#define CURSOR_h

#include "header.h"
#include <Arduino.h>
#include <Sound.h>
#include <Lcd/Lcd.h>
#include <Controls/Controls.h>

extern Lcd lcd;
extern Sound sound;
extern Controls ctrl;

class Cursor
{
public:
    uint8_t mode; // Режим взаимодействия курсора
    uint8_t type : 1;
    uint8_t controlY : 1; // Разрешить управлять курсором по оси Y
    uint8_t controlX : 1;
    uint8_t rollback : 1;  // Возвращает координаты предыдущего тика
    uint8_t borderTop : 1; // Граница вверху
    uint8_t moveSound : 1;

    uint8_t isNextVector : 1; // У курсора изменился nextVector
    uint8_t isTouch : 1;      // Курсор коснулся объекта или границы
    uint8_t isTouchDown : 1;  // Курсор коснулся нижней границы
    uint8_t isOutsideLcd : 1; // Курсор выходит за пределы экрана
    // uint8_t angle; // Угол

    // Текущие координаты курсора
    int8_t x;
    int8_t y;
    uint16_t vector;

    // Следующие координаты курсора
    int8_t nextX;
    int8_t nextY;
    uint16_t nextVector;

    uint8_t color; // Цвет (номер)
    // uint32_t colorRGB; // Цвет RGB
    uint8_t icon[3]; // Иконка курсора

    // Размеры курсора
    uint8_t width;
    uint8_t height;

    uint8_t blinkBrightness = 255;
    uint8_t blinkDirection = 2;

    void setup()
    {
        this->reset();
    }

    void reset()
    {
        this->type = CURSOR_TYPE_DISABLED;
        this->mode = CURSOR_MODE_DEFAULT;
        this->controlX = true;
        this->controlY = true;
        this->borderTop = true;
        this->moveSound = true;

        this->width = 1;
        this->height = 1;

        this->color = COLOR_WHITE;
    }

    void processing()
    {
        if (!this->type)
        {
            return;
        }

        // Отмена передвижения
        if (this->rollback)
        {
            this->rollback = false;
            this->nextX = this->x;
            this->nextY = this->y;
            this->nextVector = this->vector;
        }
        // Перемещение
        else
        {
            // Звук если есть движение
            if (this->moveSound && this->isNextVector)
            {
                sound.tick();
            }

            this->isTouch = false;
            this->vector = this->nextVector;
            this->x = this->nextX;
            this->y = this->nextY;
        }

        // Управление по X
        if (this->controlX)
        {
            if (ctrl.btnRight.wasPressed() || (ctrl.btnRight.pressedFor(LONG_PRESS_WAIT) && ctrl.isLongPressTick()))
            {
                this->nextX = this->x + 1;
            }
            if (ctrl.btnLeft.wasPressed() || (ctrl.btnLeft.pressedFor(LONG_PRESS_WAIT) && ctrl.isLongPressTick()))
            {
                this->nextX = this->x - 1;
            }
        }

        // Управление по Y
        if (this->controlY)
        {
            if (ctrl.btnDown.wasPressed() || (ctrl.btnDown.pressedFor(LONG_PRESS_WAIT) && ctrl.isLongPressTick()))
            {
                this->nextY = this->y + 1;
            }
            if (ctrl.btnUp.wasPressed() || (ctrl.btnUp.pressedFor(LONG_PRESS_WAIT) && ctrl.isLongPressTick()))
            {
                this->nextY = this->y - 1;
            }
        }

        // Следующий вектор
        this->nextVector = LCD_VECTOR(this->nextX, this->nextY);
        this->isNextVector = this->nextVector != this->vector;

        if (this->isNextVector && isCursorOverlapping())
        {
            this->rollback = true;
            this->isTouch = true;
            this->isTouchDown = this->height + this->nextY > LCD_HEIGHT;
        }

        this->isOutsideLcd = this->vector > LCD_NUMPIXELS;
    }

    void render()
    {
        if (!this->type)
        {
            return;
        }

        if (this->blinkDirection)
        {
            if (this->blinkDirection == 1)
            {
                this->blinkBrightness += 2;
                if (this->blinkBrightness >= 255)
                {
                    this->blinkDirection = 2;
                }
            }
            else
            {
                this->blinkBrightness -= 2;
                if (this->blinkBrightness <= 100)
                {
                    this->blinkDirection = 1;
                }
            }
        }

        // uint8_t vector = 0xFF;

        // lcd._neoPixel.setPixelColor(this->vector, lcd._neoPixel.ColorHSV(0, 255, this->blinkBrightness));
        lcd.setPixel(this->vector, this->color, this->blinkBrightness);
        // lcd.setPixel(this->vector, COLOR_RED, this->blinkBrightness);
    }

    /**
     * Проверяет не перекрывает ли точка другие точки на экране или не выходит за пределы экрана
     * @param  {int8_t} x        :
     * @param  {int8_t} y        :
     * @param  {uint16_t} vector :
     * @return {bool}            :
     */
    bool isPixelOverlapping(int8_t x, int8_t y, uint16_t vector)
    {
        return (this->mode == CURSOR_MODE_PARTICLE && vector <= LCD_NUMPIXELS && vector >= 0 && lcd.scene[vector]) ||
               (/* this->borderRight && */ x >= LCD_WIDTH) ||
               (/* this->borderLeft && */ x < 0) ||
               (/* this->borderBottom && */ y >= LCD_HEIGHT) ||
               (this->borderTop && y < 0);
    }

    /**
     * Проверяет не перекрывает ли следующая позиция курсора другие точки или не выходит за пределы экрана
     * @return {bool}  :
     */
    bool isCursorOverlapping()
    {
        this->isTouchDown = false;

        // проверка для точки
        if (this->type == CURSOR_TYPE_PIXEL)
        {
            this->isTouchDown = this->nextX >= 0 && this->nextX < LCD_WIDTH;
            return isPixelOverlapping(this->nextX, this->nextY, this->nextVector);
        }
        else
        {
            for (uint8_t x = 0; x < CURSOR_ICON_SIZE; x++)
            {
                for (uint8_t y = 0; y < CURSOR_ICON_SIZE; y++)
                {
                    if (bitRead(this->icon[y], 2 - x))
                    {
                        int8_t nextPixelPosition = this->nextVector + x + y * LCD_WIDTH;
                        int8_t nextX = this->nextX + x;
                        int8_t nextY = this->nextY + y;
                        if (isPixelOverlapping(nextX, nextY, nextPixelPosition))
                        {
                            this->isTouchDown = nextX >= 0 && nextX < LCD_WIDTH;
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }
};

#endif