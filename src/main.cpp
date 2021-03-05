#include "header.h"

#include <Arduino.h>
#include <Lcd/Lcd.h>
#include <Sound.h>
#include <Controls/Controls.h>
#include <Cursor.h>
#include <BaseApplication.h>
#include <Apps/MenuApplication.h>
#include <Apps/TestControlsApplication.h>
#include <Apps/DrawApplication.h>
#include <Apps/TimerApplication.h>

// -----------------------------------------------------------------------------
// Applications
// -----------------------------------------------------------------------------

TestControlsApplication appTestControls;
MenuApplication appMenu;
DrawApplication appDraw;
TimerApplication appTimer;

BaseApplication *apps[TOTAL_APPS] = {
    &appMenu,
    &appTimer,
    &appDraw,
    &appTestControls};

uint8_t currentApp = 0;

// -----------------------------------------------------------------------------
// Core libraries
// -----------------------------------------------------------------------------

Lcd lcd;
Controls ctrl;
Cursor cursor;
Sound sound;

// -----------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);

  ctrl.setup();

  lcd.setup();
  lcd.setBrightness(LCD_MAX_BRIGHTNESS);
  lcd.clear();

  sound.setup();

  cursor.setup();
}

// -----------------------------------------------------------------------------
// Loop
// -----------------------------------------------------------------------------

void loop()
{
  ctrl.read();

  // FIXME: Slow
  if (ctrl.btnStart.pressedFor(EXIT_PRESS_WAIT))
  {
    selectApp(0); // Open main menu
  }

  cursor.processing();

  apps[currentApp]->processing();

  lcd.renderScene();

  cursor.render();

  // apps[currentApp]->postProcessing();

  lcd.sync();

  sound.processing();

  // Serial.write('.');
}

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

void selectApp(uint8_t appIndex)
{
  cursor.reset();
  lcd.clearScene();
  lcd.clear();

  currentApp = appIndex;

  apps[currentApp]->start();
}