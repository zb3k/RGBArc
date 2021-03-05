#include <Arduino.h>
#include <Lcd/Lcd.h>

// -----------------------------------------------------------------------------
// Core methods
// -----------------------------------------------------------------------------

void Lcd::setup()
{
#ifdef LCD_USE_NEOPIXEL
    NEO.begin();
#else
    LED.setColorOrderRGB();
    LED.setOutput(LCD_PIN);
#endif
}

void Lcd::setBrightness(uint8_t brightness)
{
#ifdef LCD_USE_NEOPIXEL
    NEO.setBrightness(brightness);
#else
    for (uint8_t i = 0; i < PALETTE_SIZE; i++)
    {
        this->paletteRGB[i] = this->colorBrightness(i, brightness);
    }
#endif
}

void Lcd::sync()
{
#ifdef LCD_USE_NEOPIXEL
    NEO.show();
#else
    LED.sync();
#endif
}

// -----------------------------------------------------------------------------
// Direct draw methods
// -----------------------------------------------------------------------------

void Lcd::clear()
{
    this->fill(COLOR_BLACK);
}

void Lcd::setPixel(uint16_t vector, uint8_t colorIndex)
{
#ifdef LCD_USE_NEOPIXEL
    NEO.setPixelColor(vector, paletteRGB[colorIndex]);
#else
    LED.set_crgb_at(vector, this->paletteRGB[colorIndex]);
#endif
}

void Lcd::setPixel(uint16_t vector, uint8_t colorIndex, uint8_t brightness)
{
#ifdef LCD_USE_NEOPIXEL
    NEO.setPixelColor(vector, this->colorBrightness(colorIndex, brightness));
#else
    LED.set_crgb_at(vector, this->colorBrightness(colorIndex, brightness));
#endif
}

void Lcd::fill(uint8_t colorIndex)
{
    for (uint16_t vector = 0; vector < LCD_NUMPIXELS; vector++)
    {
        this->setPixel(vector, colorIndex);
    }
}

// -----------------------------------------------------------------------------
// Scene methods
// -----------------------------------------------------------------------------

void Lcd::renderScene()
{
    for (uint16_t vector = 0; vector < LCD_NUMPIXELS; vector++)
    {
        this->setPixel(vector, this->scene[vector]);
    }
}

void Lcd::clearScene()
{
    this->fillScene(COLOR_BLACK);
}

void Lcd::setScenePixel(uint16_t vector, uint8_t colorIndex)
{
    this->scene[vector] = colorIndex;
}

void Lcd::setScenePixel(uint8_t x, uint8_t y, uint8_t colorIndex)
{
    this->setScenePixel(LCD_VECTOR(x, y), colorIndex);
}

void Lcd::fillScene(uint8_t colorIndex)
{
    for (uint16_t vector = 0; vector < LCD_NUMPIXELS; vector++)
    {
        this->scene[vector] = colorIndex;
    }
}

void Lcd::loadImageToScene(const uint8_t *image)
{
    uint8_t totalColors = pgm_read_byte(image);

    for (uint8_t i = 0; i < totalColors; i++)
    {
        uint8_t offset = 1 + (i * (LCD_HEIGHT + 1));
        uint8_t color = pgm_read_byte(image + offset);
        offset++;
        for (uint8_t y = 0; y < LCD_HEIGHT; y++)
        {
            uint8_t part = pgm_read_byte(image + offset + y);

            for (uint8_t x = 0; x < LCD_WIDTH; x++)
            {
                if (bitRead(part, 7 - x))
                {
                    this->setScenePixel(x, y, color);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// PRIVATE: Helpers
// -----------------------------------------------------------------------------

#ifdef LCD_USE_NEOPIXEL
uint32_t Lcd::colorBrightness(uint8_t colorIndex, uint8_t brightness)
{
    uint32_t color = this->paletteRGB[colorIndex];

    uint8_t r = map(color >> 16, 0, 255, 0, brightness);
    uint8_t g = map(color >> 8 & 0xFF, 0, 255, 0, brightness);
    uint8_t b = map(color & 0xFF, 0, 255, 0, brightness);

    return NEO.Color(r, g, b);
}
#else
cRGB Lcd::colorBrightness(uint8_t colorIndex, uint8_t brightness)
{
    cRGB color = this->paletteRGB[colorIndex];
    color.r = map(color.r, 0, 255, 0, brightness);
    color.g = map(color.g, 0, 255, 0, brightness);
    color.b = map(color.b, 0, 255, 0, brightness);

    return color;
}
#endif
