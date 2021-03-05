#ifndef Lcd_h
#define Lcd_h

#include <header.h>

// #define LCD_USE_NEOPIXEL

#ifdef LCD_USE_NEOPIXEL
#include <Adafruit_NeoPixel.h>
#else
#include <WS2812.h>
#endif

#define LCD_VECTOR(X, Y) (X + Y * LCD_WIDTH)

class Lcd
{
public:
#ifdef LCD_USE_NEOPIXEL
    Lcd()
        : NEO(LCD_NUMPIXELS, LCD_PIN, NEO_GRB)
    {
    }
#else

    Lcd()
        : LED(LCD_NUMPIXELS)
    {
    }
#endif

    uint8_t scene[LCD_NUMPIXELS];
    // uint8_t buffer[LCD_NUMPIXELS];

    // Core methods
    void setup();
    void sync();
    void setBrightness(uint8_t brightness);

    // Direct draw methods
    void clear();
    void setPixel(uint16_t vector, uint8_t colorIndex);
    void setPixel(uint16_t vector, uint8_t colorIndex, uint8_t brightness);
    void fill(uint8_t colorIndex);

    // Scene methods
    void renderScene();
    void clearScene();
    void setScenePixel(uint16_t vector, uint8_t colorIndex);
    void setScenePixel(uint8_t x, uint8_t y, uint8_t colorIndex);
    void fillScene(uint8_t colorIndex);
    void loadImageToScene(const uint8_t *image);

private:
#ifdef LCD_USE_NEOPIXEL
    Adafruit_NeoPixel NEO = Adafruit_NeoPixel();
    uint32_t paletteRGB[PALETTE_SIZE] = {
        0x000000,  // COLOR_BLACK
        0xFFFFFF,  // COLOR_WHITE
        0xFF0000,  // COLOR_RED
        0xFF9900,  // COLOR_ORANGE
        0xFFFF00,  // COLOR_YELLOW
        0x00FF00,  // COLOR_GREEN
        0x00FFFF,  // COLOR_CYAN
        0x0000FF,  // COLOR_BLUE
        0x6600FF,  // COLOR_PURPLE
        0xFF00FF}; // COLOR_PINK
#else
    WS2812 LED;
    cRGB paletteRGB[PALETTE_SIZE] = {
        {0x00, 0x00, 0x00},  // COLOR_BLACK
        {0xFF, 0xFF, 0xFF},  // COLOR_WHITE
        {0xFF, 0x00, 0x00},  // COLOR_RED
        {0xFF, 0x99, 0x00},  // COLOR_ORANGE
        {0xFF, 0xFF, 0x00},  // COLOR_YELLOW
        {0x00, 0xFF, 0x00},  // COLOR_GREEN
        {0x00, 0xFF, 0xFF},  // COLOR_CYAN
        {0x00, 0x00, 0xFF},  // COLOR_BLUE
        {0x66, 0x00, 0xFF},  // COLOR_PURPLE
        {0xFF, 0x00, 0xFF}}; // COLOR_PINK
#endif

    // Helpers
#ifdef LCD_USE_NEOPIXEL
    uint32_t colorBrightness(uint8_t colorIndex, uint8_t brightness);
#else
    cRGB colorBrightness(uint8_t colorIndex, uint8_t brightness);
#endif
};

#endif