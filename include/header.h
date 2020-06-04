#include <Arduino.h>

/*
 
    ########  #### ##    ##  ######  
    ##     ##  ##  ###   ## ##    ## 
    ##     ##  ##  ####  ## ##       
    ########   ##  ## ## ##  ######  
    ##         ##  ##  ####       ## 
    ##         ##  ##   ### ##    ## 
    ##        #### ##    ##  ######  
 
*/
#define BUZZER_PIN 3
#define RGB_PIN 6

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
#define LCD_WIDTH 8
#define LCD_HEIGHT 8
#define NUMPIXELS (LCD_WIDTH * LCD_HEIGHT)
#define LCD_MAX_BRIGHTNESS 20
#define BLINK_INTERVAL 200
#define LCD_TEXT_BUFFER_SIZE 10

#define SPLASH_SCREEN_RAINBOW 1
#define SPLASH_SCREEN_FADE_OUT 2
#define SPLASH_SCREEN_FLASH 3

/*
 
     ######  ##     ## ########   ######   #######  ########  
    ##    ## ##     ## ##     ## ##    ## ##     ## ##     ## 
    ##       ##     ## ##     ## ##       ##     ## ##     ## 
    ##       ##     ## ########   ######  ##     ## ########  
    ##       ##     ## ##   ##         ## ##     ## ##   ##   
    ##    ## ##     ## ##    ##  ##    ## ##     ## ##    ##  
     ######   #######  ##     ##  ######   #######  ##     ## 
 
*/
#define CURSOR_BLINK_FADE 100
#define CURSOR_BLINK_STEP 2
#define CURSOR_MODE_DEFAULT 0
#define CURSOR_MODE_PARTICLE 1
#define CURSOR_TYPE_DISABLED 0
#define CURSOR_TYPE_PIXEL 1
#define CURSOR_TYPE_ICON 2
#define MAX_ICON_SIZE 3

/*
 
    ########  ##     ## ######## ########  #######  ##    ##  ######  
    ##     ## ##     ##    ##       ##    ##     ## ###   ## ##    ## 
    ##     ## ##     ##    ##       ##    ##     ## ####  ## ##       
    ########  ##     ##    ##       ##    ##     ## ## ## ##  ######  
    ##     ## ##     ##    ##       ##    ##     ## ##  ####       ## 
    ##     ## ##     ##    ##       ##    ##     ## ##   ### ##    ## 
    ########   #######     ##       ##     #######  ##    ##  ######  
 
*/
#define LONG_PRESS_WAIT 150
#define LONG_PRESS_INTERVAL 50

/*
 
     ######   #######  ##        #######  ########  
    ##    ## ##     ## ##       ##     ## ##     ## 
    ##       ##     ## ##       ##     ## ##     ## 
    ##       ##     ## ##       ##     ## ########  
    ##       ##     ## ##       ##     ## ##   ##   
    ##    ## ##     ## ##       ##     ## ##    ##  
     ######   #######  ########  #######  ##     ## 
 
*/
#define COLOR_BLACK 0
#define COLOR_WHITE 1
#define COLOR_RED 2
#define COLOR_ORANGE 3
#define COLOR_YELLOW 4
#define COLOR_GREEN 5
#define COLOR_LIGHT_BLUE 6
#define COLOR_DEEP_BLUE 7
#define COLOR_PURPLE 8
#define COLOR_PINK 9

#define COLOR_DEFAULT (COLOR_WHITE)

#define PALETTE_COLOR_FIRST (COLOR_RED)
#define PALETTE_COLOR_LAST (COLOR_PINK)
#define PALETTE_COLORS (PALETTE_COLOR_LAST + 1)

const uint16_t paletteHue[PALETTE_COLORS] = {
    0x0000,  // Black
    0x0000,  // White
    0xFFFF,  // Red
    0x1111,  // Orange
    0x2525,  // Yellow
    0x4444,  // Green
    0x7777,  // Light Blue
    0x9999,  // Deep Blue
    0xBBBB,  // Purple
    0xDDDD}; // Pink

/*
 
       ###    ########  ########  
      ## ##   ##     ## ##     ## 
     ##   ##  ##     ## ##     ## 
    ##     ## ########  ########  
    ######### ##        ##        
    ##     ## ##        ##        
    ##     ## ##        ##        
 
*/

#define DEFAULT_APP 0
#define MAX_APPS 6
#define TIMER_FIX 0.655

