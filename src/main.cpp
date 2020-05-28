/*
           _____                    _____                    _____          
          /\    \                  /\    \                  /\    \         
         /::\    \                /::\    \                /::\    \        
        /::::\    \              /::::\    \              /::::\    \       
       /::::::\    \            /::::::\    \            /::::::\    \      
      /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \     
     /:::/  \:::\    \        /:::/__\:::\    \        /:::/__\:::\    \    
    /:::/    \:::\    \      /::::\   \:::\    \      /::::\   \:::\    \   
   /:::/    / \:::\    \    /::::::\   \:::\    \    /::::::\   \:::\    \  
  /:::/    /   \:::\ ___\  /:::/\:::\   \:::\____\  /:::/\:::\   \:::\ ___\ 
 /:::/____/  ___\:::|    |/:::/  \:::\   \:::|    |/:::/__\:::\   \:::|    |
 \:::\    \ /\  /:::|____|\::/   |::::\  /:::|____|\:::\   \:::\  /:::|____|
  \:::\    /::\ \::/    /  \/____|:::::\/:::/    /  \:::\   \:::\/:::/    / 
   \:::\   \:::\ \/____/         |:::::::::/    /    \:::\   \::::::/    /  
    \:::\   \:::\____\           |::|\::::/    /      \:::\   \::::/    /   
     \:::\  /:::/    /           |::| \::/____/        \:::\  /:::/    /    
      \:::\/:::/    /            |::|  ~|               \:::\/:::/    /     
       \::::::/    /             |::|   |                \::::::/    /      
        \::::/    /              \::|   |                 \::::/    /       
         \::/____/                \:|   |                  \::/____/        
                                   \|___|                   ~~              
                                                                            
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
CursorStructure cursor = {};
ButtonStructure btn = {Button(BTN_UP), Button(BTN_DOWN), Button(BTN_LEFT), Button(BTN_RIGHT), Button(BTN_A), Button(BTN_B)};
BlinkStructure blink = {false, 0, 0};
uint8_t currentApp = DEFAULT_APP;
AppStructure app = {};

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

  if (now >= app.timer + period)
  {
    app.timer = now;
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
 * @param  {uint8_t} lcdX      :
 * @param  {uint8_t} lcdyY     :
 * @param  {uint8_t} charIndex :
 * @param  {uint8_t} color     :
 */
void lcdChar(uint8_t lcdX, uint8_t lcdyY, uint8_t charIndex, uint8_t color)
{
  int8_t x = 0;
  int8_t y = 0;
  charIndex = charIndex * 3;
  for (uint8_t i = 0; i < 3; i++)
  {
    for (uint8_t j = 0; j < 8; j++)
    {
      x = j > 3 ? j - 4 : j;

      lcd[(lcdX + x) + (lcdyY + y) * LCD_WIDTH] = bitRead(font[charIndex + i], 7 - j) ? color : COLOR_BLACK;

      if (x == 3)
      {
        y++;
      }
    }
  }
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

/**
 * Выводим изображение на экран
 */
void lcdRender()
{
  uint8_t i, j;
  for (i = 0; i < NUMPIXELS; i++)
  {
    lcdDrawPixel(i, lcd[i]);
  }

  if (cursor.type)
  {
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

  pixels.show();
}

/**
 * Выводим заставку
 * @param  {uint8_t} type :
 */
void lcdSplashScreen(uint8_t type)
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
      //
  };

  int noteDurations[] = {
      4, 8, 8, 4, 8, 8,
      4, 8, 8, 4, 8, 8,
      4, 8, 8, 4, 4,
      4, 4, 4, 4,
      //
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
    if (cursor.isNextVector)
    {
      soundBeep();
    }

    cursor.isTouch = false;
    cursor.vector = cursor.nextVector;
    cursor.x = cursor.nextX;
    cursor.y = cursor.nextY;
  }

  // Управление по X
  if (btn.right.wasPressed() || (btn.right.pressedFor(LONG_PRESS_WAIT) && isLongPressTick()))
  {
    cursor.nextX = cursor.x + 1;
  }
  if (btn.left.wasPressed() || (btn.left.pressedFor(LONG_PRESS_WAIT) && isLongPressTick()))
  {
    cursor.nextX = cursor.x - 1;
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
  app.level = 0;
  app.score = 0;
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
  cursor.borderTop = true;
  cursor.isTouch = false;
  cursor.isTouchDown = false;
  cursor.isOutsideLcd = false;
}

/**
 * Конец игры
 */
void gameOver()
{
  tone(BUZZER_PIN, NOTE_C3, 200);
  delay(300);
  lcdSplashScreen(SPLASH_SCREEN_RAINBOW);
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
  lcdSplashScreen(SPLASH_SCREEN_RAINBOW);
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
      lcdSplashScreen(SPLASH_SCREEN_FLASH);
      delay(300);
      lcdSplashScreen(SPLASH_SCREEN_FADE_OUT);
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
    app.timerPeriod = 1000;
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
  if (timerEach(btn.down.isPressed() ? 100 : app.timerPeriod))
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
void appMaze()
{
  const uint8_t mazeSizeX = APP_MAZE_MAX_SCREENS_X;
  const uint8_t mazeSizeY = APP_MAZE_MAX_SCREENS_Y;
  Vector winVector = {LCD_WIDTH * mazeSizeX, LCD_HEIGHT * mazeSizeY - 1};

  static int8_t offsetX;
  static int8_t offsetY;

  // INIT
  if (!app.isInit)
  {
    app.isInit = true;
    offsetX = -APP_MAZE_BORDER_SIZE;
    offsetY = -APP_MAZE_BORDER_SIZE;
    cursor.type = CURSOR_TYPE_PIXEL;
    cursor.mode = CURSOR_MODE_PARTICLE;
    cursorSetPosition(APP_MAZE_BORDER_SIZE, APP_MAZE_BORDER_SIZE);
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

  if (btn.down.wasPressed() && charNumber < MAX_APPS)
  {
    charNumber++;
  }
  if (btn.up.wasPressed() && charNumber > 1)
  {
    charNumber--;
  }

  if (btn.a.wasPressed())
  {
    soundBeep();
    currentApp = charNumber;
    lcdSplashScreen(SPLASH_SCREEN_FADE_OUT);
    lcdClear();
    return;
  }

  lcdChar(3, 2, charNumber, charNumber + PALETTE_COLOR_FIRST - 1);
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
  // Serial.print("ok");
  // Serial.end();

  randomSeed(analogRead(0));
  random(0, analogRead(0));
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

  default:
    appMenu();
  }

  if (cursor.color == COLOR_WHITE)
  {
    cursor.colorRGB = pixels.Color(blink.level, blink.level, blink.level);
  }
  else
  {
    cursor.colorRGB = pixels.ColorHSV(paletteHue[cursor.color], 0xFF, blink.level);
  }

  lcdRender();
}
