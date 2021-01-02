#ifndef _HEADER_h
#define _HEADER_h

#include <Arduino.h>

#define TOTAL_APPS 4

/*
 ########  #### ##    ##  ######
 ##     ##  ##  ###   ## ##    ##
 ##     ##  ##  ####  ## ##
 ########   ##  ## ## ##  ######
 ##         ##  ##  ####       ##
 ##         ##  ##   ### ##    ##
 ##        #### ##    ##  ######
*/
#define LCD_PIN 6

#define BUZZER_PIN 3

#define BTN_START 5
#define BTN_UP 7
#define BTN_DOWN 9
#define BTN_LEFT 10
#define BTN_RIGHT 8
#define BTN_A 12
#define BTN_B 11

/*
 ##        ######  ########
 ##       ##    ## ##     ##
 ##       ##       ##     ##
 ##       ##       ##     ##
 ##       ##       ##     ##
 ##       ##    ## ##     ##
 ########  ######  ########
*/

// Количество пикселей в матрице по ширине
#define LCD_WIDTH 8

// Количество пикселей в матрице по высоте
#define LCD_HEIGHT 8

// Максимальная яркость (от 0 до 255)
#define LCD_MAX_BRIGHTNESS 15

//
#define LCD_NUMPIXELS (LCD_WIDTH * LCD_HEIGHT)

/*
  ######   #######  ##        #######  ########   ######
 ##    ## ##     ## ##       ##     ## ##     ## ##    ##
 ##       ##     ## ##       ##     ## ##     ## ##
 ##       ##     ## ##       ##     ## ########   ######
 ##       ##     ## ##       ##     ## ##   ##         ##
 ##    ## ##     ## ##       ##     ## ##    ##  ##    ##
  ######   #######  ########  #######  ##     ##  ######
*/
#define COLOR_BLACK 0
#define COLOR_WHITE 1
#define COLOR_RED 2
#define COLOR_ORANGE 3
#define COLOR_YELLOW 4
#define COLOR_GREEN 5
#define COLOR_CYAN 6
#define COLOR_BLUE 7
#define COLOR_PURPLE 8
#define COLOR_PINK 9

#define PALETTE_COLOR_FIRST COLOR_RED
#define PALETTE_COLOR_LAST COLOR_PINK
#define PALETTE_SIZE PALETTE_COLOR_LAST + 1

/*
  ######   #######  ##    ## ######## ########   #######  ##        ######
 ##    ## ##     ## ###   ##    ##    ##     ## ##     ## ##       ##    ##
 ##       ##     ## ####  ##    ##    ##     ## ##     ## ##       ##
 ##       ##     ## ## ## ##    ##    ########  ##     ## ##        ######
 ##       ##     ## ##  ####    ##    ##   ##   ##     ## ##             ##
 ##    ## ##     ## ##   ###    ##    ##    ##  ##     ## ##       ##    ##
  ######   #######  ##    ##    ##    ##     ##  #######  ########  ######
*/

#define LONG_PRESS_WAIT 150
#define LONG_PRESS_INTERVAL 50
// #define EXIT_PRESS_WAIT 1000

/*
  ######  ##     ## ########   ######   #######  ########
 ##    ## ##     ## ##     ## ##    ## ##     ## ##     ##
 ##       ##     ## ##     ## ##       ##     ## ##     ##
 ##       ##     ## ########   ######  ##     ## ########
 ##       ##     ## ##   ##         ## ##     ## ##   ##
 ##    ## ##     ## ##    ##  ##    ## ##     ## ##    ##
  ######   #######  ##     ##  ######   #######  ##     ##
*/
// #define CURSOR_BLINK_FADE_MAX 20
// #define CURSOR_BLINK_FADE_MIN 10
// #define CURSOR_BLINK_STEP 1
#define CURSOR_MODE_DEFAULT 0
#define CURSOR_MODE_PARTICLE 1
#define CURSOR_TYPE_DISABLED 0
#define CURSOR_TYPE_PIXEL 1
#define CURSOR_TYPE_ICON 2
#define CURSOR_ICON_SIZE 3

/*
  ######  ######## ########  ##     ##  ######  ########
 ##    ##    ##    ##     ## ##     ## ##    ##    ##
 ##          ##    ##     ## ##     ## ##          ##
  ######     ##    ########  ##     ## ##          ##
       ##    ##    ##   ##   ##     ## ##          ##
 ##    ##    ##    ##    ##  ##     ## ##    ##    ##
  ######     ##    ##     ##  #######   ######     ##
*/
struct AppState
{
  uint8_t appIndex;
};

/*
  ######   #######  ##     ## ##    ## ########
 ##    ## ##     ## ##     ## ###   ## ##     ##
 ##       ##     ## ##     ## ####  ## ##     ##
  ######  ##     ## ##     ## ## ## ## ##     ##
       ## ##     ## ##     ## ##  #### ##     ##
 ##    ## ##     ## ##     ## ##   ### ##     ##
  ######   #######   #######  ##    ## ########
*/

#define SOUND_TICK_DURATION 10
#define SOUND_BEEP_DURATION 30

#define NOTE_C3 1
#define NOTE_CS3 2
#define NOTE_D3 3
#define NOTE_DS3 4
#define NOTE_E3 5
#define NOTE_F3 6
#define NOTE_FS3 7
#define NOTE_G3 8
#define NOTE_GS3 9
#define NOTE_A3 10
#define NOTE_AS3 11
#define NOTE_B3 12

#define NOTE_C4 13
#define NOTE_CS4 14
#define NOTE_D4 15
#define NOTE_DS4 16
#define NOTE_E4 17
#define NOTE_F4 18
#define NOTE_FS4 19
#define NOTE_G4 20
#define NOTE_GS4 21
#define NOTE_A4 22
#define NOTE_AS4 23
#define NOTE_B4 24

#define NOTE_C5 25
#define NOTE_CS5 26
#define NOTE_D5 27
#define NOTE_DS5 28
#define NOTE_E5 29
#define NOTE_F5 30
#define NOTE_FS5 31
#define NOTE_G5 32
#define NOTE_GS5 33
#define NOTE_A5 34
#define NOTE_AS5 35
#define NOTE_B5 36

#define NOTE_C6 37
#define NOTE_CS6 38
#define NOTE_D6 39
#define NOTE_DS6 40
#define NOTE_E6 41
#define NOTE_F6 42
#define NOTE_FS6 43
#define NOTE_G6 44
#define NOTE_GS6 45
#define NOTE_A6 46
#define NOTE_AS6 47
#define NOTE_B6 48

/*
  ######   ##        #######  ########     ###    ##          ######## ##    ##
 ##    ##  ##       ##     ## ##     ##   ## ##   ##          ##       ###   ##
 ##        ##       ##     ## ##     ##  ##   ##  ##          ##       ####  ##
 ##   #### ##       ##     ## ########  ##     ## ##          ######   ## ## ##
 ##    ##  ##       ##     ## ##     ## ######### ##          ##       ##  ####
 ##    ##  ##       ##     ## ##     ## ##     ## ##          ##       ##   ###
  ######   ########  #######  ########  ##     ## ########    ##       ##    ##
*/
void selectApp(uint8_t appIndex);

// END _HEADER_h
#endif