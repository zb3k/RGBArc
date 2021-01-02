#include <CoreApp.h>
#include <BaseApplication.h>

extern CoreApp app;

class MenuApplication : public BaseApplication
{
public:
    void start(){};

    void processing()
    {
        // Next app icon
        if (ctrl.btnRight.wasPressed() || ctrl.btnDown.wasPressed())
        {
            this->appIndex = this->appIndex >= TOTAL_APPS - 1 ? 1 : this->appIndex + 1;
            sound.beep();
        }

        // Prev app icon
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
            app.selectApp(appIndex);
        }
    };

private:
    uint8_t appIndex = 1;
};