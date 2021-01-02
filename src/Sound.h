/*
    Original idea: http://www.technoblogy.com/show?19YT
*/

#ifndef SOUND_h
#define SOUND_h

#include <header.h>

const uint8_t soundScale[] PROGMEM = {239, 226, 213, 201, 190, 179, 169, 160, 151, 142, 134, 127};

class Sound
{
public:
    unsigned long noteDuration = 0;
    // boolean outputTone = false; // Records current state

    void setup()
    {
        pinMode(BUZZER_PIN, OUTPUT);
    }

    void processing()
    {
        if (this->noteDuration)
        {
            if (millis() >= this->noteDuration)
            {
                this->playNote(0);
                this->noteDuration = 0;
            }
        }
    }

    void tick()
    {
        this->tone(NOTE_C3, SOUND_TICK_DURATION);
    }

    void beep()
    {
        this->tone(NOTE_C4, SOUND_BEEP_DURATION);
    }

    void tone(uint8_t note, uint8_t duration, uint16_t silentAfter = 0)
    {
        this->noteDuration = millis() + duration;
        this->playNote(note);
    }

private:
    // Range: C3 - B6
    void playNote(uint8_t note)
    {
        int prescaler = 0;

        if (note)
        {
            note--;
            prescaler = 9 - (3 + note / 12);
            if (prescaler < 3 || prescaler > 6)
            {
                prescaler = 0;
            }
            else
            {
                TCCR2A = 0 << COM2A0 | 1 << COM2B0 | 2 << WGM20;
                OCR2A = pgm_read_byte(&soundScale[note % 12]) - 1;
            }
        }

        TCCR2B = 0 << WGM22 | prescaler << CS20;
    }
};

#endif