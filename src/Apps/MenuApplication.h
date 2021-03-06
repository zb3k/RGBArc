#include <BaseApplication.h>

extern BaseApplication *apps[];

class MenuApplication : public BaseApplication
{
public:
    void start(){};

    void processing()
    {
        // Next app
        if (ctrl.btnRight.wasPressed() || ctrl.btnDown.wasPressed())
        {
            this->appIndex = this->appIndex >= TOTAL_APPS - 1 ? 1 : this->appIndex + 1;
            sound.beep();
        }

        // Prev app
        if (ctrl.btnLeft.wasPressed() || ctrl.btnUp.wasPressed())
        {
            this->appIndex = this->appIndex == 1 ? TOTAL_APPS - 1 : this->appIndex - 1;
            sound.beep();
        }

        lcd.clearScene();
        lcd.loadImageToScene(apps[this->appIndex]->icon());

        // Select app
        if (ctrl.btnStart.wasPressed())
        {
            selectApp(appIndex);
        }
    };

private:
    uint8_t appIndex = 1;
};