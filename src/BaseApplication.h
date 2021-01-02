#ifndef BASEAPP_h
#define BASEAPP_h

#include <header.h>
#include <Lcd/Lcd.h>
#include <Cursor.h>
#include <Controls/Controls.h>

extern Lcd lcd;
extern Cursor cursor;
extern Controls ctrl;

class BaseApplication
{
public:
    virtual uint8_t *icon()
    {
        return (uint8_t *)0;
    };
    virtual void start(){};
    virtual void processing(){};
    virtual void postProcessing(){};
};

#endif