const uint8_t iconPaint[] = {
    2, // Colors
    7, B00000000, B01000010, B00000000, B00011000, B00011000, B00000000, B01000010, B00000000,
    9, B00000000, B00011000, B00100100, B01000010, B01000010, B00100100, B00011000, B00000000,
    //
};
const uint8_t iconFlow[] = {
    2,
    7, B00000000, B01111110, B00000010, B00111110, B00100000, B00111100, B00000000, B00000000,
    9, B00000000, B00000000, B01111100, B01000000, B01011110, B01000010, B01111110, B00000000,
    //
};
const uint8_t iconTetris[] = {
    2,
    7, B00000000, B01000000, B01100000, B01000000, B00000100, B00000110, B00000010, B00000000,
    9, B00000000, B00001110, B00000010, B00000000, B00000000, B01000000, B01110000, B00000000,
    //
};
const uint8_t iconMaze[] = {
    2,
    7, B00000000, B01111010, B00001010, B01100010, B00111010, B00000000, B01111110, B00000000,
    9, B00000000, B00000000, B01000000, B00000000, B00000000, B00000000, B00000000, B00000000,
    //
};
const uint8_t iconSnake[] = {
    2,
    7, B00000000, B01110000, B01010000, B01010110, B01010100, B00011100, B00000000, B00000000,
    9, B00000000, B00000010, B00000000, B00000000, B00000000, B00000000, B01000000, B00000000,
    //
};
const uint8_t iconTimer[] = {
    2,
    7, B00000000, B00001110, B00001010, B00001010, B00001010, B00001010, B00001110, B00000000,
    9, B00000000, B01110000, B01010000, B01010000, B01010000, B01010000, B01110000, B00000000,
    //
};

/*
 
       ###    ########  ########     ######## ##        #######  ##      ## 
      ## ##   ##     ## ##     ##    ##       ##       ##     ## ##  ##  ## 
     ##   ##  ##     ## ##     ##    ##       ##       ##     ## ##  ##  ## 
    ##     ## ########  ########     ######   ##       ##     ## ##  ##  ## 
    ######### ##        ##           ##       ##       ##     ## ##  ##  ## 
    ##     ## ##        ##           ##       ##       ##     ## ##  ##  ## 
    ##     ## ##        ##           ##       ########  #######   ###  ###  
 
*/
#define APP_FLOW_MAX_LINES 8
#define APP_FLOW_LEVELS 5

const uint8_t flowLevelsSizes[APP_FLOW_LEVELS] = {4, 2, 5, 7, 7};
const uint8_t flowLevels[] = {

    // Level 2
    0 + 0 * LCD_WIDTH, 1 + 2 * LCD_WIDTH,
    6 + 1 * LCD_WIDTH, 7 + 3 * LCD_WIDTH,
    0 + 4 * LCD_WIDTH, 1 + 6 * LCD_WIDTH,
    6 + 5 * LCD_WIDTH, 7 + 7 * LCD_WIDTH,

    // Level 1
    0 + 7 * LCD_WIDTH, 4 + 4 * LCD_WIDTH,
    4 + 3 * LCD_WIDTH, 7 + 7 * LCD_WIDTH,

    // level 1
    7 + 4 * LCD_WIDTH, 7 + 6 * LCD_WIDTH,
    1 + 1 * LCD_WIDTH, 7 + 5 * LCD_WIDTH,
    5 + 3 * LCD_WIDTH, 1 + 5 * LCD_WIDTH,
    0 + 7 * LCD_WIDTH, 3 + 4 * LCD_WIDTH,
    4 + 5 * LCD_WIDTH, 7 + 7 * LCD_WIDTH,

    // level 2
    7 + 0 * LCD_WIDTH, 4 + 7 * LCD_WIDTH,
    1 + 1 * LCD_WIDTH, 3 + 1 * LCD_WIDTH,
    7 + 1 * LCD_WIDTH, 3 + 4 * LCD_WIDTH,
    2 + 2 * LCD_WIDTH, 7 + 4 * LCD_WIDTH,
    2 + 3 * LCD_WIDTH, 7 + 3 * LCD_WIDTH,
    5 + 7 * LCD_WIDTH, 7 + 6 * LCD_WIDTH,
    4 + 4 * LCD_WIDTH, 7 + 2 * LCD_WIDTH,

    // Level 3
    0 + 0 * LCD_WIDTH, 7 + 5 * LCD_WIDTH,
    0 + 1 * LCD_WIDTH, 4 + 5 * LCD_WIDTH,
    0 + 2 * LCD_WIDTH, 1 + 7 * LCD_WIDTH,
    1 + 6 * LCD_WIDTH, 7 + 7 * LCD_WIDTH,
    2 + 6 * LCD_WIDTH, 6 + 7 * LCD_WIDTH,
    7 + 0 * LCD_WIDTH, 6 + 4 * LCD_WIDTH,
    4 + 1 * LCD_WIDTH, 7 + 4 * LCD_WIDTH};

