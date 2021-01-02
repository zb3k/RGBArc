#ifndef COREAPP_h
#define COREAPP_h

#include <Cursor.h>
#include <BaseApplication.h>

extern Cursor cursor;
extern BaseApplication *apps[];

class CoreApp
{
public:
    uint8_t appIndex = 0;

    void selectApp(uint8_t appIndex)
    {
        cursor.reset();

        lcd.clearScene();
        lcd.clear();

        this->appIndex = appIndex;

        apps[this->appIndex]->start();
    }

    void render()
    {
        // Exit app
        if (ctrl.btnStart.pressedFor(EXIT_PRESS_WAIT))
        {
            this->selectApp(0);
        }

        cursor.processing();

        apps[this->appIndex]->processing();

        lcd.renderScene();

        cursor.render();

        apps[this->appIndex]->postProcessing();

        lcd.show();
    }
};

#endif