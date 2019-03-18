#include <Arduino.h>
#include <U8x8lib.h>

#include <string.h>
#include <TimerOne.h>

uint8_t hour = 9;
uint8_t minute = 52;
volatile uint8_t seconds = 0;
unsigned int _seconds = 0;
char timeString[6] = {0};

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 	      

void setup(void)
{
    u8x8.begin();
    u8x8.setPowerSave(0);

    // Set the timer to trigger the interrupt handler every 1 second.
    Timer1.initialize(1000000);
    Timer1.attachInterrupt(ticker);
}

void loop(void)
{
    
    u8x8.setFont(u8x8_font_artossans8_r);

    // We only need to do something if the seconds counter has changed
    // since the last time we updated the display.
    if (seconds == _seconds)
        return;
        
    // Stop triggering interrupts while we're calculating the current time.
    // We don't want to be changing the seconds variable at the same time as
    // the interrupt handler.
    noInterrupts();
    
    if ( seconds == 60 )
    {
        seconds = 0;
        minute++;
        
        if (minute == 60)
        {
            minute = 0;
            hour++;
            
            if (hour == 13) 
            {
                hour = 1;
            }
        }
    }

    // Capture the current value for seconds, so that we don't end up displaying
    // the incorrect value if it was changed by the interrupt handler.
    _seconds = seconds;

    // Start triggering interrupts again.
    interrupts();

    // Generated a formatted time string to be displayed.
    snprintf(timeString, 6, "%2d:%02d", hour, minute);

    u8x8.draw2x2String(0, 1, timeString);
    u8x8.drawGlyph(10, 2, ':');
    u8x8.drawString(11, 2, u8x8_u8toa(_seconds, 2));
    u8x8.drawString(14, 2, "AM");
    
    u8x8.drawString(0, 6, "June 2, 2017");
}

// Ticker - interrupt handler
// -------------------------------------------------------------------------
// Triggers every 1 second to update the seconds counter variable.
void ticker(void)
{
    seconds++;
}
