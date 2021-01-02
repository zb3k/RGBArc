#include "header.h"

#include <Arduino.h>
#include <Lcd/Lcd.h>
#include <Sound.h>
#include <Controls/Controls.h>
#include <Cursor.h>
#include <CoreApp.h>
#include <BaseApplication.h>
#include <Apps/MenuApplication.h>
#include <Apps/TestControlsApplication.h>
#include <Apps/DrawApplication.h>
#include <Apps/TimerApplication.h>

/*
  ######   #######  ##    ## ######## ####  ######
 ##    ## ##     ## ###   ## ##        ##  ##    ##
 ##       ##     ## ####  ## ##        ##  ##
 ##       ##     ## ## ## ## ######    ##  ##   ####
 ##       ##     ## ##  #### ##        ##  ##    ##
 ##    ## ##     ## ##   ### ##        ##  ##    ##
  ######   #######  ##    ## ##       ####  ######
*/

TestControlsApplication appTestControls;
MenuApplication appMenu;
DrawApplication appDraw;
TimerApplication appTimer;

BaseApplication *apps[TOTAL_APPS] = {
    &appMenu,
    &appTimer,
    &appDraw,
    &appTestControls};

/*
 ##     ##    ###    ########   ######
 ##     ##   ## ##   ##     ## ##    ##
 ##     ##  ##   ##  ##     ## ##
 ##     ## ##     ## ########   ######
  ##   ##  ######### ##   ##         ##
   ## ##   ##     ## ##    ##  ##    ##
    ###    ##     ## ##     ##  ######
*/
Lcd lcd;
Controls ctrl;
Cursor cursor;
CoreApp app;
Sound sound;

/*
  ######  ######## ######## ##     ## ########
 ##    ## ##          ##    ##     ## ##     ##
 ##       ##          ##    ##     ## ##     ##
  ######  ######      ##    ##     ## ########
       ## ##          ##    ##     ## ##
 ##    ## ##          ##    ##     ## ##
  ######  ########    ##     #######  ##
*/
void setup()
{
  Serial.begin(9600);

  ctrl.setup();

  lcd.setup();
  lcd.setBrightness(LCD_MAX_BRIGHTNESS);
  lcd.clear();

  sound.setup();

  cursor.setup();
}

/*
 ##        #######   #######  ########
 ##       ##     ## ##     ## ##     ##
 ##       ##     ## ##     ## ##     ##
 ##       ##     ## ##     ## ########
 ##       ##     ## ##     ## ##
 ##       ##     ## ##     ## ##
 ########  #######   #######  ##
*/
void loop()
{
  ctrl.read();
  app.render();
  sound.processing();
}
