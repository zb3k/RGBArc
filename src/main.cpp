/*
           _____                    _____                    _____          
          /\    \                  /\    \                  /\    \         
         /::\    \                /::\    \                /::\    \        
        /::::\    \              /::::\    \              /::::\    \       
       /::::::\    \            /::::::\    \            /::::::\    \      
      /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \     
     /:::/__\:::\    \        /:::/  \:::\    \        /:::/__\:::\    \    
    /::::\   \:::\    \      /:::/    \:::\    \      /::::\   \:::\    \   
   /::::::\   \:::\    \    /:::/    / \:::\    \    /::::::\   \:::\    \  
  /:::/\:::\   \:::\____\  /:::/    /   \:::\ ___\  /:::/\:::\   \:::\ ___\ 
 /:::/  \:::\   \:::|    |/:::/____/  ___\:::|    |/:::/__\:::\   \:::|    |
 \::/   |::::\  /:::|____|\:::\    \ /\  /:::|____|\:::\   \:::\  /:::|____|
  \/____|:::::\/:::/    /  \:::\    /::\ \::/    /  \:::\   \:::\/:::/    / 
        |:::::::::/    /    \:::\   \:::\ \/____/    \:::\   \::::::/    /  
        |::|\::::/    /      \:::\   \:::\____\       \:::\   \::::/    /   
        |::| \::/____/        \:::\  /:::/    /        \:::\  /:::/    /    
        |::|  ~|               \:::\/:::/    /          \:::\/:::/    /     
        |::|   |                \::::::/    /            \::::::/    /      
        \::|   |                 \::::/    /              \::::/    /       
         \:|   |                  \::/____/                \::/____/        
          \|___|                                            ~~              
                                                                            
           _____                    _____                    _____          
          /\    \                  /\    \                  /\    \         
         /::\    \                /::\    \                /::\    \        
        /::::\    \              /::::\    \              /::::\    \       
       /::::::\    \            /::::::\    \            /::::::\    \      
      /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \     
     /:::/__\:::\    \        /:::/__\:::\    \        /:::/  \:::\    \    
    /::::\   \:::\    \      /::::\   \:::\    \      /:::/    \:::\    \   
   /::::::\   \:::\    \    /::::::\   \:::\    \    /:::/    / \:::\    \  
  /:::/\:::\   \:::\    \  /:::/\:::\   \:::\____\  /:::/    /   \:::\    \ 
 /:::/  \:::\   \:::\____\/:::/  \:::\   \:::|    |/:::/____/     \:::\____\
 \::/    \:::\  /:::/    /\::/   |::::\  /:::|____|\:::\    \      \::/    /
  \/____/ \:::\/:::/    /  \/____|:::::\/:::/    /  \:::\    \      \/____/ 
           \::::::/    /         |:::::::::/    /    \:::\    \             
            \::::/    /          |::|\::::/    /      \:::\    \            
            /:::/    /           |::| \::/____/        \:::\    \           
           /:::/    /            |::|  ~|               \:::\    \          
          /:::/    /             |::|   |                \:::\    \         
         /:::/    /              \::|   |                 \:::\____\        
         \::/    /                \:|   |                  \::/    /        
          \/____/                  \|___|                   \/____/         

*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <JC_Button.h>
#include "header.h"

//-----------------------------------------------------------------------------

uint32_t paletteRGB[PALETTE_COLORS] = {};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
uint8_t lcd[NUMPIXELS] = {};
char lcdCharBuffer[LCD_TEXT_BUFFER_SIZE];
int8_t lcdCharOffset = 0;
CursorStructure cursor = {};
ButtonStructure btn = {Button(BTN_UP), Button(BTN_DOWN), Button(BTN_LEFT), Button(BTN_RIGHT), Button(BTN_A), Button(BTN_B)};
BlinkStructure blink = {false, 0, 0};
uint8_t currentApp = DEFAULT_APP;
AppStructure app = {};
MazeGenerator generator = {};

/*
   ##     ## ######## ##       ########  ######## ########   ######
   ##     ## ##       ##       ##     ## ##       ##     ## ##    ##
   ##     ## ##       ##       ##     ## ##       ##     ## ##
   ######### ######   ##       ########  ######   ########   ######
   ##     ## ##       ##       ##        ##       ##   ##         ##
   ##     ## ##       ##       ##        ##       ##    ##  ##    ##
   ##     ## ######## ######## ##        ######## ##     ##  ######
*/

bool timerEach(uint16_t period)
{
  uint32_t now = millis();

  if (app.timer == 0)
  {
    app.timer = now;
  }

  if (now >= app.timer)
  {
    app.timer += period;
    return true;
  }

  return false;
}

//-----------------------------------------------------------------------------

/*
   ##        ######  ########
   ##       ##    ## ##     ##
   ##       ##       ##     ##
   ##       ##       ##     ##
   ##       ##       ##     ##
   ##       ##    ## ##     ##
   ########  ######  ########
*/

/**
 * Выводит символ на экран
 * @param  {uint8_t} offsetX      :
 * @param  {uint8_t} offsetY     :
 * @param  {uint8_t} charIndex :
 * @param  {uint8_t} color     :
 */
void lcdChar(int8_t offsetX, int8_t offsetY, char charIndex, uint8_t color)
{
  int8_t charX = 0;
  int8_t charY = 0;
  int8_t lcdX = 0;
  int8_t lcdY = 0;

  if (charIndex >= 65)
  {
    charIndex = charIndex - 65 + 10;
  }
  else if (charIndex >= 48)
  {
    charIndex = charIndex - 48;
  }
  charIndex = charIndex * 3;

  for (uint8_t i = 0; i < 3; i++)
  {
    for (uint8_t j = 0; j < 8; j++)
    {
      charX = j > 3 ? j - 4 : j;
      lcdX = charX + offsetX;
      lcdY = charY + offsetY;

      if (lcdX >= 0 && lcdY >= 0 && lcdX < LCD_WIDTH && lcdY < LCD_HEIGHT)
      {
        lcd[lcdX + lcdY * LCD_WIDTH] = bitRead(font[charIndex + i], 7 - j) ? color : COLOR_BLACK;
      }

      if (charX == 3)
      {
        charY++;
      }
    }
  }
}