/*
 
       ###    ########  ########     ######## ######## ######## ########  ####  ######  
      ## ##   ##     ## ##     ##       ##    ##          ##    ##     ##  ##  ##    ## 
     ##   ##  ##     ## ##     ##       ##    ##          ##    ##     ##  ##  ##       
    ##     ## ########  ########        ##    ######      ##    ########   ##   ######  
    ######### ##        ##              ##    ##          ##    ##   ##    ##        ## 
    ##     ## ##        ##              ##    ##          ##    ##    ##   ##  ##    ## 
    ##     ## ##        ##              ##    ########    ##    ##     ## ####  ######  
 
*/
#define APP_TETRIS_FIGURES 7

const uint8_t tetrisFigures[(APP_TETRIS_FIGURES * 3)] = {
    COLOR_YELLOW, B110, B110,
    COLOR_DEEP_BLUE, B100, B111,
    // COLOR_WHITE, B101, B111,
    COLOR_ORANGE, B001, B111,
    COLOR_PURPLE, B010, B111,
    COLOR_GREEN, B011, B110,
    COLOR_RED, B110, B011,
    COLOR_LIGHT_BLUE, B000, B111};

/*
 
       ###    ########  ########     ##     ##    ###    ######## ######## 
      ## ##   ##     ## ##     ##    ###   ###   ## ##        ##  ##       
     ##   ##  ##     ## ##     ##    #### ####  ##   ##      ##   ##       
    ##     ## ########  ########     ## ### ## ##     ##    ##    ######   
    ######### ##        ##           ##     ## #########   ##     ##       
    ##     ## ##        ##           ##     ## ##     ##  ##      ##       
    ##     ## ##        ##           ##     ## ##     ## ######## ######## 
 
*/
#define APP_MAZE_BORDER_SIZE 1
#define APP_MAZE_MOVE_OFFSET 2
#define APP_MAZE_MAX_SCREENS_X 4 // Max 5
#define APP_MAZE_MAX_SCREENS_Y 2 // Max 5
#define APP_MAZE_MAX_PATH_STEPS 2 * 10

#define APP_MAZE_MAX_WIDTH (LCD_WIDTH * APP_MAZE_MAX_SCREENS_X)
#define APP_MAZE_MAX_HEIGHT (LCD_HEIGHT * APP_MAZE_MAX_SCREENS_Y)
#define APP_MAZE_MAX_VECTOR (APP_MAZE_MAX_WIDTH * APP_MAZE_MAX_HEIGHT - 1)
#define APP_MAZE_WALL 1
#define APP_MAZE_PATH 0
#define APP_MAZE_GENERATOR_MAX_STEPS ((APP_MAZE_MAX_WIDTH * APP_MAZE_MAX_HEIGHT) / 2 - 2)

#define DIRECTION_RIGHT 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_UP 3

#define APP_MAZE_MAX_BYTES (APP_MAZE_MAX_SCREENS_X * LCD_WIDTH * APP_MAZE_MAX_SCREENS_Y * LCD_HEIGHT / 8)

uint8_t maze[APP_MAZE_MAX_BYTES] = {};

/*
 
    ########  #######  ##    ## ########  ######  
       ##    ##     ## ###   ## ##       ##    ## 
       ##    ##     ## ####  ## ##       ##       
       ##    ##     ## ## ## ## ######    ######  
       ##    ##     ## ##  #### ##             ## 
       ##    ##     ## ##   ### ##       ##    ## 
       ##     #######  ##    ## ########  ######  
 
*/
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

