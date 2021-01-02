#ifndef SOUND_h
#define SOUND_h

#include <header.h>

const uint8_t soundScale[] PROGMEM = {239, 226, 213, 201, 190, 179, 169, 160, 151, 142, 134, 127};

class Sound
{
public:
    unsigned long noteDurationMillis = 0;
    uint8_t noteDuration = 0;
    boolean outputTone = false; // Records current state

    void setup()
    {
        pinMode(BUZZER_PIN, OUTPUT);
    }

    bool notPlayed()
    {
        return this->noteDurationMillis == 0;
    }

    void processing()
    {
        if (!this->notPlayed())
        {
            if (millis() >= this->noteDurationMillis)
            {
                this->_note(0);
                this->noteDurationMillis = 0;
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
        this->noteDurationMillis = millis() + duration;
        this->_note(note);
    }

private:
    // void _note(uint8_t noteOctave)
    // {
    //     uint8_t note = (noteOctave >> 4);
    //     uint8_t octave = noteOctave & 0xF;
    //     this->_note(note, octave);
    // }

    // http://www.technoblogy.com/show?19YT
    // Range: C3 - B6
    void _note(uint8_t note)
    {
        note--;
        TCCR2A = 0 << COM2A0 | 1 << COM2B0 | 2 << WGM20; // Toggle OC2B on match
        // int prescaler = 9 - (octave + n / 12);
        int prescaler = 9 - (3 + note / 12);
        if (prescaler < 3 || prescaler > 6)
        {
            prescaler = 0;
        }
        OCR2A = pgm_read_byte(&soundScale[note % 12]) - 1;
        TCCR2B = 0 << WGM22 | prescaler << CS20;
    }
};

#endif