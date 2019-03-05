
#include <U8g2lib.h>
#include <string.h>
#include <TimerOne.h>

// Constructor for the U8G2 library for the Addicore I2C OLED 128x64 display
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const unsigned int DISPLAY_WIDTH = 115;

unsigned int hour = 12;
unsigned int minute = 58;
volatile unsigned int seconds = 50;
unsigned int displaySeconds = 0;
unsigned int displayHour = hour;
char timeString[6] = {0};
char secondsString[3] = {0};
char amPm = 'A';

// -------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------
// Initialize the OLED display and timer interrupts.
void setup(void) {
    u8g2.begin();

    updateHourDisplay();

    // Generated the initial formatted time string to be displayed.
    snprintf(timeString, 6, "%2d:%02d", displayHour, minute);
    
    // Set the timer to trigger the interrupt handler every 1 second.
    Timer1.initialize(1000000);
    Timer1.attachInterrupt(ticker);
}

// -------------------------------------------------------------------------
// Main Looop
// -------------------------------------------------------------------------
void loop(void) {

    // We only need to do something if the seconds counter has changed
    // since the last time we updated the display.
    if (seconds == displaySeconds)
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
                
            if (hour == 25)
            {
                hour = 1;
            }

            updateHourDisplay();
        }

        // Generated the formatted time string to be displayed.
        snprintf(timeString, 6, "%2d:%02d", displayHour, minute);
    }
        
    // Capture the current value for seconds, so that we don't end up displaying
    // the incorrect value if it was changed by the interrupt handler.
    displaySeconds = seconds;

    // Start triggering interrupts again.
    interrupts();
    
    // Update the display with the current time.
    strcpy(secondsString, u8x8_u8toa(displaySeconds, 2));
    u8g2.firstPage();
    u8g2.setContrast(50);
    
    do 
    {
        // Display the time
        u8g2.setFont(u8g2_font_fur35_tn);
        u8g2.drawStr(
            DISPLAY_WIDTH - u8g2.getStrWidth(timeString), 
            u8g2.getAscent() + 4, 
            timeString
        );

        // Display seconds counter
        u8g2.setFont(u8g2_font_profont22_mn);
        u8g2.drawStr(
            DISPLAY_WIDTH - u8g2.getStrWidth(secondsString), 
            63, 
            secondsString
        );

        // Display AM/PM
        u8g2.setFont(u8g2_font_profont12_mr);
        u8g2.drawGlyph(122, 15, amPm);
        u8g2.drawGlyph(122, 28, 'M');
    } while ( u8g2.nextPage() );
}

// -------------------------------------------------------------------------
// Ticker - interrupt handler
// Triggers every 1 second to update the seconds counter variable.
// -------------------------------------------------------------------------
void ticker(void)
{
    seconds++;
}

// -------------------------------------------------------------------------
// Update AM/PM indicator based on the hour
// -------------------------------------------------------------------------
void updateHourDisplay()
{
    displayHour = (hour - 1) % 12 + 1;
    amPm = (hour == 24 || hour < 12) ? 'A' : 'P';
}