/*
 
    ########  #######  ##    ## ######## 
    ##       ##     ## ###   ##    ##    
    ##       ##     ## ####  ##    ##    
    ######   ##     ## ## ## ##    ##    
    ##       ##     ## ##  ####    ##    
    ##       ##     ## ##   ###    ##    
    ##        #######  ##    ##    ##    
 
*/
uint8_t font[] = {
    // 0
    B11101010,
    B10101010,
    B11100000,
    // 1
    B01001100,
    B01000100,
    B11100000,
    // 2
    B11100010,
    B11101000,
    B11100000,
    // 3
    B11100010,
    B11100010,
    B11100000,
    // 4
    B10101010,
    B11100010,
    B00100000,
    // 5
    B11101000,
    B11100010,
    B11100000,
    // 6
    B11101000,
    B11101010,
    B11100000,
    // 7
    B11100010,
    B00100100,
    B01000000,
    // 8
    B11101010,
    B11101010,
    B11100000,
    // 9
    B11101010,
    B11100010,
    B11100000,

    // // A
    // B01001010,
    // B10101110,
    // B10100000,

    // // B
    // B11001010,
    // B11001010,
    // B11000000,

    // // C
    // B11101000,
    // B10001000,
    // B11100000,

    // // D
    // B11001010,
    // B10101010,
    // B11000000,

    // // E
    // B11101000,
    // B11101000,
    // B11100000,

    // // F
    // B11101000,
    // B11001000,
    // B10000000,

    // // G
    // B11101000,
    // B10001010,
    // B11100000,

    // // H
    // B10101010,
    // B11101010,
    // B10100000,

    // // I
    // B01000100,
    // B01000100,
    // B01000000,

    // // J
    // B11100010,
    // B00101010,
    // B01000000,

    // // K
    // B10101010,
    // B11001010,
    // B10100000,

    // // L
    // B10001000,
    // B10001000,
    // B11000000,

    // // M
    // B10011111,
    // B10011001,
    // B10010000,

    // // N
    // B10011001,
    // B11011011,
    // B10010000,
    // // O
    // B11101010,
    // B10101010,
    // B11100000,
    // // P
    // B11101010,
    // B11101000,
    // B10000000,
    // // Q
    // B01001010,
    // B10101010,
    // B01100000,
    // // R
    // B11101010,
    // B11001010,
    // B10100000,
    // // S
    // B11101000,
    // B11100010,
    // B11100000,
    // // T
    // B11100100,
    // B01000100,
    // B01000000,
    // // U
    // B10101010,
    // B10101010,
    // B11100000,
    // // V
    // B10101010,
    // B10101010,
    // B01000000,
    // // W
    // B10011001,
    // B10011101,
    // B11110000,
    // // X
    // B10101010,
    // B01001010,
    // B10100000,
    // // Y
    // B10101010,
    // B11100100,
    // B01000000,
    // // Z
    // B11100010,
    // B01001000,
    // B11100000,
};

/*
 
     ######  ######## ########  ##     ##  ######  ######## 
    ##    ##    ##    ##     ## ##     ## ##    ##    ##    
    ##          ##    ##     ## ##     ## ##          ##    
     ######     ##    ########  ##     ## ##          ##    
          ##    ##    ##   ##   ##     ## ##          ##    
    ##    ##    ##    ##    ##  ##     ## ##    ##    ##    
     ######     ##    ##     ##  #######   ######     ##    
 
*/
struct AppStructure
{
    uint8_t isInit : 1;
    uint8_t needCheck : 1;
    uint8_t customRender : 1;

    // uint8_t changeState : 1;
    uint16_t score;
    uint8_t level;
    // uint8_t maxLevel;

    uint32_t timer;
    uint16_t timerPeriod;
};

struct Vector
{
    uint8_t x;
    uint8_t y;
};

struct ButtonStructure
{
    Button up;
    Button down;
    Button left;
    Button right;
    Button a;
    Button b;
};

struct CursorStructure
{
    uint8_t type : 2;     // Тип курсора
    uint8_t controlY : 1; // Разрешить управлять курсором по оси Y
    uint8_t controlX : 1;
    uint8_t rollback : 1; // Возвращает координаты предыдущего тика

    uint8_t isNextVector : 1; // У курсора изменился nextVector
    uint8_t isTouch : 1;      // Курсор коснулся объекта или границы
    uint8_t isTouchDown : 1;  // Курсор коснулся нижней границы
    uint8_t isOutsideLcd : 1; // Курсор выходит за пределы экрана
    uint8_t borderTop : 1;    // Граница вверху
    uint8_t moveSound : 1;

    uint8_t angle; // Угол

    uint8_t mode; // Режим взаимодействия курсора

    // Текущие координаты курсора
    int8_t x;
    int8_t y;
    uint16_t vector;

    // Следующие координаты курсора
    int8_t nextX;
    int8_t nextY;
    uint16_t nextVector;

    uint8_t color;     // Цвет (номер)
    uint32_t colorRGB; // Цвет RGB
    uint8_t icon[3];   // Иконка курсора

    // Размеры курсора
    uint8_t width;
    uint8_t height;
};

struct BlinkStructure
{
    boolean toggler;
    uint8_t level;
    uint32_t timer;
};

struct MazeGenerator
{
    uint16_t steps;
    uint8_t pathSteps;
    uint16_t trackingVector;
    uint8_t direction;
    int8_t x;
    int8_t y;
    bool randomSteps;
    bool findAltPosition;
};