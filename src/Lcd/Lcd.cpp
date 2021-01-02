#include <Arduino.h>
#include <Lcd/Lcd.h>

// -----------------------------------------------------------------------------
// Core methods
// -----------------------------------------------------------------------------

void Lcd::setup()
{
    this->LED.setColorOrderRGB();
    this->LED.setOutput(LCD_PIN);
}

void Lcd::setBrightness(uint8_t brightness)
{
    for (uint8_t i = 0; i < PALETTE_SIZE; i++)
    {
        this->paletteRGB[i] = this->colorBrightness(i, brightness);
    }
}

void Lcd::show()
{
    this->LED.sync();
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
    this->LED.set_crgb_at(vector, this->paletteRGB[colorIndex]);
}

void Lcd::setPixel(uint16_t vector, uint8_t colorIndex, uint8_t brightness)
{
    this->LED.set_crgb_at(vector, this->colorBrightness(colorIndex, brightness));
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

cRGB Lcd::colorBrightness(uint8_t colorIndex, uint8_t brightness)
{
    cRGB color = this->paletteRGB[colorIndex];
    color.r = map(color.r, 0, 255, 0, brightness);
    color.g = map(color.g, 0, 255, 0, brightness);
    color.b = map(color.b, 0, 255, 0, brightness);

    return color;
}