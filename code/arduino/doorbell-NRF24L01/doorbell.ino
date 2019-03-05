/*
 *  DOORBELL UNIT - NRF24L01+
 *  ---------------------------------------------------------------------------
 *  This version of the doorbell unit uses the NRF24L01+ 2.4GHz radio to send
 *  ring notifications to the base unit.
 *  This implementation is the first version of the doorbell, with limitted
 *  functionality - i.e. Just a doorbell.  The initial vision was for a full
 *  door monitoring system with a motion sensor to wake the device, camera and
 *  audio interaction with the person at the door, a weather station capturing
 *  temperature, humidity, and barometric pressure.
 *
 *  ATTRIBUTIONS
 *
 *  Uses the optimized RF24 library from TMRh20. https://github.com/TMRh20/RF24
 *  ---------------------------------------------------------------------------
 */

#include <avr/sleep.h>
#include <avr/power.h>
#include <SPI.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define pins used by NRF24L01+ for chip select and chip enable
#define CE_PIN   9
#define CSN_PIN  10

// This is the address of the receiver.
const uint64_t pipe = 0xE8E8F0F0E1LL;

// The message sent to the base unit is an array or two integers - one identifying
// the network (in this case, I used our house number), and another that identifies
// the specific sending unit.  This allows for multiple units to be added to the
// network - e.g. front door, back door, garage, etc.
int message[2];

// Define the NRF24L01+ radio object
RF24 radio(CE_PIN, CSN_PIN);

const int button = 3;
const int led = 4;
const int debounceDelay = 500;
const int activityTimeout = 10000;

volatile bool ringRequested = false;
volatile int lastPressed = 0;

bool ledOn = false;
bool startingUp;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Initialize NRF24L01+ radio to enable communication.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void initializeComm()
{
    message[0] = 517;       // Network ID
    message[1] = 1;         // Unit ID

    radio.begin();
    radio.openWritingPipe( pipe );

    delay(50);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void sendNotify()
{
    radio.write( message, sizeof( message ) );

    flashLed(4);
}

/* ------------------- MAIN HANDLERS & LOOP -------------------------------- */

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void flashLed(int repeat)
{
    for (byte i = 0; i < repeat; i++)
    {
        digitalWrite(led, LOW);
        delay(100);
        digitalWrite(led, HIGH);
        delay(100);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void buttonPressed()
{
    // Handle button debouncing by not handling a button press within 1 second of the previous press.
    if ( !startingUp && !ringRequested && (millis() - lastPressed > debounceDelay) )
    {
        ringRequested = true;
        ledOn = true;
        lastPressed = millis();
    }

    startingUp = false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void wakeUp()
{
    sleep_disable();

    // When we wake up, we want to trigger the button press event
    // Remove the wakeUp interrupt handler
    detachInterrupt(INT1);

    #ifdef DEBUGMODE
        Serial.println("wake!");
    #endif

    // Restore AVR peripherals used elsewhere in the code
    power_usart0_enable(); // Enable Serial
    power_timer0_enable(); // Enable millis() etc.
    power_spi_enable();    // Enable SPI

    // Turn on the NRF24L01+ radio.
    radio.powerUp();

    // Set up button to trigger an interrupt.
    attachInterrupt(INT1, buttonPressed, FALLING);

    delay(50);

    // Simulate button press to set the correct flags
    buttonPressed();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// goToSleep()
// Powers down communication module and microcontroller. The normal button
// interrupt handler is disconnected, and another handler that handles the
// wakup is attached.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void goToSleep(void)
{
    noInterrupts();
    detachInterrupt(INT1);
    delay(50);
    attachInterrupt(INT1, wakeUp, LOW);
    interrupts();
    delay(50);

    digitalWrite(led, LOW);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    radio.powerDown();

    power_spi_disable();    // Disable SPI
    delay(50);              // Stall for last serial output
    power_timer0_disable(); // Disable millis() etc.
    power_usart0_disable(); // Disable Serial

    // Go to sleep
    sleep_mode();

    /* The program will continue from here. */
    // ....
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Setup
// Initializes the communication module, and configures the LED and button.
// The button presses are handled with an interrupt handler rather than
// polling.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void setup()
{
    startingUp = true;
    ringRequested = false;

    #ifdef DEBUGMODE
        Serial.begin(115200);
        delay(10);
        Serial.println();
        Serial.println();
        Serial.print("Configuring...");
    #endif

    pinMode(button, INPUT_PULLUP);
    pinMode(led, OUTPUT);

    initializeComm();

    flashLed(3);
    ledOn = true;

    // Set up button to trigger an interrupt.
    noInterrupts();
    attachInterrupt(INT1, buttonPressed, FALLING);
    interrupts();
    delay(50);

    startingUp = false;

    #ifdef DEBUGMODE
        Serial.println();
        Serial.print("Done.");
    #endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Main loop
// Sends the wireless notification to the base unit if someone presses the
// doorbell.  Flashes the LED then leaves it solid on for 20 seconds.  If
// the bell is not pressed again in that time, the device is put to sleep.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop()
{
    if ( ringRequested )
    {
        #ifdef DEBUGMODE
            Serial.println( "Button pressed" );
            Serial.println( "Sending message to base station ..." );
        #endif

        sendNotify();

        // Reset the ring request flag so button presses are allowed again.
        ringRequested = false;
    }

    // Let's check if the unit has been awake longer than the activity timeout
    // since the last time the button was pressed.  If so, put the unit to sleep.

    int timePassed = millis() - lastPressed;

    if ( timePassed > activityTimeout )
    {
        flashLed(4);
        digitalWrite(led, LOW);
        ledOn = false;

        goToSleep();
    }
    else
    {
        // When the button is pressed, flash the LED every half a second for 2 seconds
        // to let the user know that something is happening.
        if ( timePassed < 2000 && (0 < (timePassed % 500) < 10) )
        {
            flashLed(1);
        }
    }
}