void lcdPrint(String stringOne)
{
  uint8_t length = 0;
  uint8_t color = PALETTE_COLOR_FIRST;

  stringOne.toCharArray(lcdCharBuffer, LCD_TEXT_BUFFER_SIZE);
  for (uint8_t i = 0; i < LCD_TEXT_BUFFER_SIZE; i++)
  {
    length++;

    if (lcdCharBuffer[i] == 0)
    {
      break;
    }

    lcdChar(1 + i * 4 + lcdCharOffset, 1, lcdCharBuffer[i], color);
    color++;
    if (color > PALETTE_COLOR_LAST)
    {
      color = PALETTE_COLOR_FIRST;
    }
  }

  if (timerEach(120))
  {
    lcdCharOffset--;
    if (lcdCharOffset < -length * 4)
    {
      lcdCharOffset = LCD_WIDTH - 1;
    }
  }

  // lcdChar(0, 1, length, COLOR_WHITE);
  // lcdChar(0 - 1, 1, lcdCharBuffer[0], COLOR_WHITE);
  // lcdChar(3 - 1, 1, lcdCharBuffer[1], COLOR_RED);
  // lcdChar(6 - 1, 1, lcdCharBuffer[2], COLOR_PINK);
}

/**
 * Очищает буфер вывода экрана
 */
void lcdClear()
{
  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    lcd[i] = COLOR_BLACK;
  }
}

/**
 * Удаляет все точки определенного цвета
 * @param  {uint8_t} removeColor :
 */
void lcdRemoveColor(uint8_t removeColor)
{
  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    if (lcd[i] == removeColor)
    {
      lcd[i] = COLOR_BLACK;
    }
  }
}

// /**
//  * Проверяет имеет ли соседние точки заданный цвет
//  * @param  {uint8_t} vector :
//  * @param  {uint8_t} color  :
//  * @return {bool}           :
//  */
// bool lcdHasColorAround(uint8_t vector, uint8_t color)
// {
//   if (lcd[vector + 1] == color ||
//       lcd[vector - 1] == color ||
//       lcd[vector + 8] == color ||
//       lcd[vector - 8] == color)
//   {
//     return true;
//   }
//   return false;
// }

/**
 * Рисует точку заданного цвета палитры
 */
void lcdDrawPixel(uint8_t vector, uint8_t color)
{
  pixels.setPixelColor(vector, paletteRGB[color]);
}

/**
 * Заполняет линию на экране
 */
void lcdFillLine(uint8_t y, uint8_t color)
{
  for (uint8_t x = 0; x < LCD_WIDTH; x++)
  {
    lcd[x + y * LCD_WIDTH] = color;
  }
}

/**
 * Удаляет линию с экрана
 * @param  {uint8_t} removeLine :
 */
void lcdRemoveLine(uint8_t removeLine)
{
  for (int8_t vector = removeLine * LCD_WIDTH + LCD_WIDTH - 1; vector > 0; vector--)
  {
    lcd[vector] = lcd[vector - LCD_WIDTH];
  }
  lcdFillLine(0, COLOR_BLACK);
}

void lcdImage(const uint8_t *image)
{
  uint8_t totalColors = image[0];
  lcdClear();

  for (uint8_t i = 0; i < totalColors; i++)
  {
    uint8_t offset = 1 + (i * (LCD_HEIGHT + 1));
    uint8_t color = image[offset];
    offset++;
    for (uint8_t y = 0; y < LCD_HEIGHT; y++)
    {
      for (uint8_t x = 0; x < LCD_WIDTH; x++)
      {
        if (bitRead(image[offset + y], 7 - x))
        {
          lcd[x + y * LCD_WIDTH] = color;
        }
      }
    }
  }
}

void cursorRender()
{
  uint8_t i, j;

  if (cursor.type)
  {
    if (cursor.color == COLOR_WHITE)
    {
      cursor.colorRGB = pixels.Color(blink.level, blink.level, blink.level);
    }
    else
    {
      cursor.colorRGB = pixels.ColorHSV(paletteHue[cursor.color], 0xFF, blink.level);
    }

    if (cursor.type == CURSOR_TYPE_PIXEL)
    {
      pixels.setPixelColor(cursor.vector, cursor.colorRGB);
    }
    else
    {
      for (i = 0; i < cursor.width; i++)
      {
        for (j = 0; j < cursor.height; j++)
        {
          if (bitRead(cursor.icon[j], 2 - i))
          {
            pixels.setPixelColor(cursor.vector + i + (j)*LCD_WIDTH, cursor.colorRGB);
          }
        }
      }
    }
  }
}
/**
 * Выводим изображение на экран
 */
void lcdRender()
{
  uint8_t i;

  for (i = 0; i < NUMPIXELS; i++)
  {
    lcdDrawPixel(i, lcd[i]);
  }
}

/**
 * Выводим заставку
 * @param  {uint8_t} type :
 */
void lcdSplahScreen(uint8_t type)
{
  uint8_t i, j;

  switch (type)
  {
  case SPLASH_SCREEN_RAINBOW:
    for (i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, paletteRGB[i / 8 + 2]);
      pixels.show();
      delay(40);
    }
    break;

  case SPLASH_SCREEN_FADE_OUT:
    for (j = 255; j > 0; j--)
    {
      for (i = 0; i < NUMPIXELS; i++)
      {
        if (lcd[i])
        {
          pixels.setPixelColor(i, pixels.ColorHSV(paletteHue[lcd[i]], 0xFF, j));
        }
      }
      pixels.show();
      delay(1);
    }
    break;

  case SPLASH_SCREEN_FLASH:
    for (j = 255; j > 0; j -= 5)
    {
      for (i = 0; i < NUMPIXELS; i++)
      {
        if (lcd[i])
        {
          pixels.setPixelColor(i, pixels.ColorHSV(paletteHue[lcd[i]], j));
        }
      }
      pixels.show();
    }
    for (j = 0; j < 255; j += 5)
    {
      for (i = 0; i < NUMPIXELS; i++)
      {
        if (lcd[i])
        {
          pixels.setPixelColor(i, pixels.ColorHSV(paletteHue[lcd[i]], j));
        }
      }
      pixels.show();
    }
    break;
  }
}

/*
    ######   #######  ##     ## ##    ## ########
   ##    ## ##     ## ##     ## ###   ## ##     ##
   ##       ##     ## ##     ## ####  ## ##     ##
    ######  ##     ## ##     ## ## ## ## ##     ##
         ## ##     ## ##     ## ##  #### ##     ##
   ##    ## ##     ## ##     ## ##   ### ##     ##
    ######   #######   #######  ##    ## ########
*/
void soundBeep()
{
  tone(BUZZER_PIN, NOTE_C4, 30);
  delay(30);
  // delay(1000 / 4 * 1.3);
}

