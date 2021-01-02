#ifndef Lcd_h
#define Lcd_h

#include <header.h>
#include <WS2812.h>

#define LCD_NUMPIXELS (LCD_WIDTH * LCD_HEIGHT)

#define LCD_VECTOR(X, Y) (X + Y * LCD_WIDTH)

class Lcd
{
public:
    Lcd()
        : LED(LCD_NUMPIXELS) {}

    uint8_t scene[LCD_NUMPIXELS];
    // uint8_t buffer[LCD_NUMPIXELS];

    // Core methods
    void setup();
    void show();
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

    // Helpers
    cRGB colorBrightness(uint8_t colorIndex, uint8_t brightness);
};

#endif