void soundMelody()
{
  // TETRIS THEME
  int notes[] = {
      NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
      NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
      NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
      NOTE_C5, NOTE_A4, NOTE_A4, 0,
      // 0, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
      // NOTE_E5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
      // NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
      // NOTE_C5, NOTE_A4, NOTE_A4, 0
  };

  int noteDurations[] = {
      4, 8, 8, 4, 8, 8,
      4, 8, 8, 4, 8, 8,
      4, 8, 8, 4, 4,
      4, 4, 4, 4,
      // 8, 4, 8, 4, 8, 8,
      // 4, 8, 8, 4, 8, 8,
      // 4, 8, 8, 4, 4,
      // 4, 4, 4, 4
  };

  for (int thisNote = 0; thisNote < 21; thisNote++)
  {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, notes[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

/*
    ######   #######  ##    ## ######## ########   #######  ##        ######
   ##    ## ##     ## ###   ##    ##    ##     ## ##     ## ##       ##    ##
   ##       ##     ## ####  ##    ##    ##     ## ##     ## ##       ##
   ##       ##     ## ## ## ##    ##    ########  ##     ## ##        ######
   ##       ##     ## ##  ####    ##    ##   ##   ##     ## ##             ##
   ##    ## ##     ## ##   ###    ##    ##    ##  ##     ## ##       ##    ##
    ######   #######  ##    ##    ##    ##     ##  #######  ########  ######
*/

/**
 * Задает позицию курсору
 * @param  {uint8_t} x :
 * @param  {uint8_t} y :
 */
void cursorSetPosition(uint8_t x, uint8_t y)
{
  cursor.nextX = cursor.x = x;
  cursor.nextY = cursor.y = y;
  cursor.nextVector = cursor.vector = x + y * LCD_WIDTH;
}

/**
 * Задает иконку курсору
 * @param  {uint8_t} line1 :
 * @param  {uint8_t} line2 :
 * @param  {uint8_t} line3 :
 */
void cursorSetIcon(uint8_t line1, uint8_t line2, uint8_t line3)
{
  uint8_t mix = line1 | line2 | line3;

  cursor.width = mix & B001 ? 3 : mix & B010 ? 2 : 1;
  cursor.height = line3 ? 3 : line2 ? 2 : 1;

  cursor.icon[0] = line1;
  cursor.icon[1] = line2;
  cursor.icon[2] = line3;
}

/**
 * Задает иконку курсору
 * @param  {uint8_t*} icon :
 */
void cursorSetIcon(uint8_t *icon)
{
  cursorSetIcon(icon[0], icon[1], icon[2]);
}

/**
 * Поворачивает курсор по часовой стрелки на 90 градусов
 */
void cursorRotate()
{
  uint8_t icon[MAX_ICON_SIZE];
  uint8_t i, j;

  // cursor.angle = cursor.angle == 3 ? 0 : cursor.angle + 1;

  icon[0] = B000;
  icon[1] = B000;
  icon[2] = B000;

  for (i = 0; i < MAX_ICON_SIZE; i++)
  {
    for (j = 0; j < MAX_ICON_SIZE; j++)
    {

      if (bitRead(cursor.icon[j], i))
      {
        bitSet(icon[2 - i], j);
      }
    }
  }

  cursorSetIcon(icon);
}

/**
 * Копирует курсор на экран
 */
void cursorToLcd()
{
  if (cursor.type == CURSOR_TYPE_PIXEL)
  {
    lcd[cursor.vector] = cursor.color;
  }
  else if (cursor.type == CURSOR_TYPE_ICON)
  {
    for (uint8_t i = 0; i < cursor.width; i++)
    {
      for (uint8_t j = 0; j < cursor.height; j++)
      {
        if (bitRead(cursor.icon[j], 2 - i))
        {
          lcd[cursor.vector + i + j * LCD_WIDTH] = cursor.color;
        }
      }
    }
  }
}

/**
 * Проверяет не перекрывает ли точка другие точки на экране или не выходит за пределы экрана
 * @param  {int8_t} x        :
 * @param  {int8_t} y        :
 * @param  {uint16_t} vector :
 * @return {bool}            :
 */
bool isPixelOverlapping(int8_t x, int8_t y, uint16_t vector)
{
  return (cursor.mode == CURSOR_MODE_PARTICLE && vector <= NUMPIXELS && vector >= 0 && lcd[vector]) ||
         (/* cursor.borderRight && */ x >= LCD_WIDTH) ||
         (/* cursor.borderLeft && */ x < 0) ||
         (/* cursor.borderBottom && */ y >= LCD_HEIGHT) ||
         (cursor.borderTop && y < 0);
}

/**
 * Проверяет не перекрывает ли следующая позиция курсора другие точки или не выходит за пределы экрана
 * @return {bool}  :
 */
bool isCursorOverlapping()
{
  cursor.isTouchDown = false;

  // проверка для точки
  if (cursor.type == CURSOR_TYPE_PIXEL)
  {
    // cursor.isTouchDown = cursor.nextX >= 0 && cursor.nextX < LCD_WIDTH;
    return isPixelOverlapping(cursor.nextX, cursor.nextY, cursor.nextVector);
  }
  else
  {
    for (uint8_t x = 0; x < MAX_ICON_SIZE; x++)
    {
      for (uint8_t y = 0; y < MAX_ICON_SIZE; y++)
      {
        if (bitRead(cursor.icon[y], 2 - x))
        {
          int8_t nextPixelPosition = cursor.nextVector + x + y * LCD_WIDTH;
          int8_t nextX = cursor.nextX + x;
          int8_t nextY = cursor.nextY + y;
          if (isPixelOverlapping(nextX, nextY, nextPixelPosition))
          {
            cursor.isTouchDown = nextX >= 0 && nextX < LCD_WIDTH;
            return true;
          }
        }
      }
    }
  }

  return false;
}

/**
 * Возвращает true если кнопка нажата каждые LONG_PRESS_INTERVAL
 * @return {bool}  :
 */
bool isLongPressTick()
{
  static uint32_t longPressTimer = 0;

  if (longPressTimer)
  {
    if (millis() >= longPressTimer)
    {
      longPressTimer = 0;
    }
  }
  else
  {
    longPressTimer = millis() + LONG_PRESS_INTERVAL;
  }

  return longPressTimer ? false : true;
}

/**
 * Обработчик курсора
 */
void cursorTick()
{
  // Курсор отключен
  if (!cursor.type)
  {
    return;
  }

  // Отмена передвижения
  if (cursor.rollback)
  {
    cursor.rollback = false;
    cursor.nextX = cursor.x;
    cursor.nextY = cursor.y;
    cursor.nextVector = cursor.vector;
  }
  // Перемещение
  else
  {
    // Звук если есть движение
    if (cursor.moveSound && cursor.isNextVector)
    {
      soundBeep();
    }

    cursor.isTouch = false;
    cursor.vector = cursor.nextVector;
    cursor.x = cursor.nextX;
    cursor.y = cursor.nextY;
  }

  // Управление по X
  if (cursor.controlX)
  {
    if (btn.right.wasPressed() || (btn.right.pressedFor(LONG_PRESS_WAIT) && isLongPressTick()))
    {
      cursor.nextX = cursor.x + 1;
    }
    if (btn.left.wasPressed() || (btn.left.pressedFor(LONG_PRESS_WAIT) && isLongPressTick()))
    {
      cursor.nextX = cursor.x - 1;
    }
  }

  // Управление по Y
  if (cursor.controlY)
  {
    if (btn.down.wasPressed() || (btn.down.pressedFor(LONG_PRESS_WAIT) && isLongPressTick()))
    {
      cursor.nextY = cursor.y + 1;
    }
    if (btn.up.wasPressed() || (btn.up.pressedFor(LONG_PRESS_WAIT) && isLongPressTick()))
    {
      cursor.nextY = cursor.y - 1;
    }
  }

  // Следующий вектор
  cursor.nextVector = cursor.nextX + cursor.nextY * LCD_WIDTH;
  cursor.isNextVector = cursor.nextVector != cursor.vector;

  if (cursor.isNextVector && isCursorOverlapping())
  {
    cursor.rollback = true;
    cursor.isTouch = true;
    // cursor.isTouchDown = cursor.height + cursor.nextY > LCD_HEIGHT;
  }

  cursor.isOutsideLcd = cursor.vector > NUMPIXELS;
}

/**
 * Сброс параметров app
 */
void resetApp()
{
  app.isInit = false;
  app.needCheck = false;
  app.customRender = false;
  app.level = 0;
  app.score = 0;
  app.timer = 0;
  app.timerPeriod = 0;
}

/**
 * Сброс параметров курсора
 */
void resetCursor()
{
  cursor.type = CURSOR_TYPE_DISABLED;
  cursor.mode = CURSOR_MODE_DEFAULT;
  cursor.color = COLOR_DEFAULT;
  cursor.width = 1;
  cursor.height = 1;
  cursor.controlY = true;
  cursor.controlX = true;
  cursor.borderTop = true;
  cursor.isTouch = false;
  cursor.isTouchDown = false;
  cursor.isOutsideLcd = false;
  cursor.moveSound = true;
}

/**
 * Конец игры
 */
void gameOver()
{
  tone(BUZZER_PIN, NOTE_C3, 200);
  delay(300);
  lcdSplahScreen(SPLASH_SCREEN_RAINBOW);
  delay(1000);

  lcdClear();
  resetCursor();
  resetApp();
  currentApp = 0;
}

void gameWin()
{
  resetCursor();
  lcdRender();
  soundMelody();
  lcdSplahScreen(SPLASH_SCREEN_RAINBOW);
  delay(2000);
  resetApp();
  lcdClear();
  currentApp = 0;
}

/*
      ###    ########  ########     ########     ###    #### ##    ## ########
     ## ##   ##     ## ##     ##    ##     ##   ## ##    ##  ###   ##    ##
    ##   ##  ##     ## ##     ##    ##     ##  ##   ##   ##  ####  ##    ##
   ##     ## ########  ########     ########  ##     ##  ##  ## ## ##    ##
   ######### ##        ##           ##        #########  ##  ##  ####    ##
   ##     ## ##        ##           ##        ##     ##  ##  ##   ###    ##
   ##     ## ##        ##           ##        ##     ## #### ##    ##    ##
*/
void appPaint()
{
  static boolean allowChangeColor = true;

  cursor.type = CURSOR_TYPE_PIXEL;

  if (btn.a.isPressed() && btn.b.isPressed())
  {
    lcdClear();
  }
  else
  {
    if (btn.a.isPressed())
    {
      cursorToLcd();
    }

    if (btn.b.pressedFor(100))
    {
      lcd[cursor.vector] = COLOR_BLACK;
      allowChangeColor = false;
    }
    else if (btn.b.wasReleased())
    {
      if (allowChangeColor)
      {
        allowChangeColor = true;
      }
      else
      {
        cursor.color = cursor.color >= PALETTE_COLORS - 1 ? PALETTE_COLOR_FIRST - 1 : cursor.color + 1;
      }
    }
  }
}

//-----------------------------------------------------------------------------

/*
      ###    ########  ########     ######## ##        #######  ##      ##
     ## ##   ##     ## ##     ##    ##       ##       ##     ## ##  ##  ##
    ##   ##  ##     ## ##     ##    ##       ##       ##     ## ##  ##  ##
   ##     ## ########  ########     ######   ##       ##     ## ##  ##  ##
   ######### ##        ##           ##       ##       ##     ## ##  ##  ##
   ##     ## ##        ##           ##       ##       ##     ## ##  ##  ##
   ##     ## ##        ##           ##       ########  #######   ###  ###
*/
void appFlow()
{
  static uint8_t levelIndex = 0;
  uint8_t removeColor = 0;
  uint8_t i, color, vectorA, vectorB;

  // INIT
  if (!app.isInit)
  {
    app.isInit = true;
    cursor.type = CURSOR_TYPE_PIXEL;
    cursor.mode = CURSOR_MODE_DEFAULT;
  }

  // Check compete
  if (app.needCheck)
  {
    app.needCheck = false;

    // Check game
    for (i = 0; i < NUMPIXELS; i++)
    {
      if (lcd[i] == COLOR_BLACK)
      {
        break;
      }
    }

    // Level complete
    if (i == NUMPIXELS)
    {
      lcdSplahScreen(SPLASH_SCREEN_FLASH);
      delay(300);
      lcdSplahScreen(SPLASH_SCREEN_FADE_OUT);
      delay(300);

      // Game complete
      if (app.level == APP_FLOW_LEVELS - 1)
      {
        gameWin();
        levelIndex = 0;
        return;
      }

      // Next level
      lcdClear();
      app.level++;
      levelIndex = 0;
      for (i = 0; i < app.level; i++)
      {
        levelIndex += flowLevelsSizes[i] * 2;
      }
    }
  }

  static bool allowDrawing = true;
  static uint8_t drawLineColor = COLOR_BLACK;
  static uint8_t startVector;
  static uint8_t endVector;

  // Зажата [A] - Рисуем линию
  if (btn.a.isPressed())
  {
    if (drawLineColor == COLOR_BLACK && lcd[cursor.vector] > COLOR_WHITE)
    {
      drawLineColor = lcd[cursor.vector];
      uint8_t foundColors = 0;
      for (uint8_t i = 0; i < NUMPIXELS; i++)
      {
        if (lcd[i] == drawLineColor)
        {
          foundColors++;
        }
      }

      if (foundColors > 2)
      {
        allowDrawing = false;
        drawLineColor = COLOR_BLACK;
      }
      else
      {
        startVector = cursor.vector;
        int8_t pointIndex = levelIndex + (drawLineColor - PALETTE_COLOR_FIRST) * 2;
        endVector = startVector == flowLevels[pointIndex] ? flowLevels[pointIndex + 1] : flowLevels[pointIndex];
      }
    }

    // Рисуем линию только на черном фоне
    if (allowDrawing && lcd[cursor.vector] == COLOR_BLACK && cursor.color > COLOR_WHITE)
    {
      cursorToLcd();
    }

    // Если дошли то нужной точки завершаем линию
    if (cursor.isNextVector && endVector == cursor.nextVector)
    {
      allowDrawing = false;
      tone(BUZZER_PIN, NOTE_C5, 30);
      delay(30);
    }
    else if (!allowDrawing || lcd[cursor.nextVector])
    {
      cursor.rollback = true;
    }
  }

  // Отпустили [A] - Завершаем (соединяем) линию
  if (btn.a.wasReleased())
  {
    allowDrawing = true;

    if (cursor.vector != endVector)
    {
      removeColor = drawLineColor;
    }

    if (!removeColor)
    {
      app.needCheck = true;
    }

    drawLineColor = COLOR_BLACK;
  }

  // Нажали [B] - Удаляем цвет
  if (btn.b.isPressed() && cursor.color)
  {
    removeColor = cursor.color;
  }

  // Удаляем цвет
  if (removeColor)
  {
    lcdRemoveColor(removeColor);
    removeColor = 0;
  }

  // Draw points
  for (i = 0; i < (flowLevelsSizes[app.level] * 2); i += 2)
  {
    vectorA = flowLevels[levelIndex + i];
    vectorB = flowLevels[levelIndex + i + 1];
    if (vectorA == vectorB)
    {
      break;
    }
    color = i / 2 + PALETTE_COLOR_FIRST;
    lcd[vectorA] = color;
    lcd[vectorB] = color;
  }

  // Change cursor color
  cursor.color = lcd[cursor.vector] != COLOR_BLACK ? lcd[cursor.vector] : COLOR_DEFAULT;
}

/*
      ###    ########  ########     ######## ######## ######## ########  ####  ######
     ## ##   ##     ## ##     ##       ##    ##          ##    ##     ##  ##  ##    ##
    ##   ##  ##     ## ##     ##       ##    ##          ##    ##     ##  ##  ##
   ##     ## ########  ########        ##    ######      ##    ########   ##   ######
   ######### ##        ##              ##    ##          ##    ##   ##    ##        ##
   ##     ## ##        ##              ##    ##          ##    ##    ##   ##  ##    ##
   ##     ## ##        ##              ##    ########    ##    ##     ## ####  ######
*/

bool appTetrisIsLineFull(uint8_t y)
{
  for (uint8_t x = 0; x < LCD_HEIGHT; x++)
  {
    if (lcd[x + y * LCD_WIDTH] == COLOR_BLACK)
    {
      return false;
    }
  }
  return true;
}

void appTetris()
{
  static bool nextFigure = true;
  static bool soundBeep = false;

  if (!app.isInit)
  {
    nextFigure = true;
    app.timerPeriod = 1000 * TIMER_FIX;
    app.isInit = true;
    cursor.borderTop = false;
    cursor.type = CURSOR_TYPE_ICON;
    cursor.mode = CURSOR_MODE_PARTICLE;
    cursor.controlY = false;
  }

  if (soundBeep)
  {
    soundBeep = false;
    tone(BUZZER_PIN, NOTE_C5, 30);
    delay(300);
  }

  // falling figures
  if (timerEach(btn.down.isPressed() ? 100 * TIMER_FIX : app.timerPeriod))
  {
    cursor.nextY++;
  }

  // Figure is touch
  if (cursor.isTouch && cursor.isTouchDown)
  {
    // Game Over
    if (cursor.isOutsideLcd)
    {
      cursor.nextY = 0;
      gameOver();
      return;
    }

    cursorToLcd(); // Copy figure to LCD
    nextFigure = true;
    soundBeep = true;

    // Remove full lines
    for (int8_t line = 0; line < LCD_HEIGHT; line++)
    {
      if (appTetrisIsLineFull(line))
      {
        lcdRemoveLine(line);
        app.score++;
        app.timerPeriod -= 5;
      }
    }
  }

  if (nextFigure)
  {
    nextFigure = false;

    // Figure cursor
    uint8_t figureIndex = random(0, APP_TETRIS_FIGURES) * 3;
    cursor.color = tetrisFigures[figureIndex];
    cursorSetIcon(tetrisFigures[figureIndex + 1], tetrisFigures[figureIndex + 2], B000);
    cursorSetPosition(LCD_WIDTH / 2 - cursor.width / 2, -cursor.height);
  }

  // Rotate
  if (btn.a.wasPressed())
  {
    cursorRotate();
  }
  if (btn.b.wasPressed())
  {
    cursorRotate();
    cursorRotate();
    cursorRotate();
  }
}

/*
 
       ###    ########  ########     ##     ##    ###    ######## ######## 
      ## ##   ##     ## ##     ##    ###   ###   ## ##        ##  ##       
     ##   ##  ##     ## ##     ##    #### ####  ##   ##      ##   ##       
    ##     ## ########  ########     ## ### ## ##     ##    ##    ######   
    ######### ##        ##           ##     ## #########   ##     ##       
    ##     ## ##        ##           ##     ## ##     ##  ##      ##       
    ##     ## ##        ##           ##     ## ##     ## ######## ######## 
 
*/

void mazeBitWrite(uint8_t x, uint8_t y, bool value)
{
  uint8_t mVector = (x + y * APP_MAZE_MAX_WIDTH) / 8;
  uint8_t mVectorBit = 7 - x % 8;
  bitWrite(maze[mVector], mVectorBit, value);
}

bool mazeGeneratorMoveToDir(uint8_t dir)
{
  int8_t x = generator.x;
  int8_t y = generator.y;

  int8_t xInc = 0;
  int8_t yInc = 0;

  switch (dir)
  {
  case DIRECTION_RIGHT:
    xInc = 1;
    break;
  case DIRECTION_LEFT:
    xInc = -1;
    break;
  case DIRECTION_DOWN:
    yInc = 1;
    break;
  case DIRECTION_UP:
    yInc = -1;
    break;
  }

  x += xInc;
  y += yInc;

  // Out of plane
  if (
      x < 0 ||
      y < 0 ||
      x > APP_MAZE_MAX_WIDTH - 2 ||
      y > APP_MAZE_MAX_HEIGHT - 2)
  {
    return false;
  }

  uint8_t mVector = (x + y * APP_MAZE_MAX_WIDTH) / 8;
  uint8_t mVectorBit = 7 - x % 8;

  x += xInc;
  y += yInc;

  uint8_t mVector2 = (x + y * APP_MAZE_MAX_WIDTH) / 8;
  uint8_t mVector2Bit = 7 - x % 8;

  if (
      x >= 0 &&
      y >= 0 &&
      x <= APP_MAZE_MAX_WIDTH - 2 &&
      y <= APP_MAZE_MAX_HEIGHT - 2)
  {
    if (!bitRead(maze[mVector2], mVector2Bit))
    {
      return false;
    }
  }

  bitClear(maze[mVector], mVectorBit);
  bitClear(maze[mVector2], mVector2Bit);

  generator.direction = dir;
  generator.pathSteps += 2;
  generator.steps += 2;
  generator.x = x;
  generator.y = y;

  return true;
}

// Ищем новый путь (его начало)
void appMazeFindAltPath()
{
  if (generator.randomSteps)
  {
    if (generator.steps < APP_MAZE_GENERATOR_MAX_STEPS / 1.5)
    {
      generator.trackingVector = random(0, APP_MAZE_MAX_VECTOR);
      // generator.trackingVector = Math.round(generator.vector / 2);
    }
    else
    {
      generator.randomSteps = false;
      generator.trackingVector = 0;
    }
  }

  for (uint8_t y = 0; y < APP_MAZE_MAX_HEIGHT; y = y + 2)
  {
    for (uint8_t x = 0; x < APP_MAZE_MAX_WIDTH; x = x + 2)
    {
      uint16_t mVector = (x + y * APP_MAZE_MAX_WIDTH);
      uint8_t mVectorBit = 7 - x % 8;
      bool state = bitRead(maze[mVector / 8], mVectorBit);
      if (!state && mVector >= generator.trackingVector)
      {
        generator.trackingVector = mVector + 1;
        generator.x = x;
        generator.y = y;
        return;
      }
    }
  }
}

// Generator Tick
void appMazeGeneratorTick()
{
  uint8_t dir = generator.direction;

  if (
      generator.findAltPosition ||
      generator.pathSteps > APP_MAZE_MAX_PATH_STEPS)
  {
    generator.pathSteps = 0;
    appMazeFindAltPath();
  }

  if (generator.pathSteps > 1)
  {
    dir = random(0, 3);
  }

  uint8_t attempts = 3;
  bool isMove = false;
  do
  {
    isMove = mazeGeneratorMoveToDir(dir);
    // В зависимости от четности шага перебираем доступные направления по часовой или против
    dir = dir + 1 * (generator.steps % 2 ? -1 : 1);
    // dir++; // Перебор всегда по часовой
    if (dir > 3)
      dir = 0;
    if (dir < 0)
      dir = 3;
  } while (!isMove && attempts--);

  generator.findAltPosition = !isMove;
}

// Генерируем лабиринт
void appMazeGenerate()
{
  // Generate from center
  generator.x = APP_MAZE_MAX_WIDTH / 2;
  generator.y = APP_MAZE_MAX_HEIGHT / 2;
  generator.steps = 0;
  generator.pathSteps = 0;
  generator.trackingVector = 0;
  generator.direction = DIRECTION_DOWN;
  generator.randomSteps = true;
  generator.findAltPosition = false;

  // Fill maze
  for (uint8_t i = 0; i < APP_MAZE_MAX_BYTES; i++)
  {
    maze[i] = 0xFF;
  }
  // Start generate point
  mazeBitWrite(generator.x, generator.y, 0);

  uint16_t attempts = 10000;
  while (generator.steps != APP_MAZE_GENERATOR_MAX_STEPS && attempts > 0)
  {
    appMazeGeneratorTick();
    attempts--;
  }

  // Finish points
  mazeBitWrite(APP_MAZE_MAX_WIDTH - 1, APP_MAZE_MAX_HEIGHT - 1, 0);
  mazeBitWrite(APP_MAZE_MAX_WIDTH - 2, APP_MAZE_MAX_HEIGHT - 1, 0);
}

void appMaze()
{
  static uint8_t mazeIndex = 1;
  const uint8_t mazeSizeX = APP_MAZE_MAX_SCREENS_X;
  const uint8_t mazeSizeY = APP_MAZE_MAX_SCREENS_Y;
  Vector winVector = {LCD_WIDTH * mazeSizeX, LCD_HEIGHT * mazeSizeY - 1};

  static int8_t offsetX;
  static int8_t offsetY;

  if (btn.a.wasPressed())
  {
    app.isInit = false;
  }
  if (btn.b.wasPressed())
  {
    app.isInit = false;
    mazeIndex++;
  }

  // INIT
  if (!app.isInit)
  {
    app.isInit = true;
    offsetX = -APP_MAZE_BORDER_SIZE;
    offsetY = -APP_MAZE_BORDER_SIZE;
    cursor.type = CURSOR_TYPE_PIXEL;
    cursor.moveSound = false;
    cursor.mode = CURSOR_MODE_PARTICLE;
    cursorSetPosition(APP_MAZE_BORDER_SIZE, APP_MAZE_BORDER_SIZE);
    randomSeed(mazeIndex);
    appMazeGenerate();
  }

  // Win
  if (cursor.nextX == LCD_WIDTH)
  {
    gameWin();
    return;
  }

  // Прокрутка вправо
  if (cursor.x >= LCD_WIDTH - APP_MAZE_MOVE_OFFSET)
  {
    if (offsetX < LCD_WIDTH * (mazeSizeX - 1) + APP_MAZE_BORDER_SIZE)
    {
      offsetX++;
      cursorSetPosition(cursor.x - 1, cursor.y);
    }
  }
  // Прокрутка влево
  else if (cursor.x < APP_MAZE_MOVE_OFFSET)
  {
    if (offsetX > -APP_MAZE_BORDER_SIZE)
    {
      offsetX--;
      cursorSetPosition(cursor.x + 1, cursor.y);
    }
  }
  // Прокрутка вниз
  if (cursor.y >= LCD_HEIGHT - APP_MAZE_MOVE_OFFSET)
  {
    if (offsetY < LCD_HEIGHT * (mazeSizeY - 1) + APP_MAZE_BORDER_SIZE)
    {
      offsetY++;
      cursorSetPosition(cursor.x, cursor.y - 1);
    }
  }
  // Прокрутка вверх
  else if (cursor.y < APP_MAZE_MOVE_OFFSET)
  {
    if (offsetY > -APP_MAZE_BORDER_SIZE)
    {
      offsetY--;
      cursorSetPosition(cursor.x, cursor.y + 1);
    }
  }

  // Рисуем часть лабиринта относительно смещений x и y
  for (uint8_t y = 0; y < LCD_HEIGHT; y++)
  {
    for (uint8_t x = 0; x < LCD_WIDTH; x++)
    {
      uint8_t mazeVectorBit = offsetX + x; // Номер бита в маске maze[mazeVector]
      uint8_t mazeVectorX = 0;             // Порядковый номер элемента

      while (mazeVectorBit > LCD_WIDTH - 1)
      {
        mazeVectorX++;
        mazeVectorBit -= LCD_WIDTH;
      }

      // номер элемента мозайки maze[mazeVector]
      uint8_t mazeVector = y * mazeSizeX + (offsetY * mazeSizeX) + mazeVectorX;

      uint8_t pixelColor = COLOR_BLACK;
      if (offsetX + x == winVector.x && offsetY + y == winVector.y)
      {
        pixelColor = COLOR_BLACK;
      }
      // Рамка лабиринта
      else if (offsetX + x < 0 || offsetY + y < 0 || offsetX + x >= LCD_WIDTH * mazeSizeX || offsetY + y >= LCD_HEIGHT * mazeSizeY)
      {
        pixelColor = COLOR_RED;
      }
      // Рисунок лабиринта
      else if (bitRead(maze[mazeVector], (LCD_WIDTH - 1) - mazeVectorBit))
      {
        pixelColor = COLOR_ORANGE + mazeVectorX;
      }

      lcd[x + y * LCD_WIDTH] = pixelColor;
    }
  }
}

/*
 
       ###    ########  ########      ######  ##    ##    ###    ##    ## ######## 
      ## ##   ##     ## ##     ##    ##    ## ###   ##   ## ##   ##   ##  ##       
     ##   ##  ##     ## ##     ##    ##       ####  ##  ##   ##  ##  ##   ##       
    ##     ## ########  ########      ######  ## ## ## ##     ## #####    ######   
    ######### ##        ##                 ## ##  #### ######### ##  ##   ##       
    ##     ## ##        ##           ##    ## ##   ### ##     ## ##   ##  ##       
    ##     ## ##        ##            ######  ##    ## ##     ## ##    ## ######## 
 
*/
#define APP_SNAKE_DIRECTION_RIGHT 0
#define APP_SNAKE_DIRECTION_DOWN 1
#define APP_SNAKE_DIRECTION_LEFT 2
#define APP_SNAKE_DIRECTION_UP 3

void appSnake()
{
  static uint8_t direction;
  static uint8_t nexDirection;
  const uint16_t speed = 200;
  static uint8_t snakeSize;
  static uint16_t targetVector;

  // static uint8_t snake[100] = {};
  // static uint16_t previousVector = cursor.vector;

  // INIT
  if (!app.isInit)
  {
    app.isInit = true;
    app.customRender = true;
    cursor.type = CURSOR_TYPE_PIXEL;
    cursor.mode = CURSOR_MODE_PARTICLE;
    cursor.controlX = false;
    cursor.controlY = false;
    cursor.moveSound = false;
    cursor.color = COLOR_DEEP_BLUE;
    cursorSetPosition(1, 1);
    direction = APP_SNAKE_DIRECTION_RIGHT;
    nexDirection = APP_SNAKE_DIRECTION_RIGHT;
    snakeSize = 3;
  }

  // CONTROLS
  if (btn.right.wasPressed() && direction != APP_SNAKE_DIRECTION_LEFT)
  {
    nexDirection = APP_SNAKE_DIRECTION_RIGHT;
  }
  else if (btn.left.wasPressed() && direction != APP_SNAKE_DIRECTION_RIGHT)
  {
    nexDirection = APP_SNAKE_DIRECTION_LEFT;
  }
  else if (btn.down.wasPressed() && direction != APP_SNAKE_DIRECTION_UP)
  {
    nexDirection = APP_SNAKE_DIRECTION_DOWN;
  }
  else if (btn.up.wasPressed() && direction != APP_SNAKE_DIRECTION_DOWN)
  {
    nexDirection = APP_SNAKE_DIRECTION_UP;
  }

  if (cursor.nextVector == targetVector)
  {
    cursor.moveSound = true;
  }

  // Move
  if (timerEach(speed))
  {
    cursor.moveSound = false;
    // Eat target
    if (cursor.nextVector == targetVector)
    {
      snakeSize++;
      app.level++;

      // change color
      uint8_t newColor;
      do
      {
        newColor = random(PALETTE_COLOR_FIRST, PALETTE_COLOR_LAST);
      } while (newColor == cursor.color);
      cursor.color = newColor;

      targetVector = random(0, NUMPIXELS - 1);
      while (lcd[targetVector])
      {
        targetVector++;
        if (targetVector >= NUMPIXELS)
        {
          targetVector = 0;
        }
      }
    }
    // Иначе уменьшаем жизнь частицам
    else
    {
      for (uint8_t i = 0; i < NUMPIXELS; i++)
      {
        if (lcd[i])
          lcd[i]--;
      }
    }

    // Game over
    if (lcd[cursor.vector])
    {
      gameOver();
    }

    direction = nexDirection;
    lcd[cursor.vector] = snakeSize - 1;
    // pixels.show();

    switch (direction)
    {
    case APP_SNAKE_DIRECTION_RIGHT:
      cursor.nextX++;
      if (cursor.nextX >= LCD_WIDTH)
        cursor.nextX = 0;
      break;

    case APP_SNAKE_DIRECTION_LEFT:
      cursor.nextX--;
      if (cursor.nextX < 0)
        cursor.nextX = LCD_WIDTH - 1;
      break;

    case APP_SNAKE_DIRECTION_DOWN:
      cursor.nextY++;
      if (cursor.nextY >= LCD_HEIGHT)
        cursor.nextY = 0;
      break;

    case APP_SNAKE_DIRECTION_UP:
      cursor.nextY--;
      if (cursor.nextY < 0)
        cursor.nextY = LCD_HEIGHT - 1;
      break;
    }
  }

  // CUSTOM RENDER
  pixels.clear();
  for (uint8_t i = 0; i < NUMPIXELS; i++)
  {
    if (lcd[i])
    {
      // pixels.setPixelColor(i, paletteRGB[((snakeSize - lcd[i]) % 3) == 0 ? COLOR_LIGHT_BLUE : COLOR_DEEP_BLUE]);
      pixels.setPixelColor(i, pixels.ColorHSV(paletteHue[cursor.color], 0xFF, map(lcd[i], 1, snakeSize, 50, 250)));
    }
    pixels.setPixelColor(targetVector, paletteRGB[COLOR_WHITE]);
  }
  cursorRender();
}

/*
 
       ###    ########  ########     ######## #### ##     ## ######## ########  
      ## ##   ##     ## ##     ##       ##     ##  ###   ### ##       ##     ## 
     ##   ##  ##     ## ##     ##       ##     ##  #### #### ##       ##     ## 
    ##     ## ########  ########        ##     ##  ## ### ## ######   ########  
    ######### ##        ##              ##     ##  ##     ## ##       ##   ##   
    ##     ## ##        ##              ##     ##  ##     ## ##       ##    ##  
    ##     ## ##        ##              ##    #### ##     ## ######## ##     ## 
 
*/
void appTimer()
{
  static uint8_t time = 0;
  static uint8_t decimals = 0;

  if (btn.a.wasPressed())
  {
    time = 0;
    decimals = 0;
  }

  if (timerEach(1000 * TIMER_FIX))
  {
    if (time == 60)
    {
      time = 0;
      decimals++;
    }
    uint8_t tens;
    uint8_t ones;

    tens = time / 10;
    ones = time % 10;

    lcdChar(1, 2, tens, COLOR_PINK);
    lcdChar(4, 2, ones, COLOR_DEEP_BLUE);

    for (uint8_t i = 0; i < LCD_WIDTH; i++)
    {
      lcd[i] = decimals > i ? COLOR_PINK : COLOR_DEEP_BLUE;
    }

    time++;
  }
}

/*
      ###    ########  ########     ##     ## ######## ##    ## ##     ##
     ## ##   ##     ## ##     ##    ###   ### ##       ###   ## ##     ##
    ##   ##  ##     ## ##     ##    #### #### ##       ####  ## ##     ##
   ##     ## ########  ########     ## ### ## ######   ## ## ## ##     ##
   ######### ##        ##           ##     ## ##       ##  #### ##     ##
   ##     ## ##        ##           ##     ## ##       ##   ### ##     ##
   ##     ## ##        ##           ##     ## ######## ##    ##  #######
*/
void appMenu()
{
  // lcd[0] = COLOR_RED;
  // return;
  static uint8_t charNumber = 1;

  if (btn.right.wasPressed() && charNumber < MAX_APPS)
  {
    charNumber++;
  }
  if (btn.left.wasPressed() && charNumber > 1)
  {
    charNumber--;
  }

  if (btn.a.wasPressed())
  {
    soundBeep();
    currentApp = charNumber;
    // lcdSplahScreen(SPLASH_SCREEN_FADE_OUT);
    lcdClear();
    return;
  }

  switch (charNumber)
  {
  case 1:
    lcdImage(iconPaint);
    break;
  case 2:
    lcdImage(iconFlow);
    break;
  case 3:
    lcdImage(iconTetris);
    break;
  case 4:
    lcdImage(iconMaze);
    break;
  case 5:
    lcdImage(iconSnake);
    break;
  case 6:
    lcdImage(iconTimer);
    break;
  }
  // lcdChar(3, 2, charNumber, charNumber + PALETTE_COLOR_FIRST - 1);
  // lcdChar(0, 0, 'A', COLOR_RED);
}

//-----------------------------------------------------------------------------

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

  btn.up.begin();
  btn.down.begin();
  btn.left.begin();
  btn.right.begin();
  btn.a.begin();
  btn.b.begin();

  pixels.begin();
  pixels.setBrightness(LCD_MAX_BRIGHTNESS);

  // Prepare color palette
  for (uint8_t i = PALETTE_COLOR_FIRST; i < PALETTE_COLORS; i++)
  {
    paletteRGB[i] = pixels.ColorHSV(paletteHue[i]);
  }
  paletteRGB[COLOR_BLACK] = 0x000000;
  paletteRGB[COLOR_WHITE] = 0xFFFFFF;

  // Cursor
  resetCursor();
  cursorSetPosition(LCD_WIDTH / 2, LCD_HEIGHT / 2);

  lcdClear();

  // Serial.begin(9600);
  // Serial.println(APP_MAZE_GENERATOR_MAX_STEPS);
  // Serial.end();

  // randomSeed(analogRead(0));
  // random(0, analogRead(0));
  randomSeed(11);
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
  btn.up.read();
  btn.left.read();
  btn.right.read();
  btn.down.read();
  btn.a.read();
  btn.b.read();

  // Blink
  if (blink.timer)
  {
    if (millis() >= blink.timer)
    {
      blink.timer = 0;
    }
  }
  else
  {
    blink.timer = millis() + BLINK_INTERVAL;
    blink.toggler = !blink.toggler;
  }

  if (blink.toggler)
  {
    if (blink.level < 0xFF - CURSOR_BLINK_STEP)
      blink.level += CURSOR_BLINK_STEP;
  }
  else
  {
    if (blink.level > CURSOR_BLINK_FADE)
      blink.level -= CURSOR_BLINK_STEP;
  }

  // Cursor
  cursorTick();

  switch (currentApp)
  {
  case 1:
    appPaint();
    break;
  case 2:
    appFlow();
    break;
  case 3:
    appTetris();
    break;
  case 4:
    appMaze();
    break;
  case 5:
    appSnake();
    break;
  case 6:
    appTimer();
    break;

  default:
    appMenu();
  }

  if (app.customRender == false)
  {
    lcdRender();
    cursorRender();
  }
  pixels.show();
